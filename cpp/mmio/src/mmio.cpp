#include "mmio.h"

#include <sys/mman.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <omp.h>
#include <time.h>

#include <stdio.h>

static int mem_refc = 0; // ref count for /dev/mem
static int mem_fd = 0;   // fd for /dev/mem


uint32_t* peripheral::map_io() noexcept {
	if( mem_fd <= 0 )                                // if not already mapped, try
		mem_fd = open( "/dev/mem", O_RDWR, O_SYNC );
	if( mem_fd < 0 )                                        // if fail, give up
		return nullptr;
	++mem_refc;                                             // update ref count
	return static_cast<uint32_t*>(mmap( (void*) address::io_phys, address::io_size, // mmap
		PROT_READ | PROT_WRITE | PROT_EXEC, MAP_SHARED, mem_fd,
		static_cast<off_t>(address::io_phys) ));
}

int peripheral::unmap_io( uint32_t* addr ) noexcept {
	int ret;
#ifdef DEBUG
	if( mem_refc <= 0 ) { // check for double-free
		fprintf( 2, "double-free on peripheral at addr %h with size %h\n",
			reinterpret_cast<uint64_t>(addr), static_cast<uint64_t>(len) );
		return -1;
	}
#endif
	// it's getting unmapped anyway, and the const compatibility is nice
	ret = munmap( static_cast<void*>(addr), address::io_size); // don't do this
	--mem_refc;               // update ref count
	if( mem_refc == 0 ) {
		close( mem_fd );
		mem_fd = 0;
	}
	return ret;
}

int peripheral::decrement_refc() noexcept {
	if( mem_refc > 0 )
		--mem_refc;
	return mem_refc;
}

void peripheral::fill_recognisable( uint32_t* const addr,
	                                  const uint64_t size ) noexcept {
	srand(static_cast<unsigned int>(time(NULL)));
	const char step = static_cast<char>(rand() % 1<<8);
	char value = 1;
	__builtin_prefetch(addr);
	#pragma omp parallel
	#pragma omp for schedule(dynamic, 64)
	for( unsigned long long i = 0; i < size; i++ ) {
		addr[i] = value;
		value += step;
	}
}

void peripheral::gpio_fs( uint32_t* const base_io, const int pin,
	                                             const int function ) noexcept {
	uint32_t* const base_gpio_fs =
		address::offset( base_io, address::offset_gpio + address::offset_gpio_fs );
	// 10 fs registers per word. Select the correct word
	// then shift by remainder * bits per function select register
	uint32_t* const gpio_fs = &base_gpio_fs[pin/10];
	const uint_fast8_t shift = (pin%10)*3;
	//printf("writing 0x%llx = 0x%x (setting function)\n", (uint64_t)gpio_fs, static_cast<uint32_t>((0b1<<((pin%10)*3))));
	*gpio_fs &= ~ static_cast<uint32_t>((0b111<<shift)); // clear function
	*gpio_fs |= static_cast<uint32_t>(function<<shift);  // set function
	//printf("to  %x = %x\n", base_gpio_fs, *base_gpio_fs);

}

void peripheral::gpio_write( uint32_t* const base_io, const int pin,
	                                             const bool level ) noexcept {
	const uint_fast8_t shift = pin%32;
	if(level) {
		uint32_t* const gpio_set =
			address::offset( base_io, address::offset_gpio +
			                          address::offset_gpio_out_set );
		//printf("writing 0x%llx = 0x%llx (pull high)\n", (uint64_t)&gpio_set[pin/32], (uint64_t)1<<(pin%32));
		gpio_set[pin/32] = 1<<shift;
	} else {
		uint32_t* const gpio_clr =
			address::offset( base_io, address::offset_gpio +
			                          address::offset_gpio_out_clr );
		//printf("writing 0x%llx = 0x%llx (pull low)\n", (uint64_t)&gpio_clr[pin/32], (uint64_t)1<<(pin%32));
		gpio_clr[pin/32] = 1<<shift;
	}
}

bool peripheral::gpio_read( uint32_t* base_io, int pin ) noexcept {
	uint32_t* const gpio_lev =
		address::offset( base_io, address::offset_gpio +
															address::offset_gpio_lev );
	const uint_fast8_t shift = pin%32;
	return *gpio_lev & 1<<shift;
}

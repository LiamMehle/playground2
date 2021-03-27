#include "mmio.h"

#include <sys/mman.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <omp.h>
#include <time.h>



static int mem_refc = 0; // ref count for /dev/mem
static int mem_fd = 0;   // fd for /dev/mem

void* peripheral::map( const uint64_t addr, const size_t len ) noexcept {
	if( mem_fd <= 0 ) // if not already mapped, try
		mem_fd = open( "/dev/mem", O_RDWR | O_SYNC );
	if( mem_fd < 0 )  // if fail, give up
		return nullptr;
	++mem_refc;       // update ref count
	return mmap( reinterpret_cast<void*>(addr), len,
	       PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, static_cast<off_t>(addr) );
}

int peripheral::unmap( const void* const addr, const size_t len ) noexcept {
	int ret;
#ifdef DEBUG
	if( mem_refc <= 0 ) { // check for double-free
		fprintf( 2, "double-free on peripheral at addr %h with size %h\n",
		         reinterpret_cast<long long>(addr), static_cast<long long>(len) );
		return -1;
	}
#endif
	// it's getting unmapped anyway, and the const compatibility is nice
	ret = munmap( const_cast<void*>(addr), len ); // don't do this
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

void peripheral::fill_recognisable( char* const addr,
	                                  const size_t size ) noexcept {
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

#include <stdio.h>
#include "mmio.h"
#include <errno.h>

int main() {
	namespace peri=peripheral;
	namespace addr=peri::address;
	// get access to memory-mapped gpio gregion
	uint32_t* const io_base = peri::map_io();

	if( io_base == NULL || io_base == (uint32_t*)-1 ) {
		printf( "[ERROR] peripheral::map() returned %d\n", errno );
		return errno;
	}

	// get address of function select registers, then write to the right one
	peri::gpio_fs( io_base, 0, peri::out ); // pin14 to output
	peri::gpio_write( io_base, 0, peri::high );
	//printf("%x\n", io_base[addr::offset_gpio]);
	// cleanup
	peri::unmap_io( io_base ); // made it explicit for sake of making it clear
	                             // this is an invalid pointer now.
	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include "mmio.h"
#include <errno.h>

int main() {
	namespace peri=peripheral;
	namespace addr=peri::address;
	puts("init");
	// get access to memory-mapped gpio gregion
	[[maybe_unused]]
	const uint_fast8_t gpio_size = 0xB0;
	char* const gpio_base = static_cast<char*>(
		peri::map( addr::io_phys + addr::offset_gpio, gpio_size )); // plenty

	if( gpio_base == NULL ) {
		printf( "[ERROR] peripheral::map() returned %d\n", errno );
		return errno;
	}
	// function select is at the very bottom of the structure. 0 is in, 1 is out;
	gpio_base[0] |= 1;

	gpio_base[addr::offset_gpio_output_clr] |= 1;

	// cleanup
	peri::unmap( gpio_base, gpio_size );
	puts("end");
	return 0;
}

#include <stdio.h>
#include "mmio.h"
#include <errno.h>
#include <unistd.h>
#include <time.h>

void msleep( uint_fast16_t msec ) {
	const timespec req {
		.tv_sec = static_cast<__time_t>(msec/1000),
		.tv_nsec = static_cast<__syscall_slong_t>((msec%1000)*1000*1000 )
	};
	timespec rem {};
	int ret = nanosleep( &req, &rem );
	while( ret != 0 ) { // if interrupted
		ret = nanosleep( &rem, &rem );
	}
}

int main() {
	namespace peri=peripheral;
	namespace addr=peri::address;
	// get access to memory-mapped gpio gregion
	uint32_t* const io_base = peri::map_io();

	if( io_base == NULL || io_base == (uint32_t*)-1 ) {
		printf( "[ERROR] peripheral::map() returned %d\n", errno );
		return errno;
	}

	auto test = []( uint32_t* const io_base, int function ){
	for( uint_fast8_t pin = 0; pin < 28; pin++ ) { // all pins
		for( uint_fast8_t value = 0; value <= 1; value++) { // both values
				bool wr_value = value;
				printf("%2u=%u\t", pin, value );
				peri::gpio_fs( io_base, pin, function );
				peri::gpio_write( io_base, pin, wr_value );
				msleep(10);
				bool rd_value = peri::gpio_read( io_base, pin );
				if ( wr_value == rd_value )
					printf("success                ");
				else
					printf("written %d, but read %d", wr_value, rd_value );
				printf("\t");
			}
			printf("\n");
		}
	};
	test( io_base, peri::out);
	puts("------------------------------");
	test( io_base, peri::in);

	// cleanup
	peri::unmap_io( io_base ); // made it explicit for sake of making it clear
	                             // this is an invalid pointer now.
	return 0;
}

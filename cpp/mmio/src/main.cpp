#include <stdio.h>
#include "mmio.h"
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include "common.h"

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

void test_function( uint32_t* const io_base, const int function ) {
for( uint_fast8_t pin = 0; pin < 28; pin++ ) { // all pins
	for( uint_fast8_t value = 0; value <= 1; value++) { // both values
			bool wr_value = value;
			printf("%2u=%u\t", pin, value );
			peripheral::gpio_fs( io_base, pin, function );
			peripheral::gpio_write( io_base, pin, wr_value );
			msleep(10);
			bool rd_value = peripheral::gpio_read( io_base, pin );
			if ( wr_value == rd_value )
				printf("success                ");
			else
				printf("written %d, but read %d", wr_value, rd_value );
			printf("\t");
		}
		printf("\n");
	}
};

void test_pin( uint32_t* const io_base, const int pin ) {
	for( uint_fast8_t value = 0; value <= 1; value++) { // both values
		bool wr_value = value;
		printf("%2u=%u\t", pin, value );
		peripheral::gpio_fs( io_base, pin, peripheral::out );
		peripheral::gpio_write( io_base, pin, wr_value );
		msleep(10);
		bool rd_value = peripheral::gpio_read( io_base, pin );
		if ( wr_value == rd_value )
			printf("success                ");
		else
			printf("written %d, but read %d", wr_value, rd_value );
		printf("\t");
	}
	printf("\n");
};


int main() {
	namespace peri=peripheral;
	namespace addr=peripheral::address;
	// get access to memory-mapped gpio gregion
	uint32_t* const io_base = peripheral::map_io();
	int ret = check_ptr( io_base );
	if( ret )
		return ret;
	//test_pin( io_base, 10);
	//puts("------------------------------");
	//test_function( io_base, peripheral::out);
	//puts("------------------------------");
	//test_function( io_base, peripheral::in);
	const clock_t begin = clock();
	const auto iterations = 10'000'000;
	volatile uint32_t* const lev_addr = addr::get_gpio_read_address( io_base );
	#pragma unroll
	for( int i = 0; i < iterations; ++i ){
		if( *lev_addr == 0 )
			[[unlikely]] break;
		//printf("%x\n", *lev_addr );
	}
	const clock_t end = clock();
	const double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("%fM reads/s\n", (iterations/time_spent)/1'000'000);
	// cleanup
	peripheral::unmap_io( io_base ); // made it explicit for sake of making it
	                                 // clear this is an invalid pointer now.
	return 0;
}

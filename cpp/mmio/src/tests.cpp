#include "tests.h"

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

//test_pin( io_base, 10);
//puts("------------------------------");
//test_function( io_base, peripheral::out);
//puts("------------------------------");
//test_function( io_base, peripheral::in);

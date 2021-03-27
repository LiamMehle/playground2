#include <stdio.h>
#include "mmio.h"
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include "common.h"
#include "tests.h"
#include <vector>
#include <algorithm>
#include <assert.h>

int main() {
	namespace peri=peripheral;
	namespace addr=peripheral::address;

	const auto iterations = 10'000'000;
	std::vector<uint32_t> gpio_value(iterations);
	std::vector<long>  gpio_time(iterations);
	timespec time;

	{
		timespec resolution;
		clock_getres( CLOCK_MONOTONIC_RAW, &resolution );
		assert( resolution.tv_sec == 0 );
		printf("timer resolution is %ld ns\n", resolution.tv_nsec );
	}

	// get access to memory-mapped gpio gregion
	uint32_t* const io_base = peripheral::map_io();
	int ret = check_ptr( io_base );
	volatile uint32_t* const lev_addr = addr::get_gpio_read_address( io_base );
	if( ret )
		return ret;

	const clock_t begin = clock();
	#pragma unroll 4
	for( unsigned int i = 0; i < iterations; i++ ) {
		// prefetch one cycle ahead. Experiments show 2 cycles ahead is too far
		__builtin_prefetch( (void*)&gpio_value[i+1], 1, 0 ); // write, one-off
		__builtin_prefetch( (void*)&gpio_time[i+1],  1, 0 );
		gpio_value[i] = *lev_addr;
		clock_gettime( CLOCK_MONOTONIC_RAW, &time );
		gpio_time[i]  = time.tv_nsec;
	}
	peripheral::unmap_io( io_base ); // made it explicit for sake of making it
	                                 // clear this is an invalid pointer now.
	const clock_t end = clock();
	const double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("%fM reads/s\n", (iterations/time_spent) / 1'000'000);
	// cleanup
	return 0;
}

#include "common.h"
int _check_ptr( const void* const ptr, const char* const file, const int line )
                                                                      noexcept {
	if( ptr == (void*)0 || ptr == (void*)-1 ) {
		printf( "[ERROR] errno=%d in %s on line %d\n", errno, file, line );
			return -1;
	}
	return 0;
}

void msleep( uint_fast16_t msec ) {
	const timespec req = {
		.tv_sec = static_cast<__time_t>(msec/1000),
		.tv_nsec = static_cast<__syscall_slong_t>((msec%1000)*1000*1000 )
	};
	timespec rem {};
	int ret = nanosleep( &req, &rem );
	while( ret != 0 ) { // if interrupted
		ret = nanosleep( &rem, &rem );
	}
}

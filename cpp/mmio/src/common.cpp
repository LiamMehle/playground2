#include "common.h"
int _check_ptr( const void* const ptr, const char* const file, const int line )
                                                                      noexcept {
	if( ptr == (void*)0 || ptr == (void*)-1 ) {
		printf( "[ERROR] errno=%d in %s on line %d\n", errno, file, line );
			return -1;
	}
	return 0;
}

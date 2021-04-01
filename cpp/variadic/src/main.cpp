#include <iostream>
#include <stdarg.h>

void vari_print( const char* fmt, ... ) {
	va_list args;
	va_start( args, fmt );

	while( *fmt != '\0' ) {

		switch( *fmt ) {
			case 'c': std::cout << va_arg( args, char   );
			case 'f': std::cout << va_arg( args, double );
			case 'd': std::cout << va_arg( args, int    );
		}
		++fmt;
		std::cout << '\n';
	}
	va_end(args);
}

int main() {
	vari_print( "dddddddd", 2, 6, 4, 6, 2, 3, 4, 1982371923 );
	return 0;
}

#include "check_err.hpp"
#include <iostream>

void check_err( int err, const char* const  message ) {
	if( err == 0 )
		return;

	std::cout << "[ERROR]: <" << err << "> " << message << '\n';
	std::exit(err);
}
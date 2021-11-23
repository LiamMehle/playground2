//#include <stdio.h>
#include "set_level.h"
#include "get_temp.hpp"
#include <iostream>

extern "C" {
	int set_level(char*);
}

int main() {
	for(int i = 0; i < 1000; i++) {
		get_temp();
	}
	//set_level( argv[1] );
}
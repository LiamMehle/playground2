#pragma once
#include <string>

struct Period {
	int day:4,    // day of the week
	    start:12, // start minute
	    end:12;   // end minute
	std::string name;
};
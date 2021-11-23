#include "types.h"
#include <iostream>
#include <vector>

void debug_print(std::vector<Period> const& periods) {
#define print(x) std::cout << #x << " = " << x << '\n';
	for(auto const& period : periods) {
        std::cout << "---------------\n";
        print(period.name)
        print(period.day)
        print(period.start / 60)
        print(period.start % 60)
        print(period.end / 60)
        print(period.end % 60)
	}
#undef print
}

void debug_print(char const* msg) {
    std::cout << msg << '\n';
}

void debug_print(std::string_view msg) {
    std::cout << msg << '\n';
}
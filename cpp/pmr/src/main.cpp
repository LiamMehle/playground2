#include <vector>
#include <iostream>
#include <memory_resource>

int main() {
	std::array<char, (1<<10)*32> mem;
	std::pmr::monotonic_buffer_resource rsrc{mem.data(), mem.size()};
	std::pmr::vector<int> v(&rsrc);
	v.push_back(2);
	std::cout << v.at(0) << '\n';
	return 0;
}
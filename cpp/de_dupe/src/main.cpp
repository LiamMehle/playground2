#include <algorithm>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <vector>
#include <array>
#include <future>
#include <thread>
#include <array>

#define MAX_NAME_LEN 256

namespace fs = std::filesystem;

std::vector<std::array<char, MAX_NAME_LEN>> enumerate_directory(std::filesystem::path p) {
	std::vector<std::array<char, MAX_NAME_LEN>> ret;
	const fs::directory_iterator dir_iter(p);
	//todo: add support for depth > 0
	for(auto& dir : dir_iter) { // iterator -> const fs::directory_entry&
		if(dir.is_regular_file()) {
			const auto name = dir.path().filename().string();
			size_t name_end = name.find_last_of("."); 
			const auto raw_name = name.substr(0, name_end); 
			ret.emplace_back();
			std::memcpy((void*)&ret.back()[0], &raw_name.front(), MAX_NAME_LEN);
		}
	}
	ret.shrink_to_fit();
	return ret;
}

int _main(const int argc, const char* const * const argv) {
	std::string src;
	std::string dst;
	const char source_flag[]      = "-s";
	const char destination_flag[] = "-d";

	// check args
	if(argc < 1+2) {
		std::cout << "usage: " <<  argv[0] << " -s src -d dst\n";
		return -1;
	}

	// set args
	for(int i = 0; i < argc-1; i++) {
		if(strcmp(argv[i], source_flag) == 0)
			src = argv[++i];
		if(strcmp(argv[i], destination_flag) == 0)
			dst = argv[++i];
	}

	//auto src_future = std::async(std::launch::deferred, enumerate_directory, src);
	auto dst_future = std::async(std::launch::async, enumerate_directory, dst);
	//const std::vector<std::string> src_files = src_future.get();
	const auto src_files = enumerate_directory(src);
	const auto dst_files = dst_future.get();

	std::vector<std::string_view> dupes;

	for(auto& src_name : src_files) {
		__builtin_prefetch(&src_name+1);
		const auto len = std::strlen(&src_name[0]);
		const auto matches = [src_name, len](const std::array<char, MAX_NAME_LEN>& compare_with) -> bool {
			return std::memcmp(&src_name[0], &compare_with[0], len) == 0;
		};
		bool match = std::any_of(std::begin(dst_files), std::end(dst_files), matches);
		if(match) {
			dupes.emplace_back(static_cast<const char*>(&src_name[0]), len);
		}

	}

	const auto print_string_view = [](std::string_view s) {
		std::cout << s << '\n';
	};
	std::for_each(std::begin(dupes), std::end(dupes), print_string_view);
	return 0;
}

int main(const int argc, const char* const * const argv) {
	//for(int i = 0; i < 1000000; i++)
		_main(argc, argv);
	return 0;
}
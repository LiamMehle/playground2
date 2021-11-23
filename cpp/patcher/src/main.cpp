#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>

std::string hex_to_str(std::string&& hex) {
	std::string ret;
	for(std::size_t i=0; i<hex.length(); i+=2) {
		const std::string byte = hex.substr(i,2);
		const char chr = static_cast<char>(strtol(byte.c_str(), nullptr, 16));
		ret.push_back(chr);
	}
	return ret;
}

int main(const int argc, const char* const* const argv) {
	// name file target result
	[[unlikely]]
	if(argc < 4) {
		std::cout << argv[0] << ": too few arguments\n";
		return 1;
	}

	std::fstream bin;
	bin.open(argv[1], std::ios::in | std::ios::out | std::ios::binary);
	std::string data = std::string(std::istreambuf_iterator<char>(bin),
					               std::istreambuf_iterator<char>());


	
	const std::string target = hex_to_str(argv[2]);
	const size_t index = data.find(target);
	bin.seekp(index);
	const std::string result = hex_to_str(argv[3]);
	bin << result;

	// destructors
	
	return 0;
}

/*
#include <atomic>
#include <thread>
#include <vector>

constexpr auto num_threads = 1<<10;

std::atomic<int> mailbox_flag[num_threads];
std::atomic<bool> terminate;
int mailbox[num_threads];

void worker(std::size_t id) {
	while(!terminate.load(std::memory_order_relaxed)) {
		[[maybe_unused]] volatile int Void = 0;
		while(mailbox_flag[id].load(std::memory_order_consume) != 1 && !terminate.load(std::memory_order_relaxed));
		Void = mailbox[id];
		mailbox[id] = 2;
		mailbox_flag[id].store(0, std::memory_order_release);
	}
}

int main() {
	terminate.store(0, std::memory_order_relaxed);
	std::vector<std::thread> thr;
	for(int i = 0; i < num_threads; i++) {
		mailbox[i] = 5;
		mailbox_flag[i].store(1, std::memory_order_release);
		thr.emplace_back(worker, i);
	}

	for(int i = 0; i < num_threads*10; i++) {
		const int id = i%num_threads;
		while(mailbox_flag[id].load(std::memory_order_consume) != 0);
		mailbox[id] = 5;
		mailbox_flag[id].store(1, std::memory_order_release);
	}

	terminate.store(1, std::memory_order_relaxed);

	for(int i = 0; i < num_threads; i++)
		thr[i].join();
	return 0;
}
*/
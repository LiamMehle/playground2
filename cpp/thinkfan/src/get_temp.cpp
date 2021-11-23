#include <filesystem>
#include <string>
#include <stdio.h>

//#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <mutex>

namespace fs = std::filesystem;

static int read_temp( const fs::path& p ) {
	int ret;
	FILE* const file = fopen(p.c_str(), "r");
	if(file == NULL)
		return -1;
	fscanf(file, "%i", &ret);
	fclose(file);
	return ret;
}

static int process_directory(fs::directory_entry dir) {

	const fs::path p = dir.path();
	// cheap heuristic
	if(!is_directory(p))
		return -1;

	// expensive check
	if(p.filename().string().find("thermal_zone") == std::string::npos)
		return -1;

	// valid path
	const fs::path new_path = p.string() + "/temp";
	return read_temp(new_path);
}

std::vector<fs::directory_entry> dirs;
std::mutex dirs_mutex;
std::atomic<int> max_temp = 0;

void worker() {
	while(true) {
		fs::directory_entry dir;
		{
			std::lock_guard<std::mutex>m(dirs_mutex);
			if(dirs.empty())
				return;
			dir = dirs.back();
			dirs.pop_back();
		}

		int temp_read = process_directory(dir); // do work

		// return new value
		int temp_current = 0;
		bool success;
		do {
			//std::cout << temp_current << " <- " << temp_read << '\n';
			success = max_temp.compare_exchange_weak(temp_current, temp_read, std::memory_order_release ,std::memory_order_acquire);
			if(temp_current > temp_read)
				break;
		} while(!success);
	}
}

int get_temp() {

	fs::directory_iterator base_path = fs::directory_iterator("/sys/class/thermal/");
	//auto dir_iterator = fs::directory_iterator(base_path);
	dirs = std::vector<fs::directory_entry>(fs::begin(base_path), fs::end(base_path));

	int thread_count = std::thread::hardware_concurrency();
	thread_count = thread_count == 0 ? 16 : thread_count;
	std::vector<std::thread> thread_pool(thread_count);

	for(int i = 0; i < thread_count && dirs.size() > 10; i++)
		thread_pool.push_back(std::thread(worker));

	for(auto& t : thread_pool) {
		if(t.joinable())
			t.join();
	}

	return max_temp.load(std::memory_order_seq_cst);
}
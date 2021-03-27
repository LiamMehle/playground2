#include <iostream>

#include <vector>
#include <atomic>

#include <string>
#include <cstring>
#include <string_view>

#include <dirent.h>
#include <sys/types.h>

#include <cstdlib>

#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>

#define UID1(a, b) a##b              // concatenate
#define UID2(a, b) UID1(a, b)        // evaluate
#define UID UID2(___id, __COUNTER__) // add unique value to the end

#define PATH_MAX 256

struct file_data {
	std::string path;
	std::string_view name;
	unsigned int inode;
};


class enum_dir {
private:
	// an invalid path to signal termination
	constexpr std::string break_path = "/..";

	std::queue<std::string> paths;    // holds paths of directories to enumerate
	std::mutex paths_mutex;           // path protection
	std::mutex paths_cv_mutex;        // cond var mutex
	std::condition_variable paths_cv; // cond var for threads waiting to recieve a new

	std::set<file_data> data;      // holds information of files
	std::mutex data_mutex;

	std::atomic<char> active_threads;

	std::vector<std::thread> workers;

	void thread_safe_push_path( const std::string&& path ) {
		{
			std::lock_guard<std::mutex> UID( paths_mutex );
			paths.push(std::move( path ));
		}
		paths_cv.notify_one();
	}

	std::string thread_safe_pop_path() {
		std::string ret;

		std::lock_guard<std::mutex> UID( paths_mutex );
		ret(std::move(paths.pop()));

		return std::move(ret);
	}

	void worker() {
		do {
			// get path for this iteration ( and handle termination condition )
			const std::string path = [&](){
				std::string path;
				paths_mutex.lock();                // this lock is manipulated specially

				while( paths.size() != 0 ) {           // wait until there's a path to eval
					paths_mutex.unlock();//paths queue can't be updated while lock is held
					std::unique_lock<std::mutex> paths_cv_lock( paths_cv_mutex );
					paths_cv.wait( paths_cv_lock, [](){paths.size() != 0;} );
					paths_mutex.lock();               // re-lock to prevent race condition
				} /* while loop re-checks condition after lock*/
				                                    // ...and to restore state to as if...
				                                    // ...the 'if' was never taken
				path = paths.pop();                 // get return value

				if( path == break_path )            // if path signlas termination...
					paths.push( break_path.c_str() ); // push a copy while the lock is held
				paths_mutex.unlock();               // finally, unlock
				if( path == break_path ) // guarantee all threads are signaled
					paths_cv.notify_all();

				return path;                        // path is returned regardless...
			}();                     // ..termination checking is done outside as well

			if( path == break_path )               // handle termination
				return;

			// valid path recovered, onto processing it
			{ // this section (anon scope) is when the thread is considered "active"
				inc_dec UID( active_threads ); // automatically manage number of...
				                               // ...active threads

				// operate on the path

				{ // save new data
					std::lock_guard<std::mutex> UID(data_mutex);
					data.instert( data.begin(), new_data.begin(), new_data.end() );
				}
			}

		} while( path != break_path );
	}

	void create_thread_pool() {
		int max_threads = std::thread::hardware_concurrency();
		if( max_threads <= 0 )
			max_threads = 12;
		workers.reserve(max_threads);
		for( int i = 0; i < max_threads; i++ )
			workers.emplace_back( worker );
	}

public:
	enum_dir( std::string entry_path ) {
		create_thread_pool();
		char temp[PATH_MAX];
		realpath( entry_path, temp );
		entry_path = temp;
		// thread_safe_push_path(std::move( entry_path )); // commented for test

		//////////////////////////////////////////////////////////////////////
		/*////*/thread_safe_push_path(std::move( break_path.c_str() )); // TEMPORARY STUB
		//////////////////////////////////////////////////////////////////////
		paths_cv.notify_all();
	}

	void kill_threads() {
		static already_run = 0;
		if( !already_run )
			for( auto& worker_thread : workers )
				if( worker_thread.joinable() )
					worker_thread.join();
		already_run = 1;
	}

	[[no_discard]]
	std::set<file_data> get() {
		kill_threads();
		return std::move( data );
	}

	~enum_dir() {
		kill_threads();
	}
};


int main( int argc, char** argv ) {

	if( argc != 2 )
		print_usage();

	const std::string entry_path = [=](){
		std::string temp( 256, '\0');
		realpath(argv[1], temp.data() );
		temp.shrink_to_fit();
		return temp;
	}();

	// enumerate directory here

	/*for( auto& file : files ) {
		std::cout << "path: " << file.path << "\tino: " << file.inode << '\n';
	}*/
	return 0;
}

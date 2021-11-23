#include <iostream>
#include <vector>
#include <numeric>
#include "queue.h"
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>


class thread_pool {
private:
	std::atomic<bool> terminate;
	std::mutex cv_mtx;
	std::condition_variable cv;
	std::atomic<short> thread_count;
	std::atomic<short> threads_active;

	std::vector<std::thread> thrs;
	std::vector<std::atomic<std::function>> mailbox;
	std::vector<std::atomic<char>> mailbox_owner; // 0 = delegator
	                                                       // 1 = worker

	// todo: give each worker private cv
	// todo: finish definition
	void worker(&std::atomic<std::function> maillbox, &std::atomic<char> mailbox_owner) {
		std::unique_lock<std::mutex> lk(this.cv_mtx);
		cv.wait(lk, [&](){return 1 == mailbox_owner.load(std::memory_order_acquire)});
		
		if(terminate.load(std::memory_order_relaxed))
			return;

	}

public:
	thread_pool(short count) {
		thrs.reserve(count);
		mailbox.reserve(count);

		threads_active.store(0, std::memory_order_relaxed);
		terminate.store(0, std::memory_order_relaxed);
		thread_count.store(count, std::memory_order_relaxed);
		for(int i = 0; i < count; i++) {
			mailbox.push_back();
			mailbox_owner.push_back();
			mailbox[i].store(0, std::memory_order_relaxed);
			thrs.emplace_back(this.worker, &mailbox[i], &mailbox_owner[i]);
		}
	}
};

float avg( const std::vector<float> vec ) {
	return (float)std::accumulate(vec.begin(), vec.end(), 0.0) / (float)vec.size();
}

int main() {
	[[maybe_unused]]
	constexpr unsigned ops_per_thread = 6;

	const std::vector<float> T_k = {
		33.0,
		33.8,
		33.7,
		33.5,
		34.5,
		34.6,
		33.7,
		33.9,
		34.4
	};

	const std::vector<float> T_l = {
		30.0,
		30.7,
		30.6,
		30.1,
		30.9,
		31.2,
		29.6,
		30.1,
		30.4,
		31.0
	};


	[[maybe_unused]] const float T_k_avg = avg( T_k );
	[[maybe_unused]] const float T_l_avg = avg( T_l );


	return 0;
}
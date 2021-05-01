#pragma once
#include <atomic>
#include <vector>

class queue {
private:
	unsigned length;
	std::atomic<unsigned> head = 0, tail = 0;
	std::vector<const void*> array;

public:
	queue() = delete;
	queue( unsigned len );
	bool empty();
	bool full();
	bool push( const void* value );
};
#include "queue.h"
#include <math.h>
// length is always power of 2 at least len in size
queue::queue( unsigned len ) {
	length = (int)pow(2, ceil(log(len)/log(2)));
	array.reserve( length );
}
bool queue::empty() {
	const unsigned int h = head.load(std::memory_order_acquire);
	const unsigned int t = tail.load(std::memory_order_acquire);
	return h == t;
}
bool queue::full() {
	const unsigned int h = head.load(std::memory_order_acquire);
	const unsigned int t = tail.load(std::memory_order_acquire);
	return h == ((t + 1) % length);
}

bool queue::push( const void* value ) {
	      unsigned int h = head.fetch_add(1, std::memory_order_acquire);
	const unsigned int t = head.load(std::memory_order_acquire);
	head.fetch_and(length-1, std::memory_order_release); // %= length;
	if( h == t )
		return false;
	array[h] = value;
	return true;
}

// borked, fix
/*
bool queue::push( const void* value ) {
	const unsigned int h = head.fetch_add(1, std::memory_order_seq_cst);
	const unsigned int t = tail.load(std::memory_order_consume);
	if( h+1 == t ) {

		return false;
	}
	head.fetch_and(length-1, std::memory_order_release); // %= length;
	array[h] = value;
	return true;
}

void* queue::pop() {
	const unsigned int t = tail.fetch_add(1, std::memory_order_acquire);
	const unsigned int h = head.load(std::memory_order_consume);
	tail.fetch_and(length-1, std::memory_order_release); // %= length;
	if( h == t )
		return nullptr;
	array[h] = value;
	return true;
}
*/
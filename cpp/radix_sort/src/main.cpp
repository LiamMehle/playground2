#include <iostream>
#include <vector>
#include <cmath>
#include <cstring>
#include <stdlib.h>
#include <assert.h>

void print_vec( std::vector<unsigned char> vec ) {
	std::cout << "[ ";
	for( auto& c : vec )
		std::cout << (int)c << ' ';
	std::cout << "]\n";
}

std::vector<unsigned char> radix_sort( std::vector<unsigned char>& array, const unsigned initial_radix ) {
	std::vector<unsigned char> sorted_array( array.size() );

 	const int iteration_count = (unsigned)ceil(log2((double)array.size())/log2(initial_radix))+1;

	unsigned radix = initial_radix;

	for( int i = 0; i < iteration_count; i++ ) {
		std::vector<long long> count( initial_radix );         // re-init counts		
		std::fill(count.begin(), count.end(), 0);

		const unsigned radix_div = radix/initial_radix;
		const unsigned radix_common = radix/radix_div;
		count[0] = -1; // convert prefix sum to 0-indexing
		for( auto& c : array )
			count[ c% (radix_common) ]++;
	
		for( int i = 1; i < (int)initial_radix; i++ ){ // generate prefix sum
			count[i] += count[i-1];
		}

		for( int i = 1; i < (int)initial_radix; i++ )
			assert(count[i] >= count[i-1]);


		for( int i = (int)array.size()-1; i >= 0; i-- ) {
			const unsigned m_radix = array[i] % (radix_common);   // compute radix
			const auto start_index = count[m_radix]; // get start index
			auto index = start_index;

			// this is the main bottle neck
			// alleviating this would 3x perf
			//while( sorted_array[index] != 0 )         // find first free slot
			//	index--;
			sorted_array[index] = array[i];              // insert into free slot
			count[m_radix]--; // update count to prevent scowering for free slot
		}

		array = sorted_array;
		std::fill(sorted_array.begin(), sorted_array.end(), 0);
		radix *= initial_radix;                          // shift to the next 'digit'
	}
	return array;
}


int main() {
	const std::string s = "this is random text";
	[[maybe_unused]]constexpr int kb = (1<<10);
	[[maybe_unused]]constexpr int mb = kb*kb;
	constexpr int len = 8;
	std::vector<unsigned char> array(len);
	srand(0);
	for( auto& element : array )
		element = (unsigned char)rand();

	//print_vec( array );
	radix_sort( array, 8 );
	print_vec( array );
	for( int i = 1; i < len; i++ )
		assert(array[i] >= array[i-1]);

}


#include <iostream>
#include <vector>
#include <cmath>
#include <cstring>

void print_vec( std::vector<char> vec ) {
	std::cout << "[ ";
	for( auto& c : vec )
		std::cout << (int)c << ' ';
	std::cout << "]\n";
}

std::vector<char> radix_sort( std::vector<char>& array, const unsigned initial_radix ) {
	std::vector<char> sorted_array( array.size() );

 	const int iteration_count = (unsigned)ceil(log2((double)array.size())/log2(initial_radix))+1;

	unsigned radix = initial_radix;

	for( int i = 0; i < iteration_count; i++ ) {
		std::vector<char> count( initial_radix );         // re-init counts
		memset( count.data(), 0, initial_radix );

		count[0] = -1; // convert prefix sum to 0-indexing
		for( auto& c : array )
			count[ ( c%radix )/( radix/initial_radix ) ]++;
	
		for( int i = 1; i < (int)initial_radix; i++ ){ // generate prefix sum
			count[i] += count[i-1];
		}
	
		for( int i = (int)array.size()-1; i >= 0; i-- ) {
			const unsigned m_radix = (array[i] % radix)/(radix/initial_radix);   // compute radix
			const unsigned start_index = count[m_radix]; // get start index
			unsigned index = start_index;

			while( sorted_array.at(index) != 0 )         // find first free slot
				index--;
			sorted_array[index] = array[i];              // insert into free slot
		}
		array = sorted_array;
		std::fill(sorted_array.begin(), sorted_array.end(), 0);
		radix *= initial_radix;                          // shift to the next 'digit'
	}
	return array;
}


int main() {
	const std::string s = "this is random text";
	std::vector<char> array(s.begin(), s.end());

	print_vec( array );
	radix_sort( array, 10 );
	print_vec( array );

}


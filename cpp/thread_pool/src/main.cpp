#include <iostream>
#include <vector>
#include <numeric>
#include "queue.h"

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
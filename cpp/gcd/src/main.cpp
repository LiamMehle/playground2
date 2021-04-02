#include <stdio.h>
#include <stdint.h>
#include <assert.h>


uint64_t gcd( uint64_t a, uint64_t b ) {
	//const uint64_t min = std::min(a,b);
	//const uint64_t max = std::max(a,b);
	//return gcd( min, max % min );
	
	if( a == 1 || b == 1 )
		return 1;
	if( a == 0 )
		return b;
	if( b == 0 )
		return a;

	while(true) {
		if( a > b )
			a %= b;
		if( a < b )
			b %= a;

		if( a == 1 || b == 1 )
			return 1;
		if( a == 0 )
			return b;
		if( b == 0 )
			return a;
	}
}

int main() {
	puts("Testing...");
	assert(gcd(1, 2) == 1);
	assert(gcd(2, 1) == 1);
	assert(gcd(0, 2) == 2);
	assert(gcd(1, 0) == 1);
	assert(gcd(0, 0) == 0);
	assert(gcd(5,25) == 5);
	assert(gcd(918273, 198217392) == 3);
	puts("Success!");

	return 0;
}
#include <cstdio>
#include <gmp.h>

static int mpz_print( const char* const s, const mpz_t z, char* memory ) {
	mpz_get_str( memory, 10 ,z );
	return printf("%s%s\n", s, memory);
}

int main() {
	mpz_t z[3];
	for( auto& x : z )
		mpz_init(x);

	mpz_set_str(z[0], "2387401950981734098321706523084698321074821", 10);
	mpz_set_str(z[1], "3984701982743091875098274307509324875029333", 10);
	mpz_add(z[2], z[0], z[1]);

	char buf[1<<10];
	mpz_print("z[0] = ", z[0], buf );
	mpz_print("z[1] = ", z[1], buf );
	mpz_print("z[2] = ", z[2], buf );

	for( auto& x : z )
		mpz_clear(x);
	return 0;
}
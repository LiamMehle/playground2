#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/*define DEBUG */

void debug_print( __attribute__((unused)) const char* const var_name,
                  __attribute__((unused)) const double var ) {
#ifdef DEBUG
	printf("%s=%f\n", var_name, var );
#endif
}


void print_usage( const char* const exec_name ) {
	printf("usage:\n%s znesek\n", exec_name );
}

double round( double x ) {
	int whole = (int) x;
	x -= whole;
	if( x < .5 )
		x = 0;
	else
		x = 1;
	return x + whole;
}

void divide_and_print( uint_fast16_t* const znesek, const uint_fast16_t increment ) {
	if( increment >= 100 ) {
		printf("%dx %d€\n", *znesek/increment, increment/100 );
		*znesek %= increment;
		debug_print("*znesek", *znesek );
		return;
	} else {
		printf("%dx %dc\n", *znesek/increment, increment );
		*znesek %= increment;
		debug_print("*znesek", *znesek );
		return;
	}
}

int main( int argc, char** argv ) {
	uint_fast16_t znesek;         /* v centih */
	const uint_fast16_t vrednosti[] = { /* v centih */
		100*500, /* zmnoženo z 100 za pretvorbo v cente */
		100*200,
		100*100,
		100*50,
		100*20,
		100*10,
		100*5,
		/* 100*2,
		   100*1, */
		50,
		20,
		10,
		5
	};

	if( argc != 2 ) {
		print_usage( argv[0] );
		return 1;
	}
	{
		double temp;
		sscanf( argv[1], "%lf", &temp );
		/* pretvorim v cente */
		znesek = (uint_fast16_t) round(temp*100);
	}
	{
		uint_fast8_t i;               /* for loop */
		for( i = 0; i < sizeof(vrednosti) / sizeof(vrednosti[0]); i++ )
			divide_and_print( &znesek, vrednosti[i] );
	}
	printf("preostalo je %.2f€\n", ((float)znesek)/100 );

	return 0;
}

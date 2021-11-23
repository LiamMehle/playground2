#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define STDOUT 1

void usage( const char* const name ) {
	printf( "usage: %s voltage [cell_count]\n", name );
}

uint_fast8_t compute_cell_count( const double voltage, double* const voltage_per_cell_p ) {
	uint_fast8_t cell_count = 0;
	double voltage_per_cell;
	do {
		cell_count++;
		voltage_per_cell = voltage/cell_count;
	} while( voltage_per_cell > 4.6 );
	if( voltage_per_cell_p != NULL )
		*voltage_per_cell_p = voltage_per_cell;
	return cell_count;
}
//  main(       int argc,       char**              argv )
int main( const int argc, const char* const * const argv ) {
	double voltage, voltage_per_cell;
	uint_fast8_t cell_count;
	switch( argc ) {
		case 0: // corruption, exit immediately
			return -1;

		case 2:
			voltage = atof( argv[1] );
			cell_count = compute_cell_count( voltage, &voltage_per_cell );
			printf("cell_count=%u\t voltage_per_cell=%.2f\n", cell_count, voltage_per_cell );
			return 0;

		default:
			usage( argv[0] );
			return 1;
		}
}

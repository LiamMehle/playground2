#include <stdio.h>
#include <string.h>

const char border_c = '#';
const int width     = 30;
const int height    = 10;

void padding_line() {
	putchar( border_c );
	for( int i = 0; i < width-2; i++ ) {
		putchar( ' ' );
	}
	putchar( border_c );
	putchar( '\n' );
}


void print_line( const char* const string ) {


	const int str_len = (int) strlen( string );
	const int offset = ( width - str_len )/2 - 1;

	putchar( border_c );
	for( int i = 0; i < offset; i++ ) {
		putchar( ' ' );
	}
	printf( "%s", string );

	for( int i = offset + str_len+1; i < width-1; i++ ) {
		putchar( ' ' );
	}

	putchar( border_c );
	putchar( '\n' );
}


void horizontal_border() {
	for( int i = 0; i < width; i++ ) {
		putchar( border_c );
	}
	putchar( '\n' );
}


int main() {

	// top border
	horizontal_border();
	padding_line();
	print_line( "Liam Mehle" );
	padding_line();
	print_line( "Partizanska 1" );
	print_line( "Izola, 6310" );
	padding_line();
	print_line( "+386 41 123 456" );
	padding_line();

	// bottom border
	horizontal_border();

	return 0;
}

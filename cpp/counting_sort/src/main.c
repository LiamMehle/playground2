#include <stdio.h>
#include <assert.h>
#include <string.h>

#define COUNT_LEN 1<<8

int main() {
	char array[1<<10]; // bi moglo biti dovolj spomina

	puts("Vnesite niz:");
	scanf("%[^\n]", array );

	const int array_len = (int)strlen(array);

	const int count_len = COUNT_LEN;
	int count[COUNT_LEN] = {0};
	// counting sort

	for( int i = 0; i < array_len; i++ )
		count[ (int)array[i] ]++;


	puts("Razvrščen niz je:");
	for( int i = 0; i < count_len; i++ )
		for( int j = 0; j < count[i]; j++ )
			putchar( (char)i );

	putchar('\n');

}
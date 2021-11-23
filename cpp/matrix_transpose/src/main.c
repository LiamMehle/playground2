#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

// bi lahko dodal at() v struct
typedef struct {
	int* addr;
	unsigned height;
	unsigned width;
} Matrix;

// vrne addreso vrendosti na lokaciji x,y
static int* at( const Matrix* m,
                     const unsigned x,
                     const unsigned y ) {
	return m->addr + x + y * m->width;
}

static void transpose( Matrix* m ) {
	// naredim transponirano matriko
	Matrix t = {
		.addr = (int*) malloc( sizeof(int) * m->width * m->height ),
		.width = m->height,
		.height = m->width
	};

	for( unsigned y = 0; y < m->height; y++ )
		for( unsigned x = 0; x < m->width; x++ )
			*at( &t, y, x ) = *at( m, x, y );

	free(m->addr); // sprosti spomin originala
	*m = t;        // zamenjaj original z transponirano matriko
}

static void print_matrix( Matrix* m ) {
	for( unsigned y = 0; y < m->height; y++ ) {
		printf("[ ");

		for( unsigned x = 0; x < m->width; x++ )
			printf("%3d ", *at(m, x, y));

		puts("]");
	}
}


int main() {
	Matrix m;
	puts("Vpišite število vrstic:");
	scanf("%u", &m.height);
	puts("Vpišite število stolpcev:");
	scanf("%u", &m.width);

	const size_t size = m.width * m.height;
	m.addr = (int*) calloc(size, sizeof(int));


	for( unsigned y = 0; y < m.height; y++ ) {
		for( unsigned x = 0; x < m.width; x++ ) {
			print_matrix(&m);
			printf("matrika[%u,%u]:\n", x, y);
			scanf("%d", at( &m, y, x ));
		}
	}

	puts("");
	print_matrix( &m );
	transpose( &m );
	puts("");
	print_matrix( &m );

	return 0;
}
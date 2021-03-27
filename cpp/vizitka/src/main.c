#include <stdio.h>

#define ADVANCED

#ifndef ADVANCED

int main() {
	puts(
		"##############################\n"
		"#                            #\n"
		"#         Liam Mehle         #\n"
		"#                            #\n"
		"#        Partizanska 1       #\n"
		"#         Izola, 6310        #\n"
		"#                            #\n"
		"#       +386 41 123 456      #\n"
		"#                            #\n"
		"##############################"
	);
}
#endif
#ifdef ADVANCED

#include <malloc.h>
#include <string.h>
#include <unistd.h>

#define STDOUT 1
#define WIDTH (30+1)
#define HEIGHT 10
#define TEXT_SIZE (WIDTH * HEIGHT)
#define FILL_CHAR '#'



void draw_line( char* const dst, const int offset, const int width ) {
	int i;
	/* minus 1 zaradi končnega \n */
	for( i = offset; i < width+offset-1; i++ ) {
		dst[i] = FILL_CHAR;
	}
}

void draw_col( char* const dst, const int offset, const int width, const int height ) {
	int i;
	for( i = offset; i < width*height+offset; i += width ) {
		dst[i] = FILL_CHAR;
	}
}

void setup_control_chars( char* const dst, const int width, const int height ) {
	/* -1 ker zadnja vrstica se konča z \0 */
	int line_start;
	int line_index;
	for( line_index = 0; line_index < height; line_index++ ) {
		line_start = line_index*width;            /* izračun za začetek vrstice */
		dst[line_start + (width-1)] = '\n';       /* '\n' na konec vrstice      */
	}
	/* niz se konča z null */
}

void draw_text( char* const dst, const char* const text,
                const int line,
                const int width ) {
	const int str_len    = (int) strlen( text );
	const int line_start = line*width;
	const int text_start = (width-str_len)/2;
	strncpy( &dst[line_start+text_start], text, (unsigned int) str_len );
}

void draw_borders( char* const dst, const int width, const int height ) {
	draw_line( dst, 0, width );
	draw_line( dst, (height-1)*width, width );
	draw_col(  dst, 0, width, height );
	draw_col(  dst, width-2, width, height );
}

int main() {
	/* vrstice so razporejene drug za drugo v spominu */
	/*char* const message = (char*) malloc( TEXT_SIZE );*/
	char        memory [TEXT_SIZE];
	char* const message = memory;
	/*'izpraznim' tekst vizitke*/
	memset( (void*)message, (int)' ', TEXT_SIZE );
	/* vmestim \n in \0 na pravih mestih */
	setup_control_chars( message, WIDTH, HEIGHT );
	draw_borders( message, WIDTH, HEIGHT );
	/* <text> */
	draw_text( message, "Liam Mehle", 2, WIDTH );
	draw_text( message, "Partizanska 1", 4, WIDTH );
	draw_text( message, "Izola, 6310", 5, WIDTH );
	draw_text( message, "+386 41 123 456", 7, WIDTH );
	/* </text> */
	/* print   */
	write( STDOUT, message, TEXT_SIZE );

	/* nepotrebno, saj operacijski sistem bi mogel pridobiti spomin ko ta */
	/* program zaključi izvrševanje                                       */
	/*free( message );*/
	return 0;
}

#endif

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#define GRADE_COUNT 10
#define NAME_LEN    64
typedef struct {
	uint32_t id;        // vpisna št. 0000 0000 ni validna vrednost.
	char* fname;
	char* lname;
	uint8_t grades[GRADE_COUNT]; // sklepam, da je do 10 ocen iz primera
	                             // alternativa bi bila malloc()/free() in
	                             // naj si zapomne koliko ocej je.
} Student;

int find_first_free_slot ( const Student* const array, const int size ) {
	for( int i = 0; i < size; i++ ) {
		if( !array[i].id ) { // != 0
			return i;
		}
	}
	return -1;
}

int insert_student( Student* const array, const int size, const Student* const s ) {
	const int index = find_first_free_slot( array, size );
	if( index < 0 )
		return -1;

	memset( &array[index], 0, sizeof(uint8_t) * GRADE_COUNT );

	array[index] = *s;
	return index;
}

void print_student( const Student* const array, const int index ) {
	printf("vpisna: %8u\nime:    %10s\npriimek: %10s\nocene: ",
			array[index].id, array[index].fname, array[index].lname);

	for( int i = 0; i < GRADE_COUNT; i++ ) {
		const int grade = array[index].grades[i];
		if( !grade )
			printf(" /");
		else 
			printf(" %u", array[index].grades[i]);
	}
	putchar('\n');
}

// ker podpore za brisanje iz baze ni zahtevano, lahko sklepam, da
// so vsi studenti na začetku baze
int print_students( const Student* const array, const int size ) {

	int length = find_first_free_slot( array, size );
	length = ( length == -1 ? size : length );

	for( int i = 0; i < length; i++ ) {
		print_student( array, i );
	}
	return length;
}

int find_student( Student* const array, const int size, const uint32_t id ) {
	int ret = -1;
	for( int i = 0; i < size; i++ ) {
		if( array[i].id == id ){
			ret = i;
			break;
		}
	}
	return  ret;
}


int main() {
	setvbuf( stdin, NULL, _IONBF, 0 );
	
	const unsigned student_count = 26;    // vrednost podana
	Student students[student_count];      // malloc ni potreben, saj vem št.
	                                      // elementov
	memset( students, 0, sizeof(Student) * student_count );
	
	Student buff = {
		.id = 23190092,
		.fname = "Liam",
		.lname = "Mehle",
		.grades = { 9, 10, 9, 10, 10, 10, 10 }
	};

	//insert_student( students, student_count, &buff );
	//print_students( students, student_count );

	char selection;
	int index;
	int id;
	do {
		puts("\noperacija [Dodaj, Najdi, Izpiši, Končaj]:");
		do {
			selection = (char) getchar();
		} while (selection == '\n');
		Student buffer;
		switch( selection ) {
			case 'd':
			case 'D':
				puts("Vpiši podatke novega studenta:\n"
				     "(če je manj kot 10 ocen, naj zadnji oceni naj sledi 0)");
				buffer.fname = (char*)malloc(NAME_LEN); // pripravi spomin za ime
				buffer.lname = (char*)malloc(NAME_LEN);
				scanf("%s %s %u", buffer.fname, buffer.lname,
					&buffer.id );               // vpisovanje ostalih podatkov
				for( int i = 0; i < 10; i++ ) { // vpisovanje ocen
					unsigned grade;
					scanf("%u", &grade );
					if( grade != 0 ){
						buffer.grades[i] = (uint8_t)grade;
						getchar(); // potroši naslednji presledek
					}
					else
						break;
				}

				insert_student( students, student_count, &buffer );
				break;

			case 'n':
			case 'N':
				puts("Vpišite Vpisno številko:");
				scanf("\n%u", &buffer.id ),
				index = find_student( students, student_count, buffer.id );
				if( index == -1 ) {
					puts("Vpisne številke ni v bazi.");
					break;
				}
				print_student( students, index );
				break;

			case 'i':
			case 'I':
				print_students( students, student_count );
				break;
		}
	} while( selection != 'K' ||selection != 'k' );

}  // free() nepotreben, saj se tu program konča
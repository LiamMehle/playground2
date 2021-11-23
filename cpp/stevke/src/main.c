#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

int main() {

	const int max_len = 1024;
	char string[max_len];
	bool string_is_valid;

	do {
		string_is_valid = true;

		puts("vnesi število:");                     // ----< input > ----
		scanf("%s", string );

		                                            // ----<validate>----
		for( int i = 0; i < (int) strlen(string); i++ ) { // ker je zadnji karaker \n
			const char c = string[i];                 // karakter, ki ga obravnava
			if( isdigit(c) )                          // verjetno je števka
				continue;                               // -> validno
			if( i==0 && c == '-' )                    // če je prvi karakter, je...
				continue;                               // ...minus -> veljavno
			if( string[i]   == '\n' &&                // \n je validen le, če je...
			    string[i+1] == '\0' )                 // ...naslednji karakter null
				continue;
			// če smo prišli do tu, je prišlo do napake..
			string_is_valid = false;
			puts("številka ni validna");
			break;
		}
	} while( !string_is_valid );                  // repeat until valid

	for( int i = 0; i < (int) strlen(string); i++ ) {
		const char c = string[i];
		if( c == '\n' )
			break;

		switch( c ) {     // odvisno od števke, napiše pravo besedo
			case '-':
				printf("minus");
				break;
			case '0':
				printf("nič");
				break;
			case '1':
				printf("ena");
				break;
			case '2':
				printf("dve");
				break;
			case '3':
				printf("tri");
				break;
			case '4':
				printf("štiri");
				break;
			case '5':
				printf("pet");
				break;
			case '6':
				printf("šest");
				break;
			case '7':
				printf("sedem");
				break;
			case '8':
				printf("osem");
				break;
			case '9':
				printf("devet");
				break;
		}
		putchar(' ');        // presledek med besedami
	}
	putchar('\n');         // nova vrstica na koncu v primeru, da ne bo lupina
	return 0;
}


#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
int main_3() {
	// get intput
	const char* filename = "datoteka1.txt";
	FILE* const fd = fopen(filename, "r");

	const size_t buf_size = 1<<10;
	char buffer[buf_size];
	int chars, lines, digits, words;

	while(!feof(fd)) {
		// assume no line is grater than buf_size
		bool was_space = 0;
		const char c = getc(fd);
		printf("%c", c);
		if(c == '\0')
			break;
		if(c == '\n')
			lines++;
		if(isdigit(c))
			digits++;
		if(!isspace(c) && was_space) {
			words++;
			was_space = 0;
		}
		if(isspace(c))
			was_space = 1;;
			chars++;
		bool wasnt_space = false;
	}
	return 0;
}

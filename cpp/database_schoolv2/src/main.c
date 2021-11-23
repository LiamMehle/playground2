#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define assume(x) if(!x) __builtin_unreachable()
#define PAGE 1024
#define NAME_LEN 64
#define GRADE_COUNT 10
// code assumes that SEEK_END is implemented

// ----------------------------------------------

typedef struct {
	char firstname[NAME_LEN];
	char lastname[NAME_LEN];
	uint32_t id;
	uint8_t grades[GRADE_COUNT];
} student_t;

// ----------------------------------------------

size_t fgetsize(FILE* const /*restrict*/ file) {
	if(file == NULL) {
		puts("Datoteka ni odprta.");
		return 0;
	}
	fpos_t current_pos;
	assert(0 == fgetpos(file, &current_pos));
	assert(0 == fseek(file, 0, SEEK_END));
	const size_t size = ftell(file);
	assert(0 == fsetpos(file, &current_pos));
	return size;
}

void print_file(FILE* const /*restrict*/ file) {
	if(file == NULL) {
		puts("Datoteka ni odprta.");
		return;
	}
	// 1 MB = 1k * page
	const size_t mem_size = 1000 * PAGE;
	char* const mem = malloc(mem_size);          // assume success
	const size_t size = fgetsize(file);
	fseek(file, 0, SEEK_SET);                    // assume success
	for(size_t i = 0; i < ((size-1)/mem_size)+1; i++) {
		fread (mem, PAGE, mem_size/PAGE, file);  // assume success
		fwrite(mem, PAGE, mem_size/PAGE, stdout);// assume success
	}
	free(mem);
}

void add_student(FILE* const /*restrict*/ file,
      const student_t* const /*restrict*/ s) {
	if(file == NULL) {
		puts("Datoteka ni odprta.");
		return;
	}

	fseek(file, 0, SEEK_END);
	fputc('\n', file);
	fprintf(file, "ime:     \"%s\",\n"
	              "priimek: \"%s\"\n"
	              "vpisna:  %08u\n"
	              "ocene: { ", s->firstname, s->lastname, s->id);
	for(int i = 0; i < 10; i++)
		fprintf(file, "%u ", s->grades[i]);
	fputc('}', file);
}

// finds substring(target) in string
// there is probbably much room for optimizaiton here
size_t find(const char* const /*restrict*/ string,
            const char* const /*restrict*/ target,
            const size_t from) {
	assert(string[0] != '\0');
	assert(target[0] != '\0');
	size_t string_index = from;
	size_t target_index = 0;

	for(; string[string_index] != '\0'; string_index++) {
		if(string[string_index] != target[target_index]) {
			target_index = 0;
			continue;
		} else { // they match
			if(target[target_index+1] == '\0') // end of string
				return string_index - target_index;
			target_index++;
		}
	}
	return -1;
}

size_t rfind(const char* const /*restrict*/ string,
             const char* const /*restrict*/ target,
             const size_t from) {
	assert(string[0] != '\0');
	assert(target[0] != '\0');
	//assert(string[from] != 0);
	size_t string_index = from-1;
	size_t target_string_len = strlen(target);
	assert(target_string_len != 0);
	size_t target_index = target_string_len;

	for(; string_index >= 0; string_index--) {
		if(string[string_index] != target[target_index]) {
			target_index = target_string_len-1;
			continue;
		} else { // they match
			if(target_index-1 == 0) // end of target
				return string_index;
			target_index--;
		}
	}
	return -1;
}

void rm_by_id(FILE* /*restrict*/ file, uint32_t id) {
	if(file == NULL) {
		puts("Datoteka ni odprta.");
		return;
	}
	char* const target = alloca(12);
	sprintf(target, "vpisna:  %08u", id);
	const size_t size = fgetsize(file);
	char* const data = malloc(size);
	fseek(file, 0, SEEK_SET);                        // assume success
	assert(fread(data, size, 1, file) != 0);                      // assume success

	const size_t temp        =  find(data, target, 0),
	             delete_from = rfind(data, "ime:", temp),
	             delete_to   =  find(data, " }", temp);

	file = freopen(NULL, "w+", file);                // assume success
	fseek(file, 0, SEEK_SET);                        // assume success
	fwrite(data,           delete_from-1,  1, file); // assume success
	fwrite(data+delete_to+2, size-delete_to-2, 1, file); // assume success
}

void find_by_id(FILE* /*restrict*/ file, uint32_t id) {
	if(file == NULL) {
		puts("Datoteka ni odprta.");
		return;
	}
	char* const target = alloca(12);
	sprintf(target, "vpisna:  %08u", id);
	const size_t size = fgetsize(file);
	char* const data = malloc(size);
	fseek(file, 0, SEEK_SET);                        // assume success
	assert(fread(data, size, 1, file) != 0);                      // assume success

	const size_t temp        =  find(data, target, 0),
	             print_from = rfind(data, "ime:", temp),
	             print_to   =  find(data, " }", temp);
                          // assume success
	fwrite(data+print_from, print_to-print_from+2, 1, stdout); // assume success
}

void find_by_name(FILE* /*restrict*/ file, const char* const firstname, const char* const lastname) {
	if(file == NULL) {
		puts("Datoteka ni odprta.");
		return;
	}
	char* const target = alloca(24);
	sprintf(target, "ime:     \"%s\"\n"
	                "priimek: \"%s\"\n ", firstname, lastname);
	const size_t size = fgetsize(file);
	char* const data = malloc(size);
	fseek(file, 0, SEEK_SET);                        // assume success
	assert(fread(data, size, 1, file) != 0);                      // assume success

	const size_t print_from =  find(data, target, 0),
	             print_to   =  find(data, " }", print_from);
                          // assume success
	fwrite(data+print_from, print_from-print_to-2, 1, stdout); // assume success
}

bool file_exists(const char* const /*restrict*/ filename) {
	// fails if file does not exist
	FILE* file = fopen(filename, "r");
	if(file == NULL)
		return false;
	fclose(file);
	return true;
}

FILE* open(const char* /*restrict*/ data_filename) {

	FILE* /*restrict*/ file;
	if(file_exists(data_filename))
		file = fopen(data_filename, "r+");
	else {
		file = fopen(data_filename, "w+");
		fputs("ime,priimek,vpisna,ocene\n", file); 
	}

	if(file == NULL) {
		puts("datoteka ni uspešno odprta");
	}
	return file;
}

void find_menu(FILE* const restrict file) {
	student_t s;
	int command;
	puts("\n"
		"1: iskanje po vpisni\n"
		"2: izkanje po imenu in priimku\n"
		"3: izkanje po ocenah (not implemented)\n");
	scanf("%d", &command);
	switch(command) {
		case 0:
			return;
		case 1:
			printf("vpisna:  :");
			scanf("%u", &s.id);
			find_by_id(file, s.id);
			break;
		case 2:
			fflush(file);
			printf("ime:     :");
			scanf("%s", s.firstname);
			printf("priimek: :");
			scanf("%s", s.lastname);
			find_by_name(file, s.firstname, s.lastname);
			break;
		default:
			puts("ukaz ni prepoznan");
	}
}

int main(int argc, char** argv) {

	const char* /*restrict*/ data_filename = "data.txt";
	if(argc >= 2)
		data_filename = argv[1];
	printf("baza podatkov: %s", data_filename);

	FILE* file;

	int command;
	do {
		puts("\n"
			 "1: izpis baze podatkov\n"
		     "2: vnos novega studenta\n"
		     "3: odstranitev studenta\n"
		     "4: iskanje\n"
		     "5: odpri\n"
		     "6: zapri");
		scanf("%d", &command);
		student_t s;
		switch(command) {
			case 0:
				break;
			case 1:
				print_file(file);
				break;
			case 2:
				if(file == NULL) {
					puts("Datoteka ni odprta.");
					break;
				}
				fflush(stdin);
				memset(&s, 0, sizeof(s));
				printf("ime:     :");
				scanf("%s", s.firstname);
				printf("priimek: :");
				scanf("%s", s.lastname);
				printf("vpisna:  :");
				scanf("%u", &s.id);
				printf("ocene:   { ");
				for(int i = 0; i < 10; i++) {
					scanf("%u", (int*)&s.grades[i]);
					if(s.grades[i] == 0) break;
				}
				add_student(file, &s);
				break;
			case 3:
				printf("vpisna:  :");
				scanf("%u", &s.id);
				rm_by_id(file, s.id);
				break;
			case 4:
				find_menu(file);
				break;
			case 5:
				file = open(data_filename);
				break;
			case 6:
				fclose(file);
				file = NULL;
				break;
			default:
				puts("ukaz ni prepoznan");
		}
	} while(command != 0);

	fclose(file);

	return 0;
}

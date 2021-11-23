#include <cstdio>


#ifdef DEBUG 
static
inline
void log(char const* msg) {
	printf("[LOG]: %s", msg);
}
#else
static
inline
void log(char const* msg) {}
#endif
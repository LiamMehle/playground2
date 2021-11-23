
extern "C" {
#include <gint/display.h>
#include <gint/keyboard.h>
#include <stddef.h>
#include <stdlib.h>
}

#include "array"
#include "static.hpp"

constexpr
int line(int y) {
	return y*8;
}

extern "C"
int main() {
	dclear(C_WHITE);
	dtext_static<1, 1>(C_BLACK, "Sample fxSDK add-in");
	while(true) {
		auto mem = malloc(1);
		dtext_static<1, 1*8>(C_BLACK, "malloc gave memory");
		dprint(1, 2*8, C_BLACK, "0x%x", mem);
		dupdate();
		getkey();
	}
	return 1;
}

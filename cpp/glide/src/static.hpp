// support for compile-time abstraction and correctnes checking
#pragma once
#include "display_data.hpp"

extern "C" {
#include <gint/display.h>
}


template<int x, int y, int I>
void dtext_static(int const type, char const (&msg)[I]) {
	static_assert(I*CHAR_WIDTH + x < WIDTH_PX);
	static_assert(y < HEIGHT_PX);
	dtext(x, y, type, msg);
}
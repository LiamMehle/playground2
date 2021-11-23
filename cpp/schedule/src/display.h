#pragma once
#include <vector>
#include <string_view>
#include "types.h"
void debug_print(std::vector<Period> const& periods);
void debug_print(char const* msg);
void debug_print(std::string_view const msg);
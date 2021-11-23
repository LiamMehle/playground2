#pragma once

#ifndef __has_builtin
#define __has_builtin(x) 0
#endif

#ifndef __assume
#ifdef  __builtin_unreachable
#define __assume(x) if(!(x)) __builtin_unreachable();
#endif
#endif

#define LOG(x) std::cout << #x << " = " << x << '\n';
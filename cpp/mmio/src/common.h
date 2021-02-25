#ifndef COMMON_H
#define COMMON_H

#include <errno.h>
#include <stdio.h>
#include <string>
#define check_ptr( ptr ) (_check_ptr( ptr, std::string(__FILE__).c_str(), __LINE__ ))
[[nodiscard]]
int _check_ptr( const void* ptr, const char* file, int line ) noexcept;

void msleep( uint_fast16_t msec );


#endif

#ifndef TEST_H
#define TEST_H
#include <time.h>
#include <cstdint>
#include "mmio.h"
#include "common.h"
#include <stdio.h>

void test_function( uint32_t* io_base, int function );
void test_pin( uint32_t* io_base, int pin );

#endif

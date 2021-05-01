PWD = $(shell pwd)

.DEFAULT_GOAL = all                # should be unneccessary

binary_name = main#                # desired bin name

include ../Makefile.x86_64.gcc      # compilation logic
#include ../Makefile.pi.clang       # compilation logic

# aditional project-specific flags
CFLAGS += -std=c17 -Wall -Wconversion -Wextra -Ofast -fno-exceptions
CFLAGS += 
CFLAGS +=#-fsanitize=address # -I /usr/include/arm-linux-gnueabihf/ 

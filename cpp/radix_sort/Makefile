PWD = $(shell pwd)

.DEFAULT_GOAL = all                # should be unneccessary

binary_name = main#                # desired bin name

include ../Makefile.x86_64.gcc      # compilation logic
#include ../Makefile.pi.clang       # compilation logic

# aditional project-specific flags
CFLAGS += -Wall -Wconversion -Wextra -Werror -O0 -fno-exceptions -std=c++17
CFLAGS += -g
CFLAGS += #-I /usr/include/arm-linux-gnueabihf/

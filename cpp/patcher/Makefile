PWD = $(shell pwd)

.DEFAULT_GOAL = all                # should be unneccessary

binary_name = main#                # desired bin name

include ../Makefile.x86_64.gcc      # compilation logic
#include ../Makefile.pi.clang       # compilation logic

# aditional project-specific flags
CFLAGS += -Wall -Wconversion -Wextra -Wshadow -O3 -fno-exceptions -std=c++20
CFLAGS += -g -lpthread -flto
CFLAGS += -I /usr/include/arm-linux-gnueabihf/ #-fsanitize=address

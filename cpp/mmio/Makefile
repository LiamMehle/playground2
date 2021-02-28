PWD = $(shell pwd)

.DEFAULT_GOAL = all                # should be unneccessary

binary_name = mmio                 # desired bin name

include ../Makefile.pi.clang       # compilation logic
# aditional project-specific flags
CFLAGS += -Wall -Wconversion -Werror -Ofast -fno-exceptions -std=c++17 -g
CFLAGS += -fopenmp -Wno-c99-extensions
CFLAGS += -I /usr/include/arm-linux-gnueabihf/ #-fsanitize=address

	PWD = $(shell pwd)

                       # should be unneccessary
.DEFAULT_GOAL = all
                       # desired bin name
binary_name = main

include ../Makefile.x86_64.gcc       # compilation logic
# aditional project-specific flags
CFLAGS += -Wall -Wconversion -Wextra -Wshadow -Ofast -g -std=c++2a
CFLAGS += -pthread -lstdc++fs
CFLAGS += #-I /usr/include/arm-linux-gnueabihf/  # -fsanitize=address -static-libasan

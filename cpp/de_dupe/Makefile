	PWD = $(shell pwd)

                       # should be unneccessary
.DEFAULT_GOAL = all
                       # desired bin name
binary_name = main

include ../Makefile.pi.gcc       # compilation logic
# aditional project-specific flags
CFLAGS += -Wall -Wconversion -Wextra -Werror -Ofast -g -std=c++17
CFLAGS += -pthread
CFLAGS += -I /usr/include/arm-linux-gnueabihf/ -lstdc++fs # -fsanitize=address -static-libasan

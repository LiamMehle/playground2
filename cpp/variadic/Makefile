PWD = $(shell pwd)

.DEFAULT_GOAL = all                # should be unneccessary

binary_name = main#                # desired bin name

include ../Makefile.x86_64.gcc       # compilation logic
# aditional project-specific flags
CFLAGS += #-Wall -Wconversion -Wextra -Werror -O0 -fno-exceptions -std=c++17
CFLAGS += -std=c++17
CFLAGS += #-I /usr/include/arm-linux-gnueabihf/ #-fsanitize=address

PWD = $(shell pwd)

.DEFAULT_GOAL = all                # should be unneccessary

binary_name = main#                 # desired bin name

include ../Makefile.pi.clang       # compilation logic
# aditional project-specific flags
CFLAGS += -Wall -Wconversion -Wextra -Werror -O0 -std=c11
CFLAGS +=
CFLAGS += -I /usr/include/arm-linux-gnueabihf/ #-fsanitize=address

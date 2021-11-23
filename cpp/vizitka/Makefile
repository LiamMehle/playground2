PWD = $(shell pwd)

.DEFAULT_GOAL = all                # should be unneccessary

binary_name = main                 # desired bin name

include ../Makefile.pi.clang       # compilation logic
# aditional project-specific flags
CFLAGS += -Wall -Wconversion -Wextra -Wpedantic -Werror -Ofast -fno-exceptions -std=c90
CFLAGS += -fopenmp -Wno-c99-extensions -g -fno-omit-frame-pointer
CFLAGS += -I /usr/include/arm-linux-gnueabihf/ # -fsanitize=address

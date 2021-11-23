PWD = $(shell pwd)

.DEFAULT_GOAL = all                # should be unneccessary

binary_name = sort                 # desired bin name

include ../Makefile.pi.clang       # compilation logic
# aditional project-specific flags
CFLAGS += -Wall -Wconversion -Werror -O0 -fno-exceptions
CFLAGS += -fopenmp -Wno-c99-extensions
CFLAGS += -I /usr/include/arm-linux-gnueabihf/ #-fsanitize=address

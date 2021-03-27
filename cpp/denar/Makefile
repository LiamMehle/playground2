PWD = $(shell pwd)

.DEFAULT_GOAL = all                # should be unneccessary

binary_name = main                 # desired bin name

include ../Makefile.pi.gcc       # compilation logic
# aditional project-specific flags
CFLAGS += -Wall -Wconversion -Wextra -Wpedantic -Werror -O1 -fno-exceptions -std=c90
CFLAGS += -g #-fsanitize=address
CFLAGS += -I /usr/include/arm-linux-gnueabihf/

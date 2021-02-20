PWD = $(shell pwd)

.DEFAULT_GOAL = all                # should be unneccessary

binary_name = mmio                 # desired bin name

include ../Makefile.pi.clang       # compilation logic
# aditional project-specific flags
CFLAGS += -Wall -Wpedantic -Wconversion -O0 -fno-exceptions -std=c++17 -g
CFLAGS += -fopenmp -mcpu=cortex-a53 -mfpu=neon -mfloat-abi=hard
CFLAGS += -I /usr/include/arm-linux-gnueabihf/ # -fsanitize=address

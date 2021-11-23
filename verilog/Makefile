C = iverilog

# get targets in ./src/
temp   := $(notdir $(wildcard src/*.v))
targets = $(subst .v,,$(temp))

# setting defaults
top_module := main
bin_name   := main

all: simulate

simulate: compile
	vvp bin/$(bin_name)

compile: dir
	iverilog -s $(top_module) -o bin/$(bin_name) src/$(temp)

dir:
	-mkdir bin

clean:
	-rm -rf bin

# main logic for compiling C/C++

# generate list of object files to generate
# enumerate sources, replaec .cpp with .o
a := $(notdir $(wildcard src/*.cpp)) $(notdir $(wildcard src/*.c))
b   := $(subst .c,.o,$(a:.cpp=.o))
.PHONY: Makefile ../Makefile ../Makefile.pi # todo: fix

OPTFLAGS =
INFOFLAGS = -fopt-info
CFLAGS += $(OPTFLAGS)

all: $(b) make_dirs
	$(CXX) $(CFLAGS) $(addprefix obj/,$(b)) -o bin/$(binary_name)

%.o : make_dirs src/%.cpp
	$(CXX) -c -o obj/$@ src/$(subst .o,.cpp,$@) $(CFLAGS)

%.o : make_dirs src/%.c
	$(CC) -c -o obj/$@ src/$(subst .o,.c,$@) $(CFLAGS)

make_dirs:
	-mkdir obj
	-mkdir bin

clean:
	-rm -r obj
	-rm -r bin

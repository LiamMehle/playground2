MAIN_SOURCES = functions.hs

FLAGS = -O2  -dynamic -Wno-tabs -threaded

all: main

main: $(MAIN_SOURCES:.hs=.o)
	ghc    $^ -o $@ $(FLAGS)

%.o : %.hs
	ghc -c $< -o $@ $(FLAGS)

partial-clean:
	rm $(MAIN_SOURCES:.hs=.o)

clean: partial-clean
	rm main
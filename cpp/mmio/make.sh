find . | entr bash -c 'make clean -j4 && clear ; time make -j4 ; size bin/mmio'

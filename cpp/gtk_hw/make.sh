#find src/ | entr bash -c 'make clean -j4 && clear ; time make -j4 ; size bin/main'
find src/ | entr bash -c 'rm -rf build ; clear ; mkdir build ; cd build ; cmake .. ; make -j12'

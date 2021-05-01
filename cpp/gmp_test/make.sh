find src/ | entr bash -c 'make clean -j4 && clear ; make -j4 ; perf stat bin/main'

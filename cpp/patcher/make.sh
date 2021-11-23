#find src/ | entr bash -c 'make clean -j4 && clear ; time make -j4 ; ls -lh bin/main'
find src/ | entr bash -c 'clear ; time make -j4 ; bin/main'

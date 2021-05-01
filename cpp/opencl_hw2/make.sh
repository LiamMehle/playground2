find src/ | entr bash -c 'make clean -j4 && clear ; cmake . && time make -j4 && bin/main'

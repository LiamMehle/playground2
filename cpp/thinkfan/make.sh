find src/ | entr bash -c 'cmake . && make clean -j12 && clear && time make -j12 && sudo perf stat bin/thinkfanc'

#! /bin/bash

: sudo rm $( find . -name 'perf.data'     -type f )
: sudo rm $( find . -name 'perf.data.old' -type f )

echo processing cmake projects
cd cmake
	cd hw
		: rm -rf CMakeFiles      &
		: rm CMakeCache.txt      &
		: rm cmake_install.cmake &
		: rm hw                  &
		: rm Makefile            &
	cd ..
cd ..


# clean every single project in cpp
echo processing cpp projects
PWD=$(pwd)
cd cpp
	for D in `find . -maxdepth 1 -type d`
	do
		if [ $D == '..' ]
		then
			continue
		fi
		( cd $D ; : make clean -j4 ) &
	done
cd ..
for job in `jobs -p`
do
	wait -n $job
done

echo done preprocess

git add *
git commit -m "$1"

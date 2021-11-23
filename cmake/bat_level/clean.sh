#!/bin/bash

rm -r CMakeFiles       &
rm batt_level.exe      &
rm batt_level          &
rm CMakeFiles          &
rm CMakeCache.txt      &
rm cmake_install.cmake &

wait $(jobs -p)

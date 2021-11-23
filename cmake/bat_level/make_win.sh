#!/bin/sh
cmake -D CMAKE_CC_COMPILER=x86_64-w64-mingw32-gcc \
      -D CMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++ \
      .

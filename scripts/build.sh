#!/bin/bash
set -e

rm -f  CMakeCache.txt
rm -rf CMakeFiles
unzip -u thirdparty/gtest-1.7.0.zip -d thirdparty/.

mkdir -p build
cd build
rm -f CMakeCach*
rm -rf CMakeFiles

cmake -DVERSION=1.0 \
      -DCMAKE_CXX_COMPILER_ARG1:STRING=' -ggdb -Wall -Werror -fPIC -O0 -m64 -Wl,-rpath -Wl,.' ..

make -j6

printf "\n\nLook for build output in \"build\" directory\n"


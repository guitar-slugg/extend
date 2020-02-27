#!/bin/bash

echo "cleaning"

rm -rf ./Makefile
rm -rf ./CMakeFiles
rm -rf ./cmake_install.cmake
rm -rf ./CmakeCache.txt

rm -rf ./test/Makefile
rm -rf ./test/CMakeFiles
rm -rf ./test/cmake_install.cmake
rm -rf ./test/CmakeCache.txt

rm -rf ./bin
rm -rf ./lib

rm *.log
rm *.json

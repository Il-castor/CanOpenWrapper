#!/bin/bash

if [ -d ./build ]; then
    rm -r ./build
fi

mkdir build && cd build
cmake ..
make

if [ -f main ]; then
    mv main ../main
else
    echo "--------------------[  ERROR on COMPILING  ]--------------------"
    exit 1 
fi
#!/bin/bash

mkdir build
bnfc -m -cpp -l -o build $1
cd build
make
cd ..
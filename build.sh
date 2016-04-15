#!/bin/bash

grammarfile=$(basename $1)
name="${grammarfile%.*}"
rm "${name}" # Delete old executable.
mkdir "${name}.build"
cd "${name}.build"
rm -f * # Clear build directory.
bnfc -m -cpp -l ../$1
make
cd ..
mv "${name}.build/Test${name}" "${name}"

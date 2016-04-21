#!/bin/bash

if [ "$#" -ne 1 ]; then
	echo "Usage: test.sh grammarfile.cf"
else
	# Remove all lines starting with / (comments)
	# and save the result as name.cf 
	# inside the test directory.
	grep -v '^ *//' "$1" > "test/$1"

	# Build and run testsuite.
	cd test
	ghc progs-test-lab1.hs
	./progs-test-lab1 "$1"
fi
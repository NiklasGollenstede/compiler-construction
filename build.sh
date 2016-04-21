#!/bin/bash

if [ "$#" -ne 1 ]; then
	echo "Usage: build.sh grammarfile.cf"
else
	# Get rules file name without extension.
	grammarfile=$(basename $1)
	name="${grammarfile%.*}"

	# Delete old executable.
	rm "${name}" 

	# Create build directory.
	if [ ! -d "${name}.build" ]; then
	  mkdir "${name}.build"
	fi

	# Remove all lines starting with / (comments)
	# and save the result as name.temp.cf 
	# inside the build directory.
	grep -v '^ *//' $1 > "${name}.build/${name}.temp.cf"

	# Invoke BNFC.
	cd "${name}.build"
	bnfc -m -cpp -l "${name}.temp.cf"

	# Invoke make.
	make

	# Move test executable to original directory.
	cd ..
	mv "${name}.build/Test${name}" "${name}"
fi
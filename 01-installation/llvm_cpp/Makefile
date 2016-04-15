all: toy

toy:
	clang++ -g -O3 toy.cpp `llvm-config --cxxflags --ldflags --system-libs --libs core` -o toy

clean:
	rm -f toy

test: toy
	echo "def foo(x y) x+foo(y, 4.0);" | ./toy

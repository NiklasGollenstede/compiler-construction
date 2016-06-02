CC = g++
CCFLAGS = -g -std=gnu++11
EXE = typechecker

all:
	${CC} ${CCFLAGS} *.cc ./cpp.build/*.C -o ${EXE}

run:
	./${EXE} test.cpp

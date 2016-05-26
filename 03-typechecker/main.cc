#include "type_checker.h"
#include "Parser.H"
#include <cassert>
#include <iostream>
#include <fstream>

#define ASSERT(expr)         assert((expr))
#define ASSERT_EQ(v0, v1)    assert((v0) == (v1))
#define ASSERT_NULL(ptr)     assert((ptr) == nullptr)
#define ASSERT_NOT_NULL(ptr) assert((ptr) != nullptr)
#define LOG(message)         std::cout << (message) << std::endl;

int main(int argc, char **argv) {
	Program *program = pProgram(fopen(argv[1], "r"));

	TypeChecker typeChecker;
	typeChecker.visitProgram(program);
	delete program;

	LOG("Still alive.");
}

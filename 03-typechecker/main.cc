#include <cassert>
#include <fstream>
#include "type_checker.h"
#include "cpp.build/Parser.H"

#define ASSERT(expr)         assert((expr))
#define ASSERT_EQ(v0, v1)    assert((v0) == (v1))
#define ASSERT_NULL(ptr)     assert((ptr) == nullptr)
#define ASSERT_NOT_NULL(ptr) assert((ptr) != nullptr)
#define LOG(message)         std::cout << message << std::endl;

int main(int argc, char const** argv) {
	if(argc != 2) {
		LOG("Usage: typechecker source.cpp");
	} else {
		FILE* file = fopen(argv[1], "r");
		if(file == nullptr) {
			LOG("Could not open source file.");
		} else {
			LOG("Opened source file.");
			Program* program = pProgram(file);
			LOG("Starting Typechecker.");
			TypeChecker typeChecker;
			typeChecker.visitProgram(program);
			LOG("AST traversal completed.");
			delete program;
		}
	}
}

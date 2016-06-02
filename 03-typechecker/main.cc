#include <cassert>
#include <fstream>
#include "type_checker.h"
#include "cpp.build/Parser.H"

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
			TypeChecker* typechecker = new TypeChecker();
			typechecker->visitProgram(program);
			delete typechecker;
			LOG("AST traversal completed.");
			delete program;
		}
	}
}

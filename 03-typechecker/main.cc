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
			Program* program = pProgram(file);
			LOG("Invoking typechecker on source file: " << argv[1]);
			TypeChecker checker;
			checker.visitProgram(program);
			LOG("Done.");
			delete program;
		}
	}
}

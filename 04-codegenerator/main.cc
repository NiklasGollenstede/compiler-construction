#include "type_checker.h"
#include "llvm_ir_gen.h"
#include "cpp.build/Parser.H"

int main(int argc, char const** argv) {
	
	// Wrong usage.
	if(argc != 2) {
		std::cerr << "Usage: tccpp source.cpp" << std::endl;
	} 

	// Open specified input file.
	else {
		FILE* file = fopen(argv[1], "r");
		if(file == nullptr) {
			std::cerr << "FILE NOT FOUND" << std::endl;
		} 

		else {
			Program* program = pProgram(file);
			if(program == nullptr) {
				std::cerr << "SYNTAX ERROR" << std::endl;
			} 

			else try {
				// Typecheck program.
				TypeChecker checker;
				checker.visitProgram(program);
				
				// Generate LLVM bitcode.
				LLVMIRGen codegen("test", checker.getEnv());
				codegen.visitProgram(program);

				std::cout << "OK" << std::endl;
				delete program;

			} catch(std::exception const& e) {
					std::cerr << "TYPE ERROR" << std::endl;
					std::cerr << e.what() << std::endl;
					delete program;
			}
		}
	}
}

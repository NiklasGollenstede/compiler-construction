#include "type_checker.h"
#include "llvm_ir_gen.h"
#include "cpp.build/Parser.H"

static FILE*       g_file;
static Program*    g_program;
static TypeChecker g_typechecker;
static LLVMIRGen*  g_codegen;

void cleanup() {

	// Cleanup input file.
	if(g_file != nullptr) {
		fclose(g_file);
		g_file = nullptr;
	}

	// Cleanup program.
	if(g_program != nullptr) {
		delete g_program;
		g_program = nullptr;
	}

	// Cleanup code generator.
	if(g_codegen != nullptr) {
		delete g_codegen;
		g_codegen = nullptr;
	}
}

// Typecheck program.
void typecheck() {
	try {
		std::cout << "TYPECHECKING" << std::endl;
		g_typechecker.visitProgram(g_program);
		std::cout << "OK" << std::endl;
	} catch(std::exception const& e) {
		std::cerr << "TYPE ERROR" << std::endl;
		std::cerr << e.what() << std::endl;
		cleanup();
	}
}

// Generate LLVM bitcode.
void codegen() {
	try {
		std::cout << "GENERATING CODE" << std::endl;
		g_codegen = new LLVMIRGen("test", g_typechecker.getEnv());
		g_codegen->visitProgram(g_program);
		std::cout << "OK" << std::endl;
		std::cout << "BITCODE BELOW" << std::endl << std::endl;
		g_codegen->printModule();
		
	} catch(std::exception const& e) {
		std::cerr << "CODEGEN ERROR" << std::endl;
		std::cerr << e.what() << std::endl;
		cleanup();
	}
}

int main(int argc, char const** argv) {
	
	// Wrong usage.
	if(argc != 2) {
		std::cerr << "Usage: tccpp source.cpp" << std::endl;
	} 

	// Open specified input file.
	else {
		if((g_file = fopen(argv[1], "r")) == nullptr) {
			std::cerr << "FILE NOT FOUND" << std::endl;
		} else {
			if((g_program = pProgram(g_file)) == nullptr) { 
				std::cerr << "SYNTAX ERROR" << std::endl;
			} else { 
				typecheck();
				codegen();
			}			
		}
	}

	cleanup();
}
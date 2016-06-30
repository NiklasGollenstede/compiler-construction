#include "type_checker.h"
#include "code_generator.h"
#include "cpp.build/Parser.H"

static FILE*           g_file;
static Program*        g_program;
static TypeChecker 	   g_typechecker;
static CodeGenerator*  g_codegen;

void cleanup() {
	std::cout << std::endl << "EXITING" << std::endl;

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

	exit(0);
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
void codegen(std::string path) {
	try {
		std::cout << "GENERATING CODE" << std::endl;
		g_codegen = new CodeGenerator("test", g_typechecker.getEnv());
		g_codegen->visitProgram(g_program);
		std::cout << "OK" << std::endl;
		std::cout << "BITCODE BELOW" << std::endl << std::endl;
		g_codegen->printModule();
		g_codegen->saveToFile(path);

	} catch(std::exception const& e) {
		std::cerr << "CODEGEN ERROR" << std::endl;
		std::cerr << e.what() << std::endl;
		cleanup();
	}
}

bool replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}


int main(int argc, char const** argv) {

	// Wrong usage.
	if(argc != 2) {
		std::cerr << "Usage: ccpp source.cpp" << std::endl;
	}

	// Open specified input file.
	else {
		if((g_file = fopen(argv[1], "r")) == nullptr) {
			std::cerr << "FILE NOT FOUND" << std::endl;
		} else {
			if((g_program = pProgram(g_file)) == nullptr) {
				std::cerr << "SYNTAX ERROR" << std::endl;
			} else {
				std::string infile(argv[1]);
				typecheck();
				replace(infile, ".cpp", ".ll");
				codegen(infile);
			}
		}
	}

	cleanup();
}

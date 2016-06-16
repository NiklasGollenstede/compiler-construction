#ifndef FUNCTION_H
#define FUNCTION_H

#include "common.h"
#include "datatype.h"
#include "scope.h"

struct Function {
	Function(
		Datatype* returnType, 
		std::string const& name, 
		std::vector<Variable*>* args,
		Scope* scope);

	Datatype* returnType;
	std::string name;
	std::vector<Variable*>* args;
	Scope* scope;
	llvm::Function* llvmHandle;
	
	~Function();
};

#endif
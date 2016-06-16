#include "function.h"

Function::Function(
	Datatype* returnType, 
	std::string const& name, 
	std::vector<Variable*>* args,
	Scope* scope
)   : returnType(returnType),
	  name(name),
	  args(args),
	  scope(scope),
	  llvmHandle(nullptr) {
	  	for(auto& arg : *args) scope->registerVariable(arg);
}

Function::~Function() {
	delete returnType; returnType = nullptr;
	delete args; args = nullptr;
	scope = nullptr; // Deleted from within env recursively.
}
#include "variable.h"

Variable::Variable(
	Datatype* type, 
	std::string const& name,
	llvm::Value* value
)	:type(type),
	 name(name),
	 value(value) {	 	
}

 Variable::~Variable() {
 	delete type; type = nullptr;
 	delete value; value = nullptr;
 }
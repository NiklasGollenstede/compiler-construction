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
 	if(type)  delete type;  type  = nullptr;
 	// Value ptr managed by LLVM.
 }
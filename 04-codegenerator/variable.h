#ifndef VARIABLE_H
#define VARIABLE_H

#include "common.h"
#include "datatype.h"

// Forward declare llvm::Value class.
namespace llvm { class Value; }

struct Variable {
	Datatype type;
	std::string name;
	llvm::Value* value;
};

#endif
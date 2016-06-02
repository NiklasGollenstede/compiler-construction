#ifndef FUNCTION_H
#define FUNCTION_H

#include "common.h"
#include "datatype.h"

struct Function {
	Datatype returnType;
	std::string name;
	std::vector<Variable> args;
};

#endif
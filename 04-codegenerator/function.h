#ifndef FUNCTION_H
#define FUNCTION_H

#include "common.h"
#include "datatype.h"
#include "scope.h"

struct Function : public Scope {
	Datatype returnType;
	std::string name;
	std::vector<Variable> args;
};

#endif
#ifndef VARIABLE_H
#define VARIABLE_H

#include "common.h"
#include "datatype.h"

struct Variable {
	Datatype type;
	std::string name;
};

#endif
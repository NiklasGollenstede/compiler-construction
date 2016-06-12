#ifndef SCOPE_H
#define SCOPE_H

#include "common.h"
#include "variable.h"

class Scope {
private:
	std::map<std::string, Variable> m_vars;

public:
	// Returns the variable or nullptr.
	Variable* lookupVariable(std::string const& name);

	// Returns false if name already exists.
	bool registerVariable(Variable const& variable);
};

#endif
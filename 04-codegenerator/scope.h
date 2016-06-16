#ifndef SCOPE_H
#define SCOPE_H

#include "common.h"
#include "variable.h"

class Scope {
private:
	Scope* m_parent;
	std::map<std::string, Variable*> m_vars;
	std::vector<Scope*> m_children;

	// Create a new scope inside the parent scope.
	Scope(Scope* parent);

public:
	// Create and return a global scope.
	static Scope* createGlobalScope();	

	// Returns the variable or nullptr.
	Variable* lookupVariable(std::string const& name);

	// Returns false if name already exists.
	bool registerVariable(Variable* variable);

	// Create and returns a new child scope.
	Scope* createChildScope();

	// Return the parent scope.
	Scope* getParentScope();

	// Delete child scopes.
	~Scope();
};

#endif
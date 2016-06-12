#include "scope.h"

Variable* Scope::lookupVariable(std::string const& name) {
	if(m_vars.count(name)) return &m_vars[name];
	else return nullptr;
}

bool Scope::registerVariable(Variable const& variable) {
	if(lookupVariable(variable.name) == nullptr) {
		m_vars[variable.name] = variable;
		return true;
	} else {
		// Var already exists.
		return false;
	}
}

#include "env.h"

// SCOPE

Variable const *Scope::lookupVariable(std::string const& name) {
	if(m_vars.count(name)) return &m_vars[name];
	else return nullptr;
}

bool Scope::registerVariable(Variable const& variable) {
	if(lookupVariable(variable.name) == nullptr) {
		m_vars[variable.name] = variable;
	} else return false;
}

// ENVIRONMENT

Env::Env() : m_scopes({new Scope()}) { }

Function const *Env::lookupFunction(std::string const& name) {
	if(m_funcs.count(name)) return &m_funcs[name];
	else return nullptr;
}

bool Env::registerFunction(Function const& function) {
	if(lookupFunction(function.name) == nullptr) {
		m_funcs[function.name] = function;
		return true;
	} else return false;
}

Variable const *Env::lookupVariable(std::string const& name) {
	Variable const *result = nullptr;

	// Search scopes, starting with innermost.
	for(auto iter = m_scopes.rbegin(); iter != m_scopes.rend(); ++iter) {
		if((result = (*iter)->lookupVariable(name)) != nullptr) {
			return result;
		}
	}

	// Not found.
	return nullptr;
}

bool Env::registerVariable(Variable const& variable) {
	return (*m_scopes.rbegin())->registerVariable(variable);
}

void Env::pushScope() {
	m_scopes.push_back(new Scope());
}

bool Env::popScope() {
	if(m_scopes.size() > 1) {
		m_scopes.pop_back();
		return true;
	} else return false;
}

Env::~Env() {
	for(auto iter = m_scopes.rbegin(); iter != m_scopes.rend(); ++iter) {
		if(*iter != nullptr) {
			delete *iter;
			*iter = nullptr;
		}
	}
}
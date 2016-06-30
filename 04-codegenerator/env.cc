#include "env.h"
#include "type_checker.h"
#include "cpp.build/Absyn.H"

Env::Env() 
	: m_globalScope(Scope::createGlobalScope()),
	  m_currentScope(m_globalScope), 
	  m_lastFunc(nullptr),
	  m_temp(nullptr) { 
}

Function* Env::lookupFunction(std::string const& name) {
	if(m_funcs.count(name)) return m_funcs[name];
	else return nullptr;
}

bool Env::registerFunction(Function* function) {
	if(lookupFunction(function->name) == nullptr) {
		m_funcs[function->name] = function;
		m_lastFunc = function;
		return true;
	} else return false;
}

Function* Env::getLastFunction() {
	return m_lastFunc;
}

Variable* Env::lookupVariable(std::string const& name) {
	// Search scopes, starting with innermost.
	return m_currentScope->lookupVariable(name);
}

bool Env::registerVariable(Variable* variable) {
	return m_currentScope->registerVariable(variable);
}

Scope* Env::enterNestedScope() {
	return (m_currentScope = m_currentScope->createChildScope());
}

void Env::setCurrentScope(Scope* scope) {
	m_currentScope = scope;
}

Scope* Env::exitNestedScope() {
	if(m_currentScope != nullptr) {
		return (m_currentScope = m_currentScope->getParentScope());
	} else return nullptr;
}


Scope* Env::getBlockScope(SBlock* sblock) {
	for(auto child : m_currentScope->getChildScopes()) {
		if(child->getBlockStatement() == sblock)
			return child;
	}
	return nullptr;
}

Env::~Env() {
	delete m_globalScope; // Deletes all child scopes as well.
	m_globalScope = m_currentScope = nullptr;

	for(auto& func : m_funcs) {
		delete func.second;
	}
	m_funcs.clear();
}

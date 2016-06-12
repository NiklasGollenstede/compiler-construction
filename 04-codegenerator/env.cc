#include "env.h"
#include "type_checker.h"
#include "cpp.build/Absyn.H"

Env::Env() 
	: m_scopes({ new Scope() }), 
	  m_temp(nullptr) { 
}

Function* Env::lookupFunction(std::string const& name) {
	if(m_funcs.count(name)) return &m_funcs[name];
	else return nullptr;
}

bool Env::registerFunction(Function const& function) {
	if(lookupFunction(function.name) == nullptr) {
		m_funcs[function.name] = function;
		m_lastFunc = function;
		return true;
	} else return false;
}

Function& Env::getLastFunction() {
	return m_lastFunc;
}

Variable* Env::lookupVariable(std::string const& name) {
	Variable* result = nullptr;

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
		delete m_scopes.back();
		m_scopes.pop_back();
		return true;
	} else return false;
}

Env::~Env() {
	// Pop function and block scopes.
	while(popScope());

	// Delete main scope.
	delete m_scopes.front();
	m_scopes.clear();
}

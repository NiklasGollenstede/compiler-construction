#include "scope.h"

Scope* Scope::createGlobalScope() {
	return new Scope(nullptr);
}

Scope::Scope(Scope* parent)
	:m_parent(parent) {
}

Variable* Scope::lookupVariable(std::string const& name) {
	if(m_vars.count(name) == 1) {
		return m_vars[name];
	} else if(m_parent != nullptr) {
		return m_parent->lookupVariable(name);
	} else return nullptr;
}

bool Scope::registerVariable(Variable* variable) {
	if(m_vars.count(variable->name) == 0) {
		std::cout << "Registered variable " << variable->name << " in scope " << this << "." << std::endl;
		m_vars[variable->name] = variable;
		return true;
	} else {
		std::cout << "Variable " << variable->name << " already exists in scope." << std::endl;
		return false;
	}
}

Scope* Scope::createChildScope() {
	m_children.push_back(new Scope(this));
	return m_children.back();
}

Scope* Scope::getParentScope() {
	return m_parent;
}

SBlock* Scope::getBlockStatement() {
	return m_blockStatement;
}

void Scope::setBlockStatement(SBlock* sblock) {
	m_blockStatement = sblock;
}

Scope::~Scope() {
	for(auto& var : m_vars) {
		delete var.second;
	}
	m_vars.clear();

	for(auto& scope : m_children) {
		delete scope;
	}
	m_children.clear();
}
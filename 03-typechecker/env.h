#ifndef ENV_H
#define ENV_H

#include "common.h"
#include "scope.h"
#include "function.h"

class Visitable;
class Visitor;

class Env {
private:
	std::map<std::string, Function> m_funcs;
	std::vector<Scope*> m_scopes;
	void* m_temp;

public:
	Env();

	Function const* lookupFunction(std::string const& name);
	bool registerFunction(Function const& function);

	//
	Variable const* lookupVariable(std::string const& name);
	bool registerVariable(Variable const& variable);

	// Modify scope stack.
	void pushScope();
	bool popScope();

	void setTemp(void* temp);
	void* getTemp();
	void* visit(Visitable* v, Visitor* checker);

	~Env();
};

#endif

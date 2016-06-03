#ifndef ENV_H
#define ENV_H

#include "common.h"
#include "scope.h"
#include "function.h"

#include "cpp.build/Absyn.H"
#include "type_checker.h"

class Env {
private:
	std::map<std::string, Function> m_funcs;
	Function m_lastFunc;
	std::vector<Scope*> m_scopes;
	void* m_temp;

public:
	Env();

	Function const* lookupFunction(std::string const& name);
	bool registerFunction(Function const& function);
	Function const& getLastFunction();

	//
	Variable const* lookupVariable(std::string const& name);
	bool registerVariable(Variable const& variable);

	// Modify scope stack.
	void pushScope();
	bool popScope();

	template<typename T>
	void setTemp(T* temp) {
		m_temp = (void*)temp;
	}

	template<typename T>
	bool getTemp(T* out) {
		if(m_temp == nullptr) {
			return false;
		} else {
			*out = *(T*)m_temp;
			delete (T*)m_temp;
			setTemp<void>(nullptr);
			return true;
		}
	}

	// Returns value of getTemp after the node was visited.
	template<typename T>
	bool visit(Visitable* v, TypeChecker* checker, T* out) {
		v->accept(checker);
		return getTemp<T>(out);
	}

	inline void visit(Visitable* v, TypeChecker* checker) {
		v->accept(checker);
	}

	~Env();
};

#endif

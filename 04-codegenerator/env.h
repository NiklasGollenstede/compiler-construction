#ifndef ENV_H
#define ENV_H

/**
 * File:      env.h
 * -------------------------------
 * Author(s): GOLLENSTEDE, Niklas
 *            HAUSWALD, Tom.
 * -------------------------------
 * Modified:  June 09, 2016.
 * -------------------------------
 *
 * This file contains the declaration
 * of the environment class 'env' used
 * to support the typechecking process.
 **/

#include "common.h"
#include "scope.h"
#include "function.h"

#include "cpp.build/Absyn.H"
#include "type_checker.h"

// The env (environment) class holds information about all construct defined
// inside the program to typecheck. This includes functions and variables,
// as well as their corresponding child- or implicit scopes.
class Env {

private:
	// Map of functions by name.
	// Each function name may only be used for one definition.
	std::map<std::string, Function> m_funcs;

	// Copy of last defined function object.
	Function m_lastFunc;

	// List of scopes in order of their creation.
	std::vector<Scope*> m_scopes;

	// Temporary buffer.
	void* m_temp;

public:
	// Initialize the environment instance.
	Env();

	// Returns the function that matches the specified name or
	// nullptr if no such function was defined.
	Function const* lookupFunction(std::string const& name);

	// Registers a new function.
	// Returns true on success or false, if a function
	// of the specified name already exists.
	bool registerFunction(Function const& function);

	// Returns the last function defined.
	Function const& getLastFunction();

	// Returns the variable declared in the tightest scope to the
	// caller's context or nullptr if no such is defined.
	Variable const* lookupVariable(std::string const& name);

	// Registers a new variable to the current scope.
	// Returns true on success or false, if a variable definition
	// was already supplied inside the same scope.
	bool registerVariable(Variable const& variable);

	// Create a new scope inside the current one.
	void pushScope();

	// Pops the current scope from the stack.
	// Returns true on success or false, if no function-
	// or block-level scope is stored on the stack.
	// The main scope cannot be popped.
	bool popScope();

	// Stores the specified value in the temporary buffer.
	template<typename T>
	void setTemp(T* temp) {
		m_temp = (void*)temp;
	}

	// Stores the value of the temporary buffer inside the
	// specified 'out' variable.
	// Returns true on success or false, if temp was empty.
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
	void visit(Visitable* v, Visitor* vis, T* out) {
		v->accept(vis);
		if(!getTemp<T>(out)) {
			crash();
		}
	}

	// Visit a node that does not push a value to temp.
	inline void visit(Visitable* v, Visitor* vis) {
		v->accept(vis);
	}

	// Delete remaining scopes.
	~Env();
};

#endif

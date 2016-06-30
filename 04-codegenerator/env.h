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
	std::map<std::string, Function*> m_funcs;

	// Copy of last defined function object.
	Function* m_lastFunc;

	// Global scope.
	Scope* m_globalScope;

	// Current scope.
	Scope* m_currentScope;

	// Temporary buffer.
	void* m_temp;

public:
	// Initialize the environment instance.
	Env();

	// Returns the function that matches the specified name or
	// nullptr if no such function was defined.
	Function* lookupFunction(std::string const& name);

	// Registers a new function.
	// Returns true on success or false, if a function
	// of the specified name already exists.
	bool registerFunction(Function* function);

	// Returns the last function defined.
	Function* getLastFunction();

	void setLastFunction(Function* function);

	// Returns the variable declared in the tightest scope to the
	// caller's context or nullptr if no such is defined.
	Variable* lookupVariable(std::string const& name);

	// Registers a new variable to the current scope.
	// Returns true on success or false, if a variable definition
	// was already supplied inside the same scope.
	bool registerVariable(Variable* variable);

	// Create a new scope inside the current one.
	Scope* enterNestedScope();

	// Pops the current scope from the stack.
	// Returns true on success or false, if no function-
	// or block-level scope is stored on the stack.
	// The main scope cannot be popped.
	Scope* exitNestedScope();

	Scope* getCurrentScope() { return m_currentScope; }

	void setCurrentScope(Scope* scope);
	
	Scope* getBlockScope(SBlock* sblock);

	// Stores the specified value in the temporary buffer.
	template<typename T>
	void setTemp(T* temp) {
		m_temp = (void*)temp;
	}

	// Returns the value of temp.
	template<typename T>
	T* getTemp() {
		return (T*)m_temp;
	}

	// Returns the value of temp after the node was visited.
	template<typename T>
	T* visit(Visitable* v, Visitor* vis) {
		v->accept(vis);
		return getTemp<T>();
	}

	// Delete remaining scopes.
	~Env();
};

#endif

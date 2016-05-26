#ifndef ENV_H
#define ENV_H

#include <string>
#include <vector>
#include <map>
#include <cstdint>
#include <exception>

enum class Datatype {
	Bool,
	Int,
	Double,
	Void,
	String,
	Undefined
};

class Datatypes {
public:
	static inline std::string get(Datatype type) {
		switch(type) {
			case Datatype::Bool:      return "bool";
			case Datatype::Int:       return "int";
			case Datatype::Double:    return "double";
			case Datatype::Void:      return "void";
			case Datatype::String:    return "string";
			default: throw std::exception();
		}
	}

	static inline Datatype get(std::string const& name) {
		if(name == "bool")   return Datatype::Bool;
		if(name == "int")    return Datatype::Int;
		if(name == "double") return Datatype::Double;
		if(name == "void")   return Datatype::Void;
		if(name == "string") return Datatype::String;
		return Datatype::Undefined;
	}
};

struct Variable {
	Datatype type;
	std::string name;
};

struct Function {
	Datatype returnType;
	std::string name;
	std::vector<Variable> args;
};

class Scope {
private:
	std::map<std::string, Variable> m_vars;

public:
	// Returns the variable or nullptr.
	Variable const *lookupVariable(std::string const& name);

	// Returns false if name already exists.
	bool registerVariable(Variable const& variable);
};

class Env {
private:
	std::map<std::string, Function> m_funcs;
	std::vector<Scope *> m_scopes;

public:
	Env();

	Function const *lookupFunction(std::string const& name);
	bool registerFunction(Function const& function);

	//
	Variable const *lookupVariable(std::string const& name);
	bool registerVariable(Variable const& variable);

	// Modify scope stack.
	void pushScope();
	bool popScope();

	~Env();
};

#endif

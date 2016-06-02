#ifndef DATATYPE_H
#define DATATYPE_H

#include <string>

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

#endif
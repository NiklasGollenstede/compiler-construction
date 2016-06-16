#ifndef VARIABLE_H
#define VARIABLE_H

#include "common.h"
#include "datatype.h"

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"

struct Variable {
	Variable(
		Datatype* type, 
		std::string const& name,
		llvm::Value* value
	);

	Datatype* type;
	std::string name;
	llvm::Value* value;

	~Variable();
};

#endif
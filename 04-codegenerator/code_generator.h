#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H

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

#include "common.h"
#include "env.h"

class CodeGenerator : public Visitor {
private:
	// Environment passed on by typechecker.
	Env* m_env;

	llvm::Value* m_tempValue;

	llvm::LLVMContext m_context;
	llvm::IRBuilder<> m_builder;
	llvm::Module* m_module;

	llvm::Type* convertType(Datatype type);

	// Visit functions.
	void visit(Visitable* v);
	void visit(Visitable* v, llvm::Value** value);
	void store(llvm::Value* value);

public:
	CodeGenerator(std::string const& moduleName, Env* env);

	void printModule();

	// Auto-generated.
	void visitProgram(Program* p);
	void visitDef(Def* p);
	void visitArg(Arg* p);
	void visitStm(Stm* p);
	void visitExp(Exp* p);
	void visitType(Type* p);
	void visitPDefs(PDefs* p);
	void visitDFun(DFun* p);
	void visitADecl(ADecl* p);
	void visitSExp(SExp* p);
	void visitSDecls(SDecls* p);
	void visitSInit(SInit* p);
	void visitSReturn(SReturn* p);
	void visitSReturnVoid(SReturnVoid* p);
	void visitSWhile(SWhile* p);
	void visitSBlock(SBlock* p);
	void visitSIfElse(SIfElse* p);
	void visitETrue(ETrue* p);
	void visitEFalse(EFalse* p);
	void visitEInt(EInt* p);
	void visitEDouble(EDouble* p);
	void visitEString(EString* p);
	void visitEId(EId* p);
	void visitEApp(EApp* p);
	void visitEPIncr(EPIncr* p);
	void visitEPDecr(EPDecr* p);
	void visitEIncr(EIncr* p);
	void visitEDecr(EDecr* p);
	void visitETimes(ETimes* p);
	void visitEDiv(EDiv* p);
	void visitEPlus(EPlus* p);
	void visitEMinus(EMinus* p);
	void visitELt(ELt* p);
	void visitEGt(EGt* p);
	void visitELtEq(ELtEq* p);
	void visitEGtEq(EGtEq* p);
	void visitEEq(EEq* p);
	void visitENEq(ENEq* p);
	void visitEAnd(EAnd* p);
	void visitEOr(EOr* p);
	void visitEAss(EAss* p);
	void visitETyped(ETyped* p);
	void visitType_bool(Type_bool* p);
	void visitType_int(Type_int* p);
	void visitType_double(Type_double* p);
	void visitType_void(Type_void* p);
	void visitType_string(Type_string* p);
	void visitListDef(ListDef* p);
	void visitListArg(ListArg* p);
	void visitListStm(ListStm* p);
	void visitListExp(ListExp* p);
	void visitListId(ListId* p);

	void visitId(Id x);
	void visitInteger(Integer x);
	void visitChar(Char x);
	void visitDouble(Double x);
	void visitString(String x);
	void visitIdent(Ident x);

	virtual ~CodeGenerator();
};

#endif
#include "llvm_ir_gen.h"

LLVMIRGen::LLVMIRGen(std::string const& moduleName, Env* env)
  :m_env(env),
   m_builder(m_context),
   m_module(new llvm::Module(moduleName, m_context)) {
}

llvm::Type* LLVMIRGen::convertType(Datatype type) {
  switch(type) {
    case Datatype::Void:   return llvm::Type::getVoidTy(m_context);
    case Datatype::Bool:   return llvm::Type::getInt1Ty(m_context);
    case Datatype::Int:    return llvm::Type::getInt32Ty(m_context);
    case Datatype::Double: return llvm::Type::getDoubleTy(m_context);
    case Datatype::String: return llvm::Type::getInt8PtrTy(m_context);
  }
}

void LLVMIRGen::visit(Visitable* v) {
  v->accept(this);
}

void LLVMIRGen::visit(Visitable* v, llvm::Value** value) {
  v->accept(this);
  *value = m_tempValue;
}

void LLVMIRGen::store(llvm::Value* value) {
  m_tempValue = value;
}

void LLVMIRGen::printModule() {
  m_module->dump();
}

void LLVMIRGen::visitProgram(Program* t) { 
  visitPDefs((PDefs *)t); 
}

void LLVMIRGen::visitDef(Def* t) {} //abstract class
void LLVMIRGen::visitArg(Arg* t) {} //abstract class
void LLVMIRGen::visitStm(Stm* t) {} //abstract class
void LLVMIRGen::visitExp(Exp* t) {} //abstract class
void LLVMIRGen::visitType(Type* t) {} // abstract class

void LLVMIRGen::visitPDefs(PDefs *pdefs) {
  visit(pdefs->listdef_);
}

void LLVMIRGen::visitDFun(DFun *dfun) {
  auto func = m_env->lookupFunction(dfun->id_);

  // Construct argument types list.
  std::vector<llvm::Type*> argumentTypes;
  for(auto argument : func->args) {
    argumentTypes.push_back(convertType(argument.type));
  }

  // Create function type.
  auto func_type = llvm::FunctionType::get(
    convertType(func->returnType),
    argumentTypes,
    false);

  auto llvm_func = llvm::Function::Create(
    func_type, 
    llvm::Function::ExternalLinkage, 
    func->name, 
    m_module);

  int arg_id = 0;
  for(auto& llvm_arg : llvm_func->args()) {
    llvm_arg.setName(func->args[arg_id++].name);
  }

  auto entry = llvm::BasicBlock::Create(
    m_context, 
    "entry", 
    llvm_func);
  m_builder.SetInsertPoint(entry);

  visit(dfun->liststm_);
}

void LLVMIRGen::visitADecl(ADecl *adecl) {
}

void LLVMIRGen::visitSExp(SExp *sexp) {
}

void LLVMIRGen::visitSDecls(SDecls *sdecls) {
}

void LLVMIRGen::visitSInit(SInit *sinit) {
}

void LLVMIRGen::visitSReturn(SReturn *sreturn) {
  llvm::Value* retval;
  visit(sreturn->exp_, &retval);
  store(m_builder.CreateRet(retval));
}

void LLVMIRGen::visitSReturnVoid(SReturnVoid *sreturnvoid) {
}

void LLVMIRGen::visitSWhile(SWhile *swhile) {
}

void LLVMIRGen::visitSBlock(SBlock *sblock) {
  visit(sblock->liststm_);
}

void LLVMIRGen::visitSIfElse(SIfElse *sifelse) {
}

void LLVMIRGen::visitETrue(ETrue *etrue) {
  store(llvm::ConstantInt::getTrue(m_context));
}

void LLVMIRGen::visitEFalse(EFalse *efalse) {
  store(llvm::ConstantInt::getFalse(m_context));
}

void LLVMIRGen::visitEInt(EInt *eint) {
  store(llvm::ConstantInt::get(m_context, llvm::APInt(32, eint->integer_)));
}

void LLVMIRGen::visitEDouble(EDouble *edouble) {
  store(llvm::ConstantFP::get(m_context, llvm::APFloat(edouble->double_)));
}

void LLVMIRGen::visitEString(EString *estring) {
}

void LLVMIRGen::visitEId(EId *eid) {
  auto var = m_env->lookupVariable(eid->id_);
}

void LLVMIRGen::visitEApp(EApp *eapp) {
  auto func = m_env->lookupFunction(eapp->id_);
}

void LLVMIRGen::visitEPIncr(EPIncr *epincr) {
  llvm::Value* value;
  visit(epincr->exp_, &value);
  llvm::Value* one = llvm::ConstantInt::get(m_context, llvm::APInt(1, 32));
  store(m_builder.CreateFAdd(value, one));
}

void LLVMIRGen::visitEPDecr(EPDecr *epdecr) {
  llvm::Value* value;
  visit(epdecr->exp_, &value);
  llvm::Value* one = llvm::ConstantInt::get(m_context, llvm::APInt(1, 32));
  store(m_builder.CreateFSub(value, one));
}

void LLVMIRGen::visitEIncr(EIncr *eincr) {
  llvm::Value* value;
  visit(eincr->exp_, &value);
  llvm::Value* one = llvm::ConstantInt::get(m_context, llvm::APInt(1, 32));
  store(m_builder.CreateFSub(value, one));
}

void LLVMIRGen::visitEDecr(EDecr *edecr) {
  llvm::Value* value;
  visit(edecr->exp_, &value);
  llvm::Value* one = llvm::ConstantInt::get(m_context, llvm::APInt(1, 32));
  store(m_builder.CreateFSub(value, one));
}

void LLVMIRGen::visitETimes(ETimes *etimes) {
  llvm::Value* lhs;
  llvm::Value* rhs;
  visit(etimes->exp_1, &lhs);
  visit(etimes->exp_2, &rhs);
  store(m_builder.CreateFMul(lhs, rhs));
}

void LLVMIRGen::visitEDiv(EDiv *ediv) {
  llvm::Value* lhs;
  llvm::Value* rhs;
  visit(ediv->exp_1, &lhs);
  visit(ediv->exp_2, &rhs);
  store(m_builder.CreateFDiv(lhs, rhs));
}

void LLVMIRGen::visitEPlus(EPlus *eplus) {
  llvm::Value* lhs;
  llvm::Value* rhs;
  visit(eplus->exp_1, &lhs);
  visit(eplus->exp_2, &rhs);
  store(m_builder.CreateFAdd(lhs, rhs));
}

void LLVMIRGen::visitEMinus(EMinus *eminus) {
  llvm::Value* lhs;
  llvm::Value* rhs;
  visit(eminus->exp_1, &lhs);
  visit(eminus->exp_2, &rhs);
  store(m_builder.CreateFSub(lhs, rhs));
}

void LLVMIRGen::visitELt(ELt *elt) {
}

void LLVMIRGen::visitEGt(EGt *egt) {
}

void LLVMIRGen::visitELtEq(ELtEq *elteq) {
}

void LLVMIRGen::visitEGtEq(EGtEq *egteq) {
}

void LLVMIRGen::visitEEq(EEq *eeq) {
}

void LLVMIRGen::visitENEq(ENEq *eneq) {
}

void LLVMIRGen::visitEAnd(EAnd *eand) {
}

void LLVMIRGen::visitEOr(EOr *eor) {
}

void LLVMIRGen::visitEAss(EAss *eass) {
}

void LLVMIRGen::visitETyped(ETyped *etyped) {
}

void LLVMIRGen::visitType_bool(Type_bool *type_bool) {
}

void LLVMIRGen::visitType_int(Type_int *type_int) {
}

void LLVMIRGen::visitType_double(Type_double *type_double) {
}

void LLVMIRGen::visitType_void(Type_void *type_void) {
}

void LLVMIRGen::visitType_string(Type_string *type_string) {
}

void LLVMIRGen::visitListDef(ListDef* listdef) {
  for(auto def : *listdef) {
    visit(def);
  }
}

void LLVMIRGen::visitListArg(ListArg* listarg) {
  for(auto arg : *listarg) {
    visit(arg);
  }
}

void LLVMIRGen::visitListStm(ListStm* liststm) {
  for(auto stm : *liststm) {
    visit(stm);
  }
}

void LLVMIRGen::visitListExp(ListExp* listexp) {
  for(auto exp : *listexp) {
    visit(exp);
  }
}

void LLVMIRGen::visitListId(ListId* listid) {
  for(auto id : *listid) {
    visitId(id);
  }
}

void LLVMIRGen::visitId(Id x) {
}

void LLVMIRGen::visitInteger(Integer x) {
}

void LLVMIRGen::visitChar(Char x) {
}

void LLVMIRGen::visitDouble(Double x) {
}

void LLVMIRGen::visitString(String x) {
}

void LLVMIRGen::visitIdent(Ident x) {
}

LLVMIRGen::~LLVMIRGen() {
  delete m_module;
  m_module = nullptr;
}
#include "code_generator.h"

CodeGenerator::CodeGenerator(std::string const& moduleName, Env* env)
  :m_env(env),
   m_builder(m_context),
   m_module(new llvm::Module(moduleName, m_context)) {
}

llvm::Type* CodeGenerator::convertType(Datatype type) {
  switch(type) {
    case Datatype::Void:   return llvm::Type::getVoidTy(m_context);
    case Datatype::Bool:   return llvm::Type::getInt1Ty(m_context);
    case Datatype::Int:    return llvm::Type::getInt32Ty(m_context);
    case Datatype::Double: return llvm::Type::getDoubleTy(m_context);
    case Datatype::String: error("String type not supported.");
    default:               return nullptr;
  }
}

/*
llvm::Type* CodeGenerator::convertType(Datatype type) {
  switch(type) {
    case Datatype::Void: 
      return llvm::Type::getVoidTy(m_context);
    
    case Datatype::Bool:
    case Datatype::Int:
    case Datatype::Double: 
      return llvm::Type::getDoubleTy(m_context);
    
    case Datatype::String:
      return llvm::Type::getInt8PtrTy(m_context);

    default:
      return nullptr;
  }
}
*/

void CodeGenerator::printModule() {
  m_module->dump();
}

void CodeGenerator::visitProgram(Program* t) { 
  visitPDefs((PDefs *)t); 
}

void CodeGenerator::visitDef(Def* t) {} //abstract class
void CodeGenerator::visitArg(Arg* t) {} //abstract class
void CodeGenerator::visitStm(Stm* t) {} //abstract class
void CodeGenerator::visitExp(Exp* t) {} //abstract class
void CodeGenerator::visitType(Type* t) {} // abstract class

void CodeGenerator::visitPDefs(PDefs *pdefs) {
  m_env->visit<void>(pdefs->listdef_, this);
}

void CodeGenerator::createVariableAllocation(Variable* var, llvm::Value* value) {
  var->value = m_builder.CreateAlloca(convertType(*var->type), nullptr, var->name);
  if(value != nullptr) {
    m_builder.CreateStore(value, var->value); 
  }
}

void CodeGenerator::visitDFun(DFun *dfun) {
  auto func = m_env->lookupFunction(dfun->id_);

  // Enter function scope.
  m_env->setCurrentScope(func->scope);

  // Construct argument types list.
  std::vector<llvm::Type*> argumentTypes;
  for(auto argument : *func->args) {
    argumentTypes.push_back(convertType(*argument->type));
  }

  // Create function type.
  auto func_type = llvm::FunctionType::get(
    convertType(*func->returnType),
    argumentTypes,
    false);

  // Create function.
  auto llvm_func = llvm::Function::Create(
    func_type, 
    llvm::Function::ExternalLinkage, 
    func->name, 
    m_module);

  func->llvmHandle = llvm_func;

  // Create entry basic block.
  auto entry = llvm::BasicBlock::Create(
    m_context, 
    "entry", 
    llvm_func);
  m_builder.SetInsertPoint(entry);

  // Initialize arguments.
  int argumentIndex = 0;
  for(auto& llvmFnArgument : llvm_func->args()) {

    auto localFnArgument = (*func->args)[argumentIndex];
    llvmFnArgument.setName(localFnArgument->name);
    createVariableAllocation(localFnArgument, &llvmFnArgument);
    
    ++argumentIndex;
  }

  // Visit child nodes.
  m_env->visit<void>(dfun->liststm_, this);

  llvm::verifyFunction(*func->llvmHandle);

  // Leave function scope.
  m_env->setCurrentScope(func->scope->getParentScope());
}

void CodeGenerator::visitADecl(ADecl *adecl) {
  // Purposely ignored.
}

void CodeGenerator::visitSExp(SExp *sexp) {
  m_env->visit<void>(sexp->exp_, this);
}

void CodeGenerator::visitSDecls(SDecls *sdecls) {
  auto names = m_env->visit<std::vector<std::string>>(sdecls->listid_, this);

  for(auto const& name : *names) {
    auto var = m_env->lookupVariable(name);
    createVariableAllocation(var, nullptr);
  }
}

void CodeGenerator::visitSInit(SInit *sinit) {
  auto name = sinit->id_;
  auto var  = m_env->lookupVariable(name);
  createVariableAllocation(var, m_env->visit<llvm::Value>(sinit->exp_, this));
}

void CodeGenerator::visitSReturn(SReturn *sreturn) {
  auto retval = m_env->visit<llvm::Value>(sreturn->exp_, this);
  m_env->setTemp(m_builder.CreateRet(retval));
}

void CodeGenerator::visitSReturnVoid(SReturnVoid *sreturnvoid) {
  auto retval = m_builder.CreateRetVoid();
}

void CodeGenerator::visitSWhile(SWhile *swhile) {
  auto condexpr = m_env->visit<llvm::Value>(swhile->exp_, this);
}

void CodeGenerator::visitSBlock(SBlock *sblock) {
  m_env->setCurrentScope(m_env->getBlockScope(sblock));
  m_env->visit<void>(sblock->liststm_, this);
  m_env->exitNestedScope();
}

void CodeGenerator::visitSIfElse(SIfElse *sifelse) {
  auto condexpr = m_env->visit<llvm::Value>(sifelse->exp_,  this);

  auto llvm_func  = m_env->getLastFunction()->llvmHandle;
  auto trueblock  = llvm::BasicBlock::Create(m_context, "then", llvm_func);
  auto falseblock = llvm::BasicBlock::Create(m_context, "else", llvm_func);
  auto mergeblock = llvm::BasicBlock::Create(m_context, "join", llvm_func);

  m_builder.CreateCondBr(condexpr, trueblock, falseblock);

  m_builder.SetInsertPoint(trueblock);
  m_env->visit<void>(sifelse->stm_1, this); 
  m_builder.CreateBr(mergeblock);

  m_builder.SetInsertPoint(falseblock);
  m_env->visit<void>(sifelse->stm_2, this);
  m_builder.CreateBr(mergeblock);

  m_builder.SetInsertPoint(mergeblock);
}

void CodeGenerator::visitETrue(ETrue *etrue) {
  m_env->setTemp(llvm::ConstantInt::getTrue(m_context));
}

void CodeGenerator::visitEFalse(EFalse *efalse) {
  m_env->setTemp(llvm::ConstantInt::getFalse(m_context));
}

void CodeGenerator::visitEInt(EInt *eint) {
  m_env->setTemp(llvm::ConstantInt::get(m_context, llvm::APInt(32, eint->integer_)));
}

void CodeGenerator::visitEDouble(EDouble *edouble) {
  m_env->setTemp(llvm::ConstantFP::get(m_context, llvm::APFloat(edouble->double_)));
}

void CodeGenerator::visitEString(EString *estring) {
}

void CodeGenerator::visitEId(EId *eid) {
  auto var = m_env->lookupVariable(eid->id_);
  m_env->setTemp(var->value);
}

void CodeGenerator::visitEApp(EApp *eapp) {
  auto func = m_env->lookupFunction(eapp->id_);

  auto argumentValues = m_env->visit<std::vector<llvm::Value*>>(eapp->listexp_, this);
  m_env->setTemp(m_builder.CreateCall(func->llvmHandle, *argumentValues));  
  delete argumentValues;
}

void CodeGenerator::visitEPIncr(EPIncr *epincr) {
  auto value = m_env->visit<llvm::Value>(epincr->exp_, this);
  auto one = llvm::ConstantInt::get(m_context, llvm::APInt(1, 32));
  m_env->setTemp(m_builder.CreateAdd(value, one));
}

void CodeGenerator::visitEPDecr(EPDecr *epdecr) {
  auto value = m_env->visit<llvm::Value>(epdecr->exp_, this);
  auto one = llvm::ConstantInt::get(m_context, llvm::APInt(1, 32));
  m_env->setTemp(m_builder.CreateSub(value, one));
}

void CodeGenerator::visitEIncr(EIncr *eincr) {
  auto value = m_env->visit<llvm::Value>(eincr->exp_, this);
  auto one = llvm::ConstantInt::get(m_context, llvm::APInt(1, 32));
  m_env->setTemp(m_builder.CreateAdd(value, one));
}

void CodeGenerator::visitEDecr(EDecr *edecr) {
  auto value = m_env->visit<llvm::Value>(edecr->exp_, this);
  auto one = llvm::ConstantInt::get(m_context, llvm::APInt(1, 32));
  m_env->setTemp(m_builder.CreateSub(value, one));
}

void CodeGenerator::visitETimes(ETimes *etimes) {
  auto lhs = m_env->visit<llvm::Value>(etimes->exp_1, this);
  auto rhs = m_env->visit<llvm::Value>(etimes->exp_2, this);
  m_env->setTemp(m_builder.CreateMul(lhs, rhs));
}

void CodeGenerator::visitEDiv(EDiv *ediv) {
  auto lhs = m_env->visit<llvm::Value>(ediv->exp_1, this);
  auto rhs = m_env->visit<llvm::Value>(ediv->exp_2, this);
  m_env->setTemp(m_builder.CreateMul(lhs, rhs));
}

void CodeGenerator::visitEPlus(EPlus *eplus) {
  auto lhs = m_env->visit<llvm::Value>(eplus->exp_1, this);
  auto rhs = m_env->visit<llvm::Value>(eplus->exp_2, this);
  m_env->setTemp(m_builder.CreateAdd(lhs, rhs));
}

void CodeGenerator::visitEMinus(EMinus *eminus) {
  auto lhs = m_env->visit<llvm::Value>(eminus->exp_1, this);
  auto rhs = m_env->visit<llvm::Value>(eminus->exp_2, this);
  m_env->setTemp(m_builder.CreateSub(lhs, rhs));
}

void CodeGenerator::visitELt(ELt *elt) {
  auto lhs = m_env->visit<llvm::Value>(elt->exp_1, this);
  auto rhs = m_env->visit<llvm::Value>(elt->exp_2, this);
  m_env->setTemp(m_builder.CreateFCmpOLT(lhs, rhs));
}

void CodeGenerator::visitEGt(EGt *egt) {
  auto lhs = m_env->visit<llvm::Value>(egt->exp_1, this);
  auto rhs = m_env->visit<llvm::Value>(egt->exp_2, this);
  m_env->setTemp(m_builder.CreateFCmpOGT(lhs, rhs));
}

void CodeGenerator::visitELtEq(ELtEq *elteq) {
  auto lhs = m_env->visit<llvm::Value>(elteq->exp_1, this);
  auto rhs = m_env->visit<llvm::Value>(elteq->exp_2, this);
  m_env->setTemp(m_builder.CreateFCmpOLE(lhs, rhs));
}

void CodeGenerator::visitEGtEq(EGtEq *egteq) {
  auto lhs = m_env->visit<llvm::Value>(egteq->exp_1, this);
  auto rhs = m_env->visit<llvm::Value>(egteq->exp_2, this);
  m_env->setTemp(m_builder.CreateFCmpOGE(lhs, rhs));
}

void CodeGenerator::visitEEq(EEq *eeq) {
  auto lhs = m_env->visit<llvm::Value>(eeq->exp_1, this);
  auto rhs = m_env->visit<llvm::Value>(eeq->exp_2, this);
  m_env->setTemp(m_builder.CreateFCmpUEQ(lhs, rhs));
}

void CodeGenerator::visitENEq(ENEq *eneq) {
  auto lhs = m_env->visit<llvm::Value>(eneq->exp_1, this);
  auto rhs = m_env->visit<llvm::Value>(eneq->exp_2, this);
  m_env->setTemp(m_builder.CreateFCmpUNE(lhs, rhs));
}

void CodeGenerator::visitEAnd(EAnd *eand) {
  auto lhs = m_env->visit<llvm::Value>(eand->exp_1, this);
  auto rhs = m_env->visit<llvm::Value>(eand->exp_2, this);
  m_env->setTemp(m_builder.CreateAnd(lhs, rhs));
}

void CodeGenerator::visitEOr(EOr *eor) {
  auto lhs = m_env->visit<llvm::Value>(eor->exp_1, this);
  auto rhs = m_env->visit<llvm::Value>(eor->exp_2, this);
  m_env->setTemp(m_builder.CreateOr(lhs, rhs));
}

void CodeGenerator::visitEAss(EAss *eass) {
  auto varValue = m_env->lookupVariable(
    ((EId*)(eass->exp_1))->id_
  )->value;

  auto assignedValue = m_env->visit<llvm::Value>(eass->exp_2, this);
  m_env->setTemp(m_builder.CreateStore(assignedValue, varValue));
}

void CodeGenerator::visitETyped(ETyped *etyped) {
}

void CodeGenerator::visitType_bool(Type_bool *type_bool) {
}

void CodeGenerator::visitType_int(Type_int *type_int) {
}

void CodeGenerator::visitType_double(Type_double *type_double) {
}

void CodeGenerator::visitType_void(Type_void *type_void) {
}

void CodeGenerator::visitType_string(Type_string *type_string) {
}

void CodeGenerator::visitListDef(ListDef* listdef) {
  for(auto def : *listdef) {
    m_env->visit<void>(def, this);
  }
}

void CodeGenerator::visitListArg(ListArg* listarg) {
  for(auto arg : *listarg) {
    m_env->visit<void>(arg, this);
  }
}

void CodeGenerator::visitListStm(ListStm* liststm) {
  for(auto stm : *liststm) {
    m_env->visit<void>(stm, this);
  }
}

void CodeGenerator::visitListExp(ListExp* listexp) {
  auto expValues = new std::vector<llvm::Value*>();
  for(auto exp : *listexp) {
    expValues->push_back(m_env->visit<llvm::Value>(exp, this));
  }
  m_env->setTemp(expValues);
}

void CodeGenerator::visitListId(ListId* listid) {
}

void CodeGenerator::visitId(Id x) {
}

void CodeGenerator::visitInteger(Integer x) {
}

void CodeGenerator::visitChar(Char x) {
}

void CodeGenerator::visitDouble(Double x) {
}

void CodeGenerator::visitString(String x) {
}

void CodeGenerator::visitIdent(Ident x) {
}

CodeGenerator::~CodeGenerator() {
  delete m_module;
  m_module = nullptr;
}
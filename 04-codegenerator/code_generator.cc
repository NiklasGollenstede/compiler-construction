#include "code_generator.h"
// #include <llvm/Bitcode/BitcodeWriter.h>

#define HAS_TYPE_INT(x) ((x)->getType() == convertType(Datatype::Int))

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

void CodeGenerator::printModule() {
  m_module->dump();
}

void CodeGenerator::saveToFile(std::string path) {
  std::error_code err;
  llvm::raw_fd_ostream file(path, err, llvm::sys::fs::OpenFlags::F_Text);
  m_module->print(file, nullptr);
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
  std::cout << "Creating variable allocation for var ptr: " << var << std::endl;
  var->value = m_builder.CreateAlloca(convertType(*var->type), nullptr, var->name);
  if(value != nullptr) {
    m_builder.CreateStore(value, var->value);
  }
}

void CodeGenerator::visitDFun(DFun *dfun) {
  auto func = m_env->lookupFunction(dfun->id_);
  m_env->setLastFunction(func);

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
  if (*(func->returnType) == Datatype::Void) {
    m_builder.CreateRetVoid();
  }

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
  std::cout << "visitSDecls(";
  auto names = m_env->visit<std::vector<std::string>>(sdecls->listid_, this);
  for(auto const& name : *names) { std::cout << name << ((name == names->back()) ? "" : ", "); }
  std::cout << ")" << std::endl;

  for(auto const& name : *names) {
    auto var = m_env->lookupVariable(name);
    createVariableAllocation(var, nullptr);
  }

  delete names;
}

void CodeGenerator::visitSInit(SInit *sinit) {
  std::cout << "visitSInit(" << sinit->id_ << ")" << std::endl;
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
  auto llvm_func  = m_env->getLastFunction()->llvmHandle;
  auto condblock  = llvm::BasicBlock::Create(m_context, "cond", llvm_func);
  auto loopblock  = llvm::BasicBlock::Create(m_context, "loop", llvm_func);
  auto mergeblock = llvm::BasicBlock::Create(m_context, "join", llvm_func);

  m_builder.CreateBr(condblock);
  m_builder.SetInsertPoint(condblock);
  auto condexpr = m_env->visit<llvm::Value>(swhile->exp_, this);
  m_builder.CreateCondBr(condexpr, loopblock, mergeblock);

  m_builder.SetInsertPoint(loopblock);
  m_env->visit<void>(swhile->stm_, this);
  m_builder.CreateBr(condblock);

  m_builder.SetInsertPoint(mergeblock);
}

void CodeGenerator::visitSBlock(SBlock *sblock) {
  m_env->setCurrentScope(m_env->getBlockScope(sblock));
  m_env->visit<void>(sblock->liststm_, this);
  m_env->exitNestedScope();
}

void CodeGenerator::visitSIfElse(SIfElse *sifelse) {
  auto condexpr = m_env->visit<llvm::Value>(sifelse->exp_, this);

  auto llvm_func  = m_env->getLastFunction()->llvmHandle;
  auto trueblock  = llvm::BasicBlock::Create(m_context, "then", llvm_func);
  auto falseblock = llvm::BasicBlock::Create(m_context, "else", llvm_func);
  auto mergeblock = llvm::BasicBlock::Create(m_context, "join", llvm_func);

  m_builder.CreateCondBr(condexpr, trueblock, falseblock);

  m_builder.SetInsertPoint(trueblock);
  m_env->visit<void>(sifelse->stm_1, this);
  m_builder.CreateBr(mergeblock);
  // trueblock = m_builder.GetInsertPoint();

  m_builder.SetInsertPoint(falseblock);
  m_env->visit<void>(sifelse->stm_2, this);
  m_builder.CreateBr(mergeblock);
  // falseblock = m_builder.GetInsertPoint();

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
  // TODO: throw?
}

void CodeGenerator::visitEId(EId *eid) {
  std::cout << "visitEId(" << eid->id_ << ") in scope " << m_env->getCurrentScope() << std::endl;
  auto var = m_env->lookupVariable(eid->id_);
  m_env->setTemp(m_builder.CreateLoad(var->value, var->name));
}

void CodeGenerator::visitEApp(EApp *eapp) {
  auto func = m_env->lookupFunction(eapp->id_);

  auto argumentValues = m_env->visit<std::vector<llvm::Value*>>(eapp->listexp_, this);
  m_env->setTemp(m_builder.CreateCall(func->llvmHandle, *argumentValues));
  delete argumentValues;
}

void CodeGenerator::visitEPIncr(EPIncr *epincr) {
  auto value = m_env->visit<llvm::Value>(epincr->exp_, this);
  auto one = llvm::ConstantInt::get(m_context, llvm::APInt(32, 1));
  m_env->setTemp(m_builder.CreateAdd(value, one));
}

void CodeGenerator::visitEPDecr(EPDecr *epdecr) {
  auto value = m_env->visit<llvm::Value>(epdecr->exp_, this);
  auto one = llvm::ConstantInt::get(m_context, llvm::APInt(32, 1));
  m_env->setTemp(m_builder.CreateSub(value, one));
}

void CodeGenerator::visitEIncr(EIncr *eincr) {
  auto value = m_env->visit<llvm::Value>(eincr->exp_, this);
  auto one = llvm::ConstantInt::get(m_context, llvm::APInt(32, 1));
  m_env->setTemp(m_builder.CreateAdd(value, one));
}

void CodeGenerator::visitEDecr(EDecr *edecr) {
  auto value = m_env->visit<llvm::Value>(edecr->exp_, this);
  auto one = llvm::ConstantInt::get(m_context, llvm::APInt(32, 1));
  m_env->setTemp(m_builder.CreateSub(value, one));
}

void CodeGenerator::visitETimes(ETimes *etimes) {
  auto lhs = m_env->visit<llvm::Value>(etimes->exp_1, this);
  auto rhs = m_env->visit<llvm::Value>(etimes->exp_2, this);
  m_env->setTemp(
    HAS_TYPE_INT(lhs)
    ? m_builder.CreateMul(lhs, rhs)
    : m_builder.CreateFMul(lhs, rhs)
  );
}

void CodeGenerator::visitEDiv(EDiv *ediv) {
  auto lhs = m_env->visit<llvm::Value>(ediv->exp_1, this);
  auto rhs = m_env->visit<llvm::Value>(ediv->exp_2, this);
  m_env->setTemp(m_builder.CreateFDiv(lhs, rhs));
}

void CodeGenerator::visitEPlus(EPlus *eplus) {
  auto lhs = m_env->visit<llvm::Value>(eplus->exp_1, this);
  auto rhs = m_env->visit<llvm::Value>(eplus->exp_2, this);
  m_env->setTemp(
    HAS_TYPE_INT(lhs)
    ? m_builder.CreateAdd(lhs, rhs)
    : m_builder.CreateFAdd(lhs, rhs)
  );
}

void CodeGenerator::visitEMinus(EMinus *eminus) {
  auto lhs = m_env->visit<llvm::Value>(eminus->exp_1, this);
  auto rhs = m_env->visit<llvm::Value>(eminus->exp_2, this);
  m_env->setTemp(
    HAS_TYPE_INT(lhs)
    ? m_builder.CreateSub(lhs, rhs)
    : m_builder.CreateFSub(lhs, rhs)
  );
}

void CodeGenerator::visitELt(ELt *elt) {
  auto lhs = m_env->visit<llvm::Value>(elt->exp_1, this);
  auto rhs = m_env->visit<llvm::Value>(elt->exp_2, this);
  m_env->setTemp(
    HAS_TYPE_INT(lhs)
    ? m_builder.CreateICmpULT(lhs, rhs)
    : m_builder.CreateFCmpOLT(lhs, rhs)
  );
}

void CodeGenerator::visitEGt(EGt *egt) {
  auto lhs = m_env->visit<llvm::Value>(egt->exp_1, this);
  auto rhs = m_env->visit<llvm::Value>(egt->exp_2, this);
  m_env->setTemp(
    HAS_TYPE_INT(lhs)
    ? m_builder.CreateICmpUGT(lhs, rhs)
    : m_builder.CreateFCmpOGT(lhs, rhs)
  );
}

void CodeGenerator::visitELtEq(ELtEq *elteq) {
  auto lhs = m_env->visit<llvm::Value>(elteq->exp_1, this);
  auto rhs = m_env->visit<llvm::Value>(elteq->exp_2, this);
  m_env->setTemp(
    HAS_TYPE_INT(lhs)
    ? m_builder.CreateICmpULE(lhs, rhs)
    : m_builder.CreateFCmpOLE(lhs, rhs)
  );
}

void CodeGenerator::visitEGtEq(EGtEq *egteq) {
  auto lhs = m_env->visit<llvm::Value>(egteq->exp_1, this);
  auto rhs = m_env->visit<llvm::Value>(egteq->exp_2, this);
  m_env->setTemp(
    HAS_TYPE_INT(lhs)
    ? m_builder.CreateICmpUGE(lhs, rhs)
    : m_builder.CreateFCmpOGE(lhs, rhs)
  );
}

void CodeGenerator::visitEEq(EEq *eeq) {
  auto lhs = m_env->visit<llvm::Value>(eeq->exp_1, this);
  auto rhs = m_env->visit<llvm::Value>(eeq->exp_2, this);
  m_env->setTemp(
    HAS_TYPE_INT(lhs)
    ? m_builder.CreateICmpEQ(lhs, rhs)
    : m_builder.CreateFCmpUEQ(lhs, rhs)
  );
}

void CodeGenerator::visitENEq(ENEq *eneq) {
  auto lhs = m_env->visit<llvm::Value>(eneq->exp_1, this);
  auto rhs = m_env->visit<llvm::Value>(eneq->exp_2, this);
  m_env->setTemp(
    HAS_TYPE_INT(lhs)
    ? m_builder.CreateICmpNE(lhs, rhs)
    : m_builder.CreateFCmpUNE(lhs, rhs)
  );
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
  auto idValues = new std::vector<std::string>();
  for(auto id : *listid) {
    idValues->push_back(id);
  }
  m_env->setTemp(idValues);
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

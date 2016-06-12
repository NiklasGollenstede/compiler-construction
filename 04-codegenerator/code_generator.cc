#include "code_generator.h"

CodeGenerator::CodeGenerator(std::string const& moduleName, Env* env)
  :m_env(env),
   m_builder(m_context),
   m_module(new llvm::Module(moduleName, m_context)) {
}

/*
llvm::Type* CodeGenerator::convertType(Datatype type) {
  switch(type) {
    case Datatype::Void:   return llvm::Type::getVoidTy(m_context);
    case Datatype::Bool:   return llvm::Type::getInt1Ty(m_context);
    case Datatype::Int:    return llvm::Type::getInt32Ty(m_context);
    case Datatype::Double: return llvm::Type::getDoubleTy(m_context);
    case Datatype::String: return llvm::Type::getInt8PtrTy(m_context);
  }
}
*/

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

void CodeGenerator::visit(Visitable* v) {
  v->accept(this);
}

void CodeGenerator::visit(Visitable* v, llvm::Value** value) {
  v->accept(this);
  *value = m_tempValue;
}

void CodeGenerator::store(llvm::Value* value) {
  m_tempValue = value;
}

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
  visit(pdefs->listdef_);
}

void CodeGenerator::visitDFun(DFun *dfun) {
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

  // Create function.
  auto llvm_func = llvm::Function::Create(
    func_type, 
    llvm::Function::ExternalLinkage, 
    func->name, 
    m_module);

  // Create entry basic block.
  auto entry = llvm::BasicBlock::Create(
    m_context, 
    "entry", 
    llvm_func);
  m_builder.SetInsertPoint(entry);

  // Initialize arguments.
  int arg_id = 0;
  for(auto& llvm_arg : llvm_func->args()) {
    llvm_arg.setName(func->args[arg_id].name);
    
    auto value = m_builder.CreateAlloca(
      convertType(func->args[arg_id].type)
    );

    m_builder.CreateStore(
      &llvm_arg,
      value
    );

    func->args[arg_id].value = value;
    ++arg_id;
  }

  // Visit child nodes.
  visit(dfun->liststm_);
}

void CodeGenerator::visitADecl(ADecl *adecl) {
}

void CodeGenerator::visitSExp(SExp *sexp) {
  std::cout << "visiting expression statement" << std::endl;
  visit(sexp->exp_);
}

void CodeGenerator::visitSDecls(SDecls *sdecls) {
}

void CodeGenerator::visitSInit(SInit *sinit) {
}

void CodeGenerator::visitSReturn(SReturn *sreturn) {
  llvm::Value* retval;
  visit(sreturn->exp_, &retval);
  store(m_builder.CreateRet(retval));
}

void CodeGenerator::visitSReturnVoid(SReturnVoid *sreturnvoid) {
}

void CodeGenerator::visitSWhile(SWhile *swhile) {
}

void CodeGenerator::visitSBlock(SBlock *sblock) {
  visit(sblock->liststm_);
}

void CodeGenerator::visitSIfElse(SIfElse *sifelse) {
}

void CodeGenerator::visitETrue(ETrue *etrue) {
  store(llvm::ConstantInt::getTrue(m_context));
}

void CodeGenerator::visitEFalse(EFalse *efalse) {
  store(llvm::ConstantInt::getFalse(m_context));
}

void CodeGenerator::visitEInt(EInt *eint) {
  store(llvm::ConstantInt::get(m_context, llvm::APInt(32, eint->integer_)));
}

void CodeGenerator::visitEDouble(EDouble *edouble) {
  store(llvm::ConstantFP::get(m_context, llvm::APFloat(edouble->double_)));
}

void CodeGenerator::visitEString(EString *estring) {
}

void CodeGenerator::visitEId(EId *eid) {
  auto var = m_env->lookupVariable(eid->id_);
  store(m_builder.CreateLoad(var->value, var->name));
}

void CodeGenerator::visitEApp(EApp *eapp) {
  auto func = m_env->lookupFunction(eapp->id_);
}

void CodeGenerator::visitEPIncr(EPIncr *epincr) {
  llvm::Value* value;
  visit(epincr->exp_, &value);
  llvm::Value* one = llvm::ConstantInt::get(m_context, llvm::APInt(1, 32));
  store(m_builder.CreateAdd(value, one));
}

void CodeGenerator::visitEPDecr(EPDecr *epdecr) {
  llvm::Value* value;
  visit(epdecr->exp_, &value);
  llvm::Value* one = llvm::ConstantInt::get(m_context, llvm::APInt(1, 32));
  store(m_builder.CreateSub(value, one));
}

void CodeGenerator::visitEIncr(EIncr *eincr) {
  llvm::Value* value;
  visit(eincr->exp_, &value);
  llvm::Value* one = llvm::ConstantInt::get(m_context, llvm::APInt(1, 32));
  store(m_builder.CreateSub(value, one));
}

void CodeGenerator::visitEDecr(EDecr *edecr) {
  llvm::Value* value;
  visit(edecr->exp_, &value);
  llvm::Value* one = llvm::ConstantInt::get(m_context, llvm::APInt(1, 32));
  store(m_builder.CreateFSub(value, one));
}

void CodeGenerator::visitETimes(ETimes *etimes) {
  llvm::Value* lhs;
  llvm::Value* rhs;
  visit(etimes->exp_1, &lhs);
  visit(etimes->exp_2, &rhs);
  store(m_builder.CreateMul(lhs, rhs));
}

void CodeGenerator::visitEDiv(EDiv *ediv) {
  llvm::Value* lhs;
  llvm::Value* rhs;
  visit(ediv->exp_1, &lhs);
  visit(ediv->exp_2, &rhs);
  store(m_builder.CreateFDiv(lhs, rhs));
}

void CodeGenerator::visitEPlus(EPlus *eplus) {
  llvm::Value* lhs;
  llvm::Value* rhs;
  visit(eplus->exp_1, &lhs);
  visit(eplus->exp_2, &rhs);
  store(m_builder.CreateAdd(lhs, rhs));
}

void CodeGenerator::visitEMinus(EMinus *eminus) {
  llvm::Value* lhs;
  llvm::Value* rhs;
  visit(eminus->exp_1, &lhs);
  visit(eminus->exp_2, &rhs);
  store(m_builder.CreateSub(lhs, rhs));
}

void CodeGenerator::visitELt(ELt *elt) {
  llvm::Value* lhs;
  llvm::Value* rhs;
  visit(elt->exp_1, &lhs);
  visit(elt->exp_2, &rhs);
  store(m_builder.CreateFCmpOLT(lhs, rhs));
}

void CodeGenerator::visitEGt(EGt *egt) {
  llvm::Value* lhs;
  llvm::Value* rhs;
  visit(egt->exp_1, &lhs);
  visit(egt->exp_2, &rhs);
  store(m_builder.CreateFCmpOGT(lhs, rhs));
}

void CodeGenerator::visitELtEq(ELtEq *elteq) {
  llvm::Value* lhs;
  llvm::Value* rhs;
  visit(elteq->exp_1, &lhs);
  visit(elteq->exp_2, &rhs);
  store(m_builder.CreateFCmpOLE(lhs, rhs));
}

void CodeGenerator::visitEGtEq(EGtEq *egteq) {
  llvm::Value* lhs;
  llvm::Value* rhs;
  visit(egteq->exp_1, &lhs);
  visit(egteq->exp_2, &rhs);
  store(m_builder.CreateFCmpOLT(lhs, rhs));
}

void CodeGenerator::visitEEq(EEq *eeq) {
  llvm::Value* lhs;
  llvm::Value* rhs;
  visit(eeq->exp_1, &lhs);
  visit(eeq->exp_2, &rhs);
  store(m_builder.CreateFCmpUEQ(lhs, rhs));
}

void CodeGenerator::visitENEq(ENEq *eneq) {
  llvm::Value* lhs;
  llvm::Value* rhs;
  visit(eneq->exp_1, &lhs);
  visit(eneq->exp_2, &rhs);
  store(m_builder.CreateFCmpUNE(lhs, rhs));
}

void CodeGenerator::visitEAnd(EAnd *eand) {
  llvm::Value* lhs;
  llvm::Value* rhs;
  visit(eand->exp_1, &lhs);
  visit(eand->exp_2, &rhs);
  store(m_builder.CreateAnd(lhs, rhs));
}

void CodeGenerator::visitEOr(EOr *eor) {
  llvm::Value* lhs;
  llvm::Value* rhs;
  visit(eor->exp_1, &lhs);
  visit(eor->exp_2, &rhs);
  store(m_builder.CreateOr(lhs, rhs));
}

void CodeGenerator::visitEAss(EAss *eass) {
  std::cout << "visiting assignment" << std::endl;
  auto var = m_env->lookupVariable(
    ((EId*)(eass->exp_1))->id_
  );
  std::cout << "assigning to variable " << var->name << std::endl;

  llvm::Value* value;
  visit(eass->exp_2, &value);
  store(m_builder.CreateStore(value, var->value));
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
    visit(def);
  }
}

void CodeGenerator::visitListArg(ListArg* listarg) {
  for(auto arg : *listarg) {
    visit(arg);
  }
}

void CodeGenerator::visitListStm(ListStm* liststm) {
  for(auto stm : *liststm) {
    visit(stm);
  }
}

void CodeGenerator::visitListExp(ListExp* listexp) {
  for(auto exp : *listexp) {
    visit(exp);
  }
}

void CodeGenerator::visitListId(ListId* listid) {
  for(auto id : *listid) {
    visitId(id);
  }
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
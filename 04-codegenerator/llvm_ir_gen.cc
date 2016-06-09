#include "llvm_ir_gen.h"

LLVMIRGen::LLVMIRGen(std::string const& moduleName, Env* env)
  :m_module(LLVMModuleCreateWithName(moduleName.c_str())),
   m_env(env) {
}

LLVMTypeRef LLVMIRGen::createType(Datatype type) {
  switch(type) {
    case Datatype::Void:   return LLVMVoidType();
    case Datatype::Bool:   return LLVMInt8Type();
    case Datatype::Int:    return LLVMInt32Type();
    case Datatype::Double: return LLVMDoubleType();
    case Datatype::String: return LLVMPointerType(LLVMInt8Type());
  }
}

std::vector<LLVMTypeRef> LLVMIRGen::createTypeList(std::vector<Datatype> const& types) {
  std::vector<LLVMTypeRef> list;
  for(auto type : types) { list.push_back(createType(type)); }
  return list;
}

void LLVMIRGen::createFunction(Function const& func) {
  std::vector<Datatype> argTypes;
  for(int i=0; i<func.args.size(); ++i) {
    argTypes.push_back(func.args[i].type);
  }
  auto llvmArgTypes = createTypeList(argTypes);
  LLVMAddFunction(
    m_module, 
    func.name.c_str(), 
    LLVMFunctionType(
      createType(func.returnType), 
      &llvmArgTypes[0], 
      llvmArgTypes.size(), 
      0
  ));
}

void LLVMIRGen::visitProgram(Program* t) { visitPDefs((PDefs *)t); m_module->print(std::cout); } // abstract class
void LLVMIRGen::visitDef(Def* t) {} //abstract class
void LLVMIRGen::visitArg(Arg* t) {} //abstract class
void LLVMIRGen::visitStm(Stm* t) {} //abstract class
void LLVMIRGen::visitExp(Exp* t) {} //abstract class
void LLVMIRGen::visitType(Type* t) {} // abstract class

void LLVMIRGen::visitPDefs(PDefs *pdefs) {
}

void LLVMIRGen::visitDFun(DFun *dfun) {
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
}

void LLVMIRGen::visitSReturnVoid(SReturnVoid *sreturnvoid) {
}

void LLVMIRGen::visitSWhile(SWhile *swhile) {
}

void LLVMIRGen::visitSBlock(SBlock *sblock) {
}

void LLVMIRGen::visitSIfElse(SIfElse *sifelse) {
}

void LLVMIRGen::visitETrue(ETrue *etrue) {
}

void LLVMIRGen::visitEFalse(EFalse *efalse) {
}

void LLVMIRGen::visitEInt(EInt *eint) {
}

void LLVMIRGen::visitEDouble(EDouble *edouble) {
}

void LLVMIRGen::visitEString(EString *estring) {
}

void LLVMIRGen::visitEId(EId *eid) {
}

void LLVMIRGen::visitEApp(EApp *eapp) {
}

void LLVMIRGen::visitEPIncr(EPIncr *epincr) {
}

void LLVMIRGen::visitEPDecr(EPDecr *epdecr) {
}

void LLVMIRGen::visitEIncr(EIncr *eincr) {
}

void LLVMIRGen::visitEDecr(EDecr *edecr) {
}

void LLVMIRGen::visitETimes(ETimes *etimes) {
}

void LLVMIRGen::visitEDiv(EDiv *ediv) {
}

void LLVMIRGen::visitEPlus(EPlus *eplus) {
}

void LLVMIRGen::visitEMinus(EMinus *eminus) {
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
}

void LLVMIRGen::visitListArg(ListArg* listarg) {
}

void LLVMIRGen::visitListStm(ListStm* liststm) {
}

void LLVMIRGen::visitListExp(ListExp* listexp) {
}

void LLVMIRGen::visitListId(ListId* listid) {
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
  free(m_module);
  m_module = nullptr;
}
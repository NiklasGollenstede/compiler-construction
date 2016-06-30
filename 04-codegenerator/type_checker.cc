#include "type_checker.h"
#include "env.h"

TypeChecker::TypeChecker()
  :m_env(new Env()) {
}

void TypeChecker::visitProgram(Program* t) { visitPDefs((PDefs *)t); } // abstract class
void TypeChecker::visitDef(Def* t) {} //abstract class
void TypeChecker::visitArg(Arg* t) {} //abstract class
void TypeChecker::visitStm(Stm* t) {} //abstract class
void TypeChecker::visitExp(Exp* t) {} //abstract class
void TypeChecker::visitType(Type* t) {} // abstract class

void TypeChecker::visitPDefs(PDefs *pdefs)
{
  pdefs->listdef_->accept(this);
}

void TypeChecker::visitDFun(DFun *dfun)
{
  std::string const& name = dfun->id_;

  // Function name already exists. Abort.
  if(m_env->lookupFunction(name) != nullptr) {
    error(dfun, "Function " + name + " redefined.");
  }

  Function* fn = new Function(
    m_env->visit<Datatype>(dfun->type_, this),
    name,
    m_env->visit<std::vector<Variable*>>(dfun->listarg_, this),
    m_env->enterNestedScope()
  );

  // Register function.
  m_env->registerFunction(fn);

  // Visit function body.
  m_env->visit<void>(dfun->liststm_, this);

  // Leave function scope.
  m_env->exitNestedScope();
}


void TypeChecker::visitADecl(ADecl *adecl)
{
  m_env->setTemp(new Variable(
    m_env->visit<Datatype>(adecl->type_, this),
    adecl->id_,
    nullptr
  ));
}

void TypeChecker::visitSExp(SExp *sexp)
{
  sexp->exp_->accept(this);
}

void TypeChecker::visitSDecls(SDecls *sdecls)
{
  auto type = m_env->visit<Datatype>(sdecls->type_, this);
  if(*type == Datatype::Void){
    error(sdecls, "Variable of type void is not allowed.");
  }

  auto names = m_env->visit<std::vector<std::string>>(sdecls->listid_, this);
  for(auto name : *names) {
    if(m_env->lookupVariable(name) != nullptr) {
      error(sdecls, "Redefined variable " + name + ".");
    } else {
      m_env->registerVariable(new Variable(type, name, nullptr));
    }
  }
  delete names;
}

void TypeChecker::visitSInit(SInit *sinit)
{
  auto varType  = m_env->visit<Datatype>(sinit->type_, this);
  auto exprType = m_env->visit<Datatype>(sinit->exp_, this);

  if(*varType != *exprType) {
    error(sinit, "Variable initialization type mismatch.");
  }

  if(*varType == Datatype::Void){
    error(sinit, "Variable of type void is not allowed.");
  }

  auto name = sinit->id_;

  if(m_env->lookupVariable(name) != nullptr) {
    error(sinit, "Redefined variable " + name + ".");
  } else {
    m_env->registerVariable(new Variable(varType, name, nullptr));
  }
}

void TypeChecker::visitSReturn(SReturn *sreturn)
{
  auto exprType = m_env->visit<Datatype>(sreturn->exp_, this);

  if(*exprType != *m_env->getLastFunction()->returnType) {
    error(
      sreturn,
      "Returning "
        + Datatypes::get(*exprType)
        + " instead of "
        + Datatypes::get(*m_env->getLastFunction()->returnType)
        + ".");
  }
}

void TypeChecker::visitSReturnVoid(SReturnVoid *sreturnvoid)
{
  if(*m_env->getLastFunction()->returnType != Datatype::Void){
    error(sreturnvoid, "Returning void from non-void function.");
  }
}

void TypeChecker::checkExprType(Exp *expr, Datatype type, std::string const& errmsg){
  auto exprType = m_env->visit<Datatype>(expr, this);
  if(*exprType != type){
    error(expr, errmsg);
  }
}

void TypeChecker::visitSWhile(SWhile *swhile)
{
  checkExprType(swhile->exp_, Datatype::Bool, "While condition must be of type bool.");
  swhile->stm_->accept(this);
}

void TypeChecker::visitSBlock(SBlock *sblock) {
  m_env->enterNestedScope()->setBlockStatement(sblock);
  sblock->liststm_->accept(this);
  m_env->exitNestedScope();
}

void TypeChecker::visitSIfElse(SIfElse *sifelse)
{
  checkExprType(sifelse->exp_, Datatype::Bool, "If condition must be of type bool.");
  sifelse->stm_1->accept(this);
  sifelse->stm_2->accept(this);
}

void TypeChecker::visitETrue(ETrue *etrue)
{
  m_env->setTemp(enum_new(Datatype::Bool));
}

void TypeChecker::visitEFalse(EFalse *efalse)
{
  m_env->setTemp(enum_new(Datatype::Bool));
}

void TypeChecker::visitEInt(EInt *eint)
{
  visitInteger(eint->integer_);
  m_env->setTemp(enum_new(Datatype::Int));
}

void TypeChecker::visitEDouble(EDouble *edouble)
{
  visitDouble(edouble->double_);
  m_env->setTemp(enum_new(Datatype::Double));
}

void TypeChecker::visitEString(EString *estring)
{
  visitString(estring->string_);
  m_env->setTemp(enum_new(Datatype::String));
}

void TypeChecker::visitEId(EId *eid)
{
  visitId(eid->id_);
  auto name = eid->id_;
  auto var  = m_env->lookupVariable(name);

  if(var != nullptr) {
    m_env->setTemp(enum_new(*var->type));
  } else error(eid, "Variable " + name + " undefined.");
}

void TypeChecker::visitEApp(EApp *eapp)
{
  auto name = eapp->id_;
  visitId(eapp->id_);

  auto func = m_env->lookupFunction(name);
  if(func == nullptr){
    error(eapp, "Function " + name + " undefined.");
  }

  auto argtypes = m_env->visit<std::vector<Datatype*>>(eapp->listexp_, this);
  auto expected = func->args->size();
  auto supplied = argtypes->size();

  if(expected != supplied){
    error(
      eapp,
      "Function " + name + " expects " + std::to_string(expected)
      + " arguments, but " + std::to_string(supplied) + " were supplied."
    );
  }

  bool argsMatch = true;
  for(int i=0; i<argtypes->size(); ++i){
    if(*(*argtypes)[i] != *(*func->args)[i]->type){
      argsMatch = false;
      break;
    }
  }

  if(!argsMatch) {
    std::string errmsg = "Function " + name + " expects arguments (";
    for(auto const& argument : *func->args) {
      errmsg += Datatypes::get(*argument->type) + ", ";
    }
    errmsg += ") but got (";
    for(auto const& type : *argtypes) {
      errmsg += Datatypes::get(*type) + ", ";
    }
    errmsg += ").";

    error(eapp, errmsg);
  }

  m_env->setTemp(enum_new(*func->returnType));
}

void TypeChecker::visitEPIncr(EPIncr *epincr)
{
  checkExprType(epincr->exp_, Datatype::Int, "Increment operator expects integer.");
  m_env->setTemp(enum_new(Datatype::Int));
}

void TypeChecker::visitEPDecr(EPDecr *epdecr)
{
  checkExprType(epdecr->exp_, Datatype::Int, "Decrement operator expects integer.");
  m_env->setTemp(enum_new(Datatype::Int));
}

void TypeChecker::visitEIncr(EIncr *eincr)
{
  checkExprType(eincr->exp_, Datatype::Int, "Increment operator expects integer.");
  m_env->setTemp(enum_new(Datatype::Int));
}

void TypeChecker::visitEDecr(EDecr *edecr)
{
  checkExprType(edecr->exp_, Datatype::Int, "Decrement operator expects integer.");
  m_env->setTemp(enum_new(Datatype::Int));
}

Datatype TypeChecker::checkOperands(Operation op, Exp *lhs, Exp *rhs) {
  auto lhs_type = m_env->visit<Datatype>(lhs, this);
  auto rhs_type = m_env->visit<Datatype>(rhs, this);

  // Enforce same type.
  if(*lhs_type != *rhs_type) {
    error(
      lhs,
      "Operands to arithmetic operation must be of the same type. (Got "
        + Datatypes::get(*lhs_type) + ", "
        + Datatypes::get(*rhs_type) + ")"
    );
  }

  auto type = *lhs_type;

  switch(op) {

    case Operation::Logic:
      if(type != Datatype::Bool) {
        error(
          lhs,
          "Logical operator needs boolean operands. (Got "
          + Datatypes::get(type) + ")"
        );
      }
    break;

    case Operation::Sub:
    case Operation::Mul:
    case Operation::Div:
      if(type == Datatype::String) {
        error(lhs, "String type only supports concatenation ('+' operator).");
      }

    case Operation::Add:
    case Operation::Ineq:
      if(type == Datatype::Bool) {
        error(lhs, "Boolean type only supports logical operations ('||', '&&' operators).");
      }

    case Operation::Eq:
    case Operation::Assign:
    break;
  }

  return type;
}

void TypeChecker::visitETimes(ETimes *etimes) {
  m_env->setTemp(enum_new(checkOperands(Operation::Mul, etimes->exp_1, etimes->exp_2)));
}

void TypeChecker::visitEDiv(EDiv *ediv) {
  m_env->setTemp(enum_new(checkOperands(Operation::Div, ediv->exp_1, ediv->exp_2)));
}

void TypeChecker::visitEPlus(EPlus *eplus) {
  m_env->setTemp(enum_new(checkOperands(Operation::Add, eplus->exp_1, eplus->exp_2)));
}

void TypeChecker::visitEMinus(EMinus *eminus) {
  m_env->setTemp(enum_new(checkOperands(Operation::Sub, eminus->exp_1, eminus->exp_2)));
}

void TypeChecker::visitELt(ELt *elt)
{
  checkOperands(Operation::Ineq, elt->exp_1, elt->exp_2);
  m_env->setTemp(enum_new(Datatype::Bool));
}

void TypeChecker::visitEGt(EGt *egt)
{
  checkOperands(Operation::Ineq, egt->exp_1, egt->exp_2);
  m_env->setTemp(enum_new(Datatype::Bool));
}

void TypeChecker::visitELtEq(ELtEq *elteq)
{
  checkOperands(Operation::Ineq, elteq->exp_1, elteq->exp_2);
  m_env->setTemp(enum_new(Datatype::Bool));
}

void TypeChecker::visitEGtEq(EGtEq *egteq)
{
  checkOperands(Operation::Ineq, egteq->exp_1, egteq->exp_2);
  m_env->setTemp(enum_new(Datatype::Bool));
}

void TypeChecker::visitEEq(EEq *eeq)
{
  checkOperands(Operation::Eq, eeq->exp_1, eeq->exp_2);
  m_env->setTemp(enum_new(Datatype::Bool));
}

void TypeChecker::visitENEq(ENEq *eneq)
{
  checkOperands(Operation::Eq, eneq->exp_1, eneq->exp_2);
  m_env->setTemp(enum_new(Datatype::Bool));
}

void TypeChecker::visitEAnd(EAnd *eand)
{
  m_env->setTemp(enum_new(checkOperands(Operation::Logic, eand->exp_1, eand->exp_2)));
}

void TypeChecker::visitEOr(EOr *eor)
{
  m_env->setTemp(enum_new(checkOperands(Operation::Logic, eor->exp_1, eor->exp_2)));
}

void TypeChecker::visitEAss(EAss *eass)
{
  m_env->setTemp(enum_new(checkOperands(Operation::Assign, eass->exp_1, eass->exp_2)));
}

void TypeChecker::visitETyped(ETyped *etyped)
{
  // TODO: \TBI
  error(etyped, "Not implemented!");

  etyped->exp_->accept(this);
  etyped->type_->accept(this);
}

void TypeChecker::visitType_bool(Type_bool *type_bool)
{
  m_env->setTemp(enum_new(Datatype::Bool));
}

void TypeChecker::visitType_int(Type_int *type_int)
{
  m_env->setTemp(enum_new(Datatype::Int));
}

void TypeChecker::visitType_double(Type_double *type_double)
{
  m_env->setTemp(enum_new(Datatype::Double));
}

void TypeChecker::visitType_void(Type_void *type_void)
{
  m_env->setTemp(enum_new(Datatype::Void));
}

void TypeChecker::visitType_string(Type_string *type_string)
{
  m_env->setTemp(enum_new(Datatype::String));
}

void TypeChecker::visitListDef(ListDef* listdef)
{
  for (ListDef::iterator i = listdef->begin() ; i != listdef->end() ; ++i)
  {
    (*i)->accept(this);
  }
}

void TypeChecker::visitListArg(ListArg* listarg)
{
  auto arguments = new std::vector<Variable*>();
  for (ListArg::iterator i = listarg->begin(); i != listarg->end(); ++i) {
    auto arg = m_env->visit<Variable>(*i, this);
    arguments->push_back(arg);
  }
  m_env->setTemp(arguments);
}

void TypeChecker::visitListStm(ListStm* liststm)
{
  for (ListStm::iterator i = liststm->begin() ; i != liststm->end() ; ++i)
  {
    (*i)->accept(this);
  }
}

void TypeChecker::visitListExp(ListExp* listexp)
{
  auto exprTypes = new std::vector<Datatype*>();
  for (ListExp::iterator i = listexp->begin() ; i != listexp->end() ; ++i) {
    exprTypes->push_back(m_env->visit<Datatype>(*i, this));
  }
  m_env->setTemp(exprTypes);
}

void TypeChecker::visitListId(ListId* listid)
{
  auto ids = new std::vector<std::string>();
  for (ListId::iterator i = listid->begin() ; i != listid->end() ; ++i) {
    ids->push_back(*i);
  }
  m_env->setTemp(ids);
}

void TypeChecker::visitId(Id x)
{
  /* Code for Id Goes Here */
}

void TypeChecker::visitInteger(Integer x)
{
  /* Code for Integer Goes Here */
}

void TypeChecker::visitChar(Char x)
{
  /* Code for Char Goes Here */
}

void TypeChecker::visitDouble(Double x)
{
  /* Code for Double Goes Here */
}

void TypeChecker::visitString(String x)
{
  /* Code for String Goes Here */
}

void TypeChecker::visitIdent(Ident x)
{
  /* Code for Ident Goes Here */
}

TypeChecker::~TypeChecker() {
  delete m_env;
}

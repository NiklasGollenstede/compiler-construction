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

  Function fn;
  fn.name = name;

  // Visit datatype.
  Datatype returnType;
  if(m_env->visit(dfun->type_, this, &returnType)) {
    fn.returnType = returnType;
  } else crash(dfun->type_);

  visitId(dfun->id_);

  // Visit argument list.
  std::vector<Variable> args;
  if(m_env->visit(dfun->listarg_, this, &args)) {
    fn.args = args;
  } else crash();

  // Register new function and create fn scope.
  m_env->registerFunction(fn);
  m_env->pushScope();

  // Visit function body.
  m_env->visit(dfun->liststm_, this);

  // Remove function scope.
  m_env->popScope();
}


void TypeChecker::visitADecl(ADecl *adecl)
{
  auto arg = new Variable();

  Datatype type;
  if(m_env->visit(adecl->type_, this, &type)) {
    arg->type = type;
  } else crash(adecl->type_);

  visitId(adecl->id_);
  arg->name = adecl->id_;

  m_env->setTemp(arg);
}

void TypeChecker::visitSExp(SExp *sexp)
{
  sexp->exp_->accept(this);
}

void TypeChecker::visitSDecls(SDecls *sdecls)
{
  Datatype type;
  if(m_env->visit(sdecls->type_, this, &type)) {

    if(type == Datatype::Void){
      error(sdecls, "Variable of type void is not allowed.");
    }

    std::vector<std::string> names;
    if(m_env->visit(sdecls->listid_, this, &names)) {
      for(auto name : names) {
        if(!m_env->registerVariable(Variable { type, name })){
          error(sdecls, "Redefined variable " + name + ".");
        }
      }
    }
  }
}

void TypeChecker::visitSInit(SInit *sinit)
{
  Datatype varType, exprType;
  if(m_env->visit(sinit->type_, this, &varType) && m_env->visit(sinit->exp_, this, &exprType)){
    if(varType != exprType) {
      error(sinit, "Variable initialization type mismatch.");
    }
  } else crash();

  if(varType == Datatype::Void){
    error(sinit, "Variable of type void is not allowed.");
  }

  auto name = sinit->id_;
  visitId(sinit->id_);

  if(!m_env->registerVariable(Variable { varType, name })){
    error(sinit, "Redefined variable " + name + ".");
  }
}

void TypeChecker::visitSReturn(SReturn *sreturn)
{
  Datatype exprType;
  if(m_env->visit(sreturn->exp_, this, &exprType)) {
    if(exprType != m_env->getLastFunction().returnType) {
      error(
        sreturn,
        "Returning "
          + Datatypes::get(exprType)
          + " instead of "
          + Datatypes::get(m_env->getLastFunction().returnType)
          + ".");
    }
  } else crash(sreturn->exp_);
}

void TypeChecker::visitSReturnVoid(SReturnVoid *sreturnvoid)
{
  if(m_env->getLastFunction().returnType != Datatype::Void){
    error(sreturnvoid, "Returning void from non-void function.");
  }
}

void TypeChecker::checkExprType(Exp *expr, Datatype type, std::string const& errmsg){
  Datatype exprType;
  if(m_env->visit(expr, this, &exprType)){
    if(exprType != type){
      error(expr, errmsg);
    }
  } else crash();
}

void TypeChecker::visitSWhile(SWhile *swhile)
{
  checkExprType(swhile->exp_, Datatype::Bool, "While condition must be of type bool.");
  swhile->stm_->accept(this);
}

void TypeChecker::visitSBlock(SBlock *sblock)
{
  m_env->pushScope();
  sblock->liststm_->accept(this);
  m_env->popScope();
}

void TypeChecker::visitSIfElse(SIfElse *sifelse)
{
  checkExprType(sifelse->exp_, Datatype::Bool, "If condition must be of type bool.");
  sifelse->stm_1->accept(this);
  sifelse->stm_2->accept(this);
}

void TypeChecker::visitETrue(ETrue *etrue)
{
  m_env->setTemp(new Datatype(Datatype::Bool));
}

void TypeChecker::visitEFalse(EFalse *efalse)
{
  m_env->setTemp(new Datatype(Datatype::Bool));
}

void TypeChecker::visitEInt(EInt *eint)
{
  visitInteger(eint->integer_);
  m_env->setTemp(new Datatype(Datatype::Int));
}

void TypeChecker::visitEDouble(EDouble *edouble)
{
  visitDouble(edouble->double_);
  m_env->setTemp(new Datatype(Datatype::Double));
}

void TypeChecker::visitEString(EString *estring)
{
  visitString(estring->string_);
  m_env->setTemp(new Datatype(Datatype::String));
}

void TypeChecker::visitEId(EId *eid)
{
  visitId(eid->id_);
  auto name = eid->id_;
  auto var  = m_env->lookupVariable(name);

  if(var != nullptr) {
    m_env->setTemp(new Datatype(var->type));
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

  std::vector<Datatype> argtypes;
  if(m_env->visit(eapp->listexp_, this, &argtypes)){
    auto expected = func->args.size();
    auto supplied = argtypes.size();
    if(expected != supplied){
      error(
        eapp,
        "Function " + name + " expects " + std::to_string(expected)
        + " arguments, but " + std::to_string(supplied) + " were supplied."
      );
    }

    for(int i=0; i<argtypes.size(); ++i){
      if(argtypes[i] != func->args[i].type){
        error(eapp, "Function argument type mismatch.");
      }
    }
  } else crash();

  m_env->setTemp(new Datatype(func->returnType));
}

void TypeChecker::visitEPIncr(EPIncr *epincr)
{
  checkExprType(epincr->exp_, Datatype::Int, "Increment operator expects integer.");
  m_env->setTemp(new Datatype(Datatype::Int));
}

void TypeChecker::visitEPDecr(EPDecr *epdecr)
{
  checkExprType(epdecr->exp_, Datatype::Int, "Decrement operator expects integer.");
  m_env->setTemp(new Datatype(Datatype::Int));
}

void TypeChecker::visitEIncr(EIncr *eincr)
{
  checkExprType(eincr->exp_, Datatype::Int, "Increment operator expects integer.");
  m_env->setTemp(new Datatype(Datatype::Int));
}

void TypeChecker::visitEDecr(EDecr *edecr)
{
  checkExprType(edecr->exp_, Datatype::Int, "Decrement operator expects integer.");
  m_env->setTemp(new Datatype(Datatype::Int));
}

Datatype TypeChecker::checkOperands(Operation op, Exp *lhs, Exp *rhs) {
  Datatype lhs_type, rhs_type, type;

  if(m_env->visit(lhs, this, &lhs_type) && m_env->visit(rhs, this, &rhs_type)) {

    // Enforce same type.
    if(lhs_type != rhs_type) {
      error(
        lhs,
        "Operand type mismatch. ("
          + Datatypes::get(lhs_type) + ", "
          + Datatypes::get(rhs_type) + ")"
      );
    }

    type = lhs_type;

    switch(op) {

      case Operation::Logic:
        if(type != Datatype::Bool) {
          error(lhs, "Logical operator needs boolean operands.");
        }
      break;

      case Operation::Sub:
      case Operation::Mul:
      case Operation::Div:
        if(type == Datatype::String) {
          error(lhs, "Strings only support addition.");
        }

      case Operation::Add:
      case Operation::Ineq:
        if(type == Datatype::Bool) {
          error(lhs, "Boolean type does not support arithmetics.");
        }

      case Operation::Eq:
      case Operation::Assign:
      break;
    }
  } else crash();

  return type;
}

void TypeChecker::visitETimes(ETimes *etimes) {
  m_env->setTemp(new Datatype(checkOperands(Operation::Mul, etimes->exp_1, etimes->exp_2)));
}

void TypeChecker::visitEDiv(EDiv *ediv) {
  m_env->setTemp(new Datatype(checkOperands(Operation::Div, ediv->exp_1, ediv->exp_2)));
}

void TypeChecker::visitEPlus(EPlus *eplus) {
  m_env->setTemp(new Datatype(checkOperands(Operation::Add, eplus->exp_1, eplus->exp_2)));
}

void TypeChecker::visitEMinus(EMinus *eminus) {
  m_env->setTemp(new Datatype(checkOperands(Operation::Sub, eminus->exp_1, eminus->exp_2)));
}

void TypeChecker::visitELt(ELt *elt)
{
  checkOperands(Operation::Ineq, elt->exp_1, elt->exp_2);
  m_env->setTemp(new Datatype(Datatype::Bool));
}

void TypeChecker::visitEGt(EGt *egt)
{
  checkOperands(Operation::Ineq, egt->exp_1, egt->exp_2);
  m_env->setTemp(new Datatype(Datatype::Bool));
}

void TypeChecker::visitELtEq(ELtEq *elteq)
{
  checkOperands(Operation::Ineq, elteq->exp_1, elteq->exp_2);
  m_env->setTemp(new Datatype(Datatype::Bool));
}

void TypeChecker::visitEGtEq(EGtEq *egteq)
{
  checkOperands(Operation::Ineq, egteq->exp_1, egteq->exp_2);
  m_env->setTemp(new Datatype(Datatype::Bool));
}

void TypeChecker::visitEEq(EEq *eeq)
{
  checkOperands(Operation::Eq, eeq->exp_1, eeq->exp_2);
  m_env->setTemp(new Datatype(Datatype::Bool));
}

void TypeChecker::visitENEq(ENEq *eneq)
{
  checkOperands(Operation::Eq, eneq->exp_1, eneq->exp_2);
  m_env->setTemp(new Datatype(Datatype::Bool));
}

void TypeChecker::visitEAnd(EAnd *eand)
{
  m_env->setTemp(new Datatype(checkOperands(Operation::Logic, eand->exp_1, eand->exp_2)));
}

void TypeChecker::visitEOr(EOr *eor)
{
  m_env->setTemp(new Datatype(checkOperands(Operation::Logic, eor->exp_1, eor->exp_2)));
}

void TypeChecker::visitEAss(EAss *eass)
{
  m_env->setTemp(new Datatype(checkOperands(Operation::Assign, eass->exp_1, eass->exp_2)));
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
  m_env->setTemp(new Datatype(Datatype::Bool));
}

void TypeChecker::visitType_int(Type_int *type_int)
{
  m_env->setTemp(new Datatype(Datatype::Int));
}

void TypeChecker::visitType_double(Type_double *type_double)
{
  m_env->setTemp(new Datatype(Datatype::Double));
}

void TypeChecker::visitType_void(Type_void *type_void)
{
  m_env->setTemp(new Datatype(Datatype::Void));
}

void TypeChecker::visitType_string(Type_string *type_string)
{
  m_env->setTemp(new Datatype(Datatype::String));
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
  auto arguments = new std::vector<Variable>();
  for (ListArg::iterator i = listarg->begin(); i != listarg->end(); ++i) {
    Variable arg;
    if(m_env->visit(*i, this, &arg)) {
      arguments->push_back(arg);
    } else crash();
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
  auto exprTypes = new std::vector<Datatype>();
  for (ListExp::iterator i = listexp->begin() ; i != listexp->end() ; ++i) {
    Datatype exprType;
    if(m_env->visit(*i, this, &exprType)){
      exprTypes->push_back(exprType);
    } else crash();
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

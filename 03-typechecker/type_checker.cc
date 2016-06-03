#include "type_checker.h"
#include "env.h"

// Throw an exception containing the given node's line position and description.
template<typename T>
void error(T* node, std::string const& description) {
  EXCEPT("Error in line " + std::to_string(node->line_number) + ": " + description);
}

// Throw a critical error w/ default message.
inline void crash() { EXCEPT("A critical error occured."); }

template<typename T>
void crash(T* node) {
  EXCEPT("A critical error occured in line " + std::to_string(node->line_number) + ".");
}

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
  /* Code For SExp Goes Here */

  sexp->exp_->accept(this);

}

void TypeChecker::visitSDecls(SDecls *sdecls)
{
  Datatype type;
  if(m_env->visit(sdecls->type_, this, &type)) {
    std::vector<std::string> names;
    if(m_env->visit(sdecls->listid_, this, &names)) {
      for(auto name : names) {
        m_env->registerVariable(Variable { type, name });
      }
    }
  }
}

void TypeChecker::visitSInit(SInit *sinit)
{
  /* Code For SInit Goes Here */

  sinit->type_->accept(this);
  visitId(sinit->id_);
  sinit->exp_->accept(this);

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

void TypeChecker::visitSWhile(SWhile *swhile)
{
  /* Code For SWhile Goes Here */

  swhile->exp_->accept(this);
  swhile->stm_->accept(this);

}

void TypeChecker::visitSBlock(SBlock *sblock)
{
  /* Code For SBlock Goes Here */

  sblock->liststm_->accept(this);

}

void TypeChecker::visitSIfElse(SIfElse *sifelse)
{
  /* Code For SIfElse Goes Here */

  sifelse->exp_->accept(this);
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
  /* Code For EApp Goes Here */

  visitId(eapp->id_);
  eapp->listexp_->accept(this);

}

void TypeChecker::visitEPIncr(EPIncr *epincr)
{
  /* Code For EPIncr Goes Here */

  epincr->exp_->accept(this);

}

void TypeChecker::visitEPDecr(EPDecr *epdecr)
{
  /* Code For EPDecr Goes Here */

  epdecr->exp_->accept(this);

}

void TypeChecker::visitEIncr(EIncr *eincr)
{
  /* Code For EIncr Goes Here */

  eincr->exp_->accept(this);

}

void TypeChecker::visitEDecr(EDecr *edecr)
{
  /* Code For EDecr Goes Here */

  edecr->exp_->accept(this);

}

void TypeChecker::visitETimes(ETimes *etimes)
{
  /* Code For ETimes Goes Here */

  etimes->exp_1->accept(this);
  etimes->exp_2->accept(this);

}

void TypeChecker::visitEDiv(EDiv *ediv)
{
  /* Code For EDiv Goes Here */

  ediv->exp_1->accept(this);
  ediv->exp_2->accept(this);

}

void TypeChecker::visitEPlus(EPlus *eplus)
{
  /* Code For EPlus Goes Here */

  eplus->exp_1->accept(this);
  eplus->exp_2->accept(this);

}

void TypeChecker::visitEMinus(EMinus *eminus)
{
  /* Code For EMinus Goes Here */

  eminus->exp_1->accept(this);
  eminus->exp_2->accept(this);

}

void TypeChecker::visitELt(ELt *elt)
{
  /* Code For ELt Goes Here */

  elt->exp_1->accept(this);
  elt->exp_2->accept(this);

}

void TypeChecker::visitEGt(EGt *egt)
{
  /* Code For EGt Goes Here */

  egt->exp_1->accept(this);
  egt->exp_2->accept(this);

}

void TypeChecker::visitELtEq(ELtEq *elteq)
{
  /* Code For ELtEq Goes Here */

  elteq->exp_1->accept(this);
  elteq->exp_2->accept(this);

}

void TypeChecker::visitEGtEq(EGtEq *egteq)
{
  /* Code For EGtEq Goes Here */

  egteq->exp_1->accept(this);
  egteq->exp_2->accept(this);

}

void TypeChecker::visitEEq(EEq *eeq)
{
  /* Code For EEq Goes Here */

  eeq->exp_1->accept(this);
  eeq->exp_2->accept(this);

}

void TypeChecker::visitENEq(ENEq *eneq)
{
  /* Code For ENEq Goes Here */

  eneq->exp_1->accept(this);
  eneq->exp_2->accept(this);

}

void TypeChecker::visitEAnd(EAnd *eand)
{
  /* Code For EAnd Goes Here */

  eand->exp_1->accept(this);
  eand->exp_2->accept(this);

}

void TypeChecker::visitEOr(EOr *eor)
{
  /* Code For EOr Goes Here */

  eor->exp_1->accept(this);
  eor->exp_2->accept(this);

}

void TypeChecker::visitEAss(EAss *eass)
{
  /* Code For EAss Goes Here */

  eass->exp_1->accept(this);
  eass->exp_2->accept(this);

}

void TypeChecker::visitETyped(ETyped *etyped)
{
  /* Code For ETyped Goes Here */

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
  for (ListExp::iterator i = listexp->begin() ; i != listexp->end() ; ++i)
  {
    (*i)->accept(this);
  }
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
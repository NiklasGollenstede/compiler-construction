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
  if(m_env->lookupFunction(name) != nullptr) EXCEPT("Function " + name + " redefined.");

  Function fn;
  fn.name = name;
  LOG("Function name is: " << name);

  // Visit datatype.
  Datatype returnType;
  if(m_env->visit(dfun->type_, this, &returnType)) {
    LOG("Return type is: " << Datatypes::get(returnType));
    fn.returnType = returnType;
  } else EXCEPT("Undefined return type.");

  visitId(dfun->id_);

  // Visit argument list.

  std::vector<Variable> args;
  if(m_env->visit(dfun->listarg_, this, &args)) {
    for(auto arg : args) {
      LOG("Argument: " << Datatypes::get(arg.type) << " " << arg.name);
    }
    fn.args = args;
  } //else EXCEPT("Undefined argument list.");

  // Visit statement list.
  m_env->visit(dfun->liststm_, this);

  // After success, register checked function.
  m_env->registerFunction(fn);
}


void TypeChecker::visitADecl(ADecl *adecl)
{
  auto arg = new Variable();

  Datatype type;
  if(m_env->visit(adecl->type_, this, &type)) {
    arg->type = type;
  } else EXCEPT("Argument type undefined.");

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
  /* Code For SDecls Goes Here */

  sdecls->type_->accept(this);
  sdecls->listid_->accept(this);

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
  /* Code For SReturn Goes Here */

  sreturn->exp_->accept(this);

}

void TypeChecker::visitSReturnVoid(SReturnVoid *sreturnvoid)
{
  /* Code For SReturnVoid Goes Here */


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
  /* Code For ETrue Goes Here */


}

void TypeChecker::visitEFalse(EFalse *efalse)
{
  /* Code For EFalse Goes Here */


}

void TypeChecker::visitEInt(EInt *eint)
{
  /* Code For EInt Goes Here */

  visitInteger(eint->integer_);

}

void TypeChecker::visitEDouble(EDouble *edouble)
{
  /* Code For EDouble Goes Here */

  visitDouble(edouble->double_);

}

void TypeChecker::visitEString(EString *estring)
{
  /* Code For EString Goes Here */

  visitString(estring->string_);

}

void TypeChecker::visitEId(EId *eid)
{
  /* Code For EId Goes Here */

  visitId(eid->id_);

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
  
  for (ListArg::iterator i = listarg->begin() ; i != listarg->end() ; ++i)
  {
    Variable arg;
    if(m_env->visit(*i, this, &arg)) {
      arguments->push_back(arg);
    } else EXCEPT("Argument undefined.");
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
  for (ListId::iterator i = listid->begin() ; i != listid->end() ; ++i)
  {
    visitId(*i) ;
  }
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
/* This Bison file was machine-generated by BNFC */
%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <algorithm>
#include "Absyn.H"
typedef struct yy_buffer_state *YY_BUFFER_STATE;
int yyparse(void);
int yylex(void);
YY_BUFFER_STATE yy_scan_string(const char *str);
void yy_delete_buffer(YY_BUFFER_STATE buf);
int yy_mylinenumber;
int initialize_lexer(FILE * inp);
int yywrap(void)
{
  return 1;
}
void yyerror(const char *str)
{
  std::cout << "line " << yy_mylinenumber << std::endl ;
  fprintf(stderr,"error: %s\n",str);
}



static Program* YY_RESULT_Program_ = 0;
Program* pProgram(FILE *inp)
{
  yy_mylinenumber = 1;
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_Program_;
  }
}
Program* pProgram(const char *str)
{
  YY_BUFFER_STATE buf;
  int result;
  yy_mylinenumber = 1;
  initialize_lexer(0);
  buf = yy_scan_string(str);
  result = yyparse();
  yy_delete_buffer(buf);
  if (result)
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_Program_;
  }
}

static Def* YY_RESULT_Def_ = 0;
Def* pDef(FILE *inp)
{
  yy_mylinenumber = 1;
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_Def_;
  }
}
Def* pDef(const char *str)
{
  YY_BUFFER_STATE buf;
  int result;
  yy_mylinenumber = 1;
  initialize_lexer(0);
  buf = yy_scan_string(str);
  result = yyparse();
  yy_delete_buffer(buf);
  if (result)
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_Def_;
  }
}

static ListDef* YY_RESULT_ListDef_ = 0;
ListDef* pListDef(FILE *inp)
{
  yy_mylinenumber = 1;
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_ListDef_;
  }
}
ListDef* pListDef(const char *str)
{
  YY_BUFFER_STATE buf;
  int result;
  yy_mylinenumber = 1;
  initialize_lexer(0);
  buf = yy_scan_string(str);
  result = yyparse();
  yy_delete_buffer(buf);
  if (result)
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_ListDef_;
  }
}

static Arg* YY_RESULT_Arg_ = 0;
Arg* pArg(FILE *inp)
{
  yy_mylinenumber = 1;
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_Arg_;
  }
}
Arg* pArg(const char *str)
{
  YY_BUFFER_STATE buf;
  int result;
  yy_mylinenumber = 1;
  initialize_lexer(0);
  buf = yy_scan_string(str);
  result = yyparse();
  yy_delete_buffer(buf);
  if (result)
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_Arg_;
  }
}

static ListArg* YY_RESULT_ListArg_ = 0;
ListArg* pListArg(FILE *inp)
{
  yy_mylinenumber = 1;
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_ListArg_;
  }
}
ListArg* pListArg(const char *str)
{
  YY_BUFFER_STATE buf;
  int result;
  yy_mylinenumber = 1;
  initialize_lexer(0);
  buf = yy_scan_string(str);
  result = yyparse();
  yy_delete_buffer(buf);
  if (result)
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_ListArg_;
  }
}

static Stm* YY_RESULT_Stm_ = 0;
Stm* pStm(FILE *inp)
{
  yy_mylinenumber = 1;
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_Stm_;
  }
}
Stm* pStm(const char *str)
{
  YY_BUFFER_STATE buf;
  int result;
  yy_mylinenumber = 1;
  initialize_lexer(0);
  buf = yy_scan_string(str);
  result = yyparse();
  yy_delete_buffer(buf);
  if (result)
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_Stm_;
  }
}

static ListStm* YY_RESULT_ListStm_ = 0;
ListStm* pListStm(FILE *inp)
{
  yy_mylinenumber = 1;
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_ListStm_;
  }
}
ListStm* pListStm(const char *str)
{
  YY_BUFFER_STATE buf;
  int result;
  yy_mylinenumber = 1;
  initialize_lexer(0);
  buf = yy_scan_string(str);
  result = yyparse();
  yy_delete_buffer(buf);
  if (result)
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_ListStm_;
  }
}

static Exp* YY_RESULT_Exp_ = 0;
Exp* pExp(FILE *inp)
{
  yy_mylinenumber = 1;
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_Exp_;
  }
}
Exp* pExp(const char *str)
{
  YY_BUFFER_STATE buf;
  int result;
  yy_mylinenumber = 1;
  initialize_lexer(0);
  buf = yy_scan_string(str);
  result = yyparse();
  yy_delete_buffer(buf);
  if (result)
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_Exp_;
  }
}

static ListExp* YY_RESULT_ListExp_ = 0;
ListExp* pListExp(FILE *inp)
{
  yy_mylinenumber = 1;
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_ListExp_;
  }
}
ListExp* pListExp(const char *str)
{
  YY_BUFFER_STATE buf;
  int result;
  yy_mylinenumber = 1;
  initialize_lexer(0);
  buf = yy_scan_string(str);
  result = yyparse();
  yy_delete_buffer(buf);
  if (result)
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_ListExp_;
  }
}

static Type* YY_RESULT_Type_ = 0;
Type* pType(FILE *inp)
{
  yy_mylinenumber = 1;
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_Type_;
  }
}
Type* pType(const char *str)
{
  YY_BUFFER_STATE buf;
  int result;
  yy_mylinenumber = 1;
  initialize_lexer(0);
  buf = yy_scan_string(str);
  result = yyparse();
  yy_delete_buffer(buf);
  if (result)
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_Type_;
  }
}

static ListId* YY_RESULT_ListId_ = 0;
ListId* pListId(FILE *inp)
{
  yy_mylinenumber = 1;
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_ListId_;
  }
}
ListId* pListId(const char *str)
{
  YY_BUFFER_STATE buf;
  int result;
  yy_mylinenumber = 1;
  initialize_lexer(0);
  buf = yy_scan_string(str);
  result = yyparse();
  yy_delete_buffer(buf);
  if (result)
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_ListId_;
  }
}



%}

%union
{
  int int_;
  char char_;
  double double_;
  char* string_;
  Program* program_;
  Def* def_;
  ListDef* listdef_;
  Arg* arg_;
  ListArg* listarg_;
  Stm* stm_;
  ListStm* liststm_;
  Exp* exp_;
  ListExp* listexp_;
  Type* type_;
  ListId* listid_;

}


%token _ERROR_
%token _SYMB_0    //   (
%token _SYMB_1    //   )
%token _SYMB_2    //   {
%token _SYMB_3    //   }
%token _SYMB_4    //   ,
%token _SYMB_5    //   ;
%token _SYMB_6    //   =
%token _SYMB_7    //   ++
%token _SYMB_8    //   --
%token _SYMB_9    //   *
%token _SYMB_10    //   /
%token _SYMB_11    //   +
%token _SYMB_12    //   -
%token _SYMB_13    //   <
%token _SYMB_14    //   >
%token _SYMB_15    //   <=
%token _SYMB_16    //   >=
%token _SYMB_17    //   ==
%token _SYMB_18    //   !=
%token _SYMB_19    //   &&
%token _SYMB_20    //   ||
%token _SYMB_21    //   :
%token _SYMB_22    //   bool
%token _SYMB_23    //   double
%token _SYMB_24    //   else
%token _SYMB_25    //   false
%token _SYMB_26    //   if
%token _SYMB_27    //   int
%token _SYMB_28    //   return
%token _SYMB_29    //   string
%token _SYMB_30    //   true
%token _SYMB_31    //   void
%token _SYMB_32    //   while
%token<string_> _SYMB_33    //   Id

%type <program_> Program
%type <def_> Def
%type <listdef_> ListDef
%type <arg_> Arg
%type <listarg_> ListArg
%type <stm_> Stm
%type <liststm_> ListStm
%type <exp_> Exp15
%type <exp_> Exp14
%type <exp_> Exp13
%type <exp_> Exp12
%type <exp_> Exp11
%type <exp_> Exp9
%type <exp_> Exp8
%type <exp_> Exp4
%type <exp_> Exp3
%type <exp_> Exp2
%type <exp_> Exp
%type <exp_> Exp1
%type <exp_> Exp5
%type <exp_> Exp6
%type <exp_> Exp7
%type <exp_> Exp10
%type <listexp_> ListExp
%type <type_> Type
%type <listid_> ListId

%token<string_> _STRING_
%token<int_> _INTEGER_
%token<double_> _DOUBLE_

%%
Program : ListDef {  $$ = new PDefs($1); $$->line_number = yy_mylinenumber; YY_RESULT_Program_= $$; } 
;
Def : Type _SYMB_33 _SYMB_0 ListArg _SYMB_1 _SYMB_2 ListStm _SYMB_3 {  std::reverse($4->begin(),$4->end()) ;$$ = new DFun($1, $2, $4, $7); $$->line_number = yy_mylinenumber; YY_RESULT_Def_= $$; } 
;
ListDef : /* empty */ {  $$ = new ListDef(); YY_RESULT_ListDef_= $$; } 
  | ListDef Def {  $1->push_back($2) ; $$ = $1 ; YY_RESULT_ListDef_= $$; }
;
Arg : Type _SYMB_33 {  $$ = new ADecl($1, $2); $$->line_number = yy_mylinenumber; YY_RESULT_Arg_= $$; } 
;
ListArg : /* empty */ {  $$ = new ListArg(); YY_RESULT_ListArg_= $$; } 
  | Arg {  $$ = new ListArg() ; $$->push_back($1); YY_RESULT_ListArg_= $$; }
  | Arg _SYMB_4 ListArg {  $3->push_back($1) ; $$ = $3 ; YY_RESULT_ListArg_= $$; }
;
Stm : Exp _SYMB_5 {  $$ = new SExp($1); $$->line_number = yy_mylinenumber; YY_RESULT_Stm_= $$; } 
  | Type ListId _SYMB_5 {  std::reverse($2->begin(),$2->end()) ;$$ = new SDecls($1, $2); $$->line_number = yy_mylinenumber; YY_RESULT_Stm_= $$; }
  | Type _SYMB_33 _SYMB_6 Exp _SYMB_5 {  $$ = new SInit($1, $2, $4); $$->line_number = yy_mylinenumber; YY_RESULT_Stm_= $$; }
  | _SYMB_28 Exp _SYMB_5 {  $$ = new SReturn($2); $$->line_number = yy_mylinenumber; YY_RESULT_Stm_= $$; }
  | _SYMB_28 _SYMB_5 {  $$ = new SReturnVoid(); $$->line_number = yy_mylinenumber; YY_RESULT_Stm_= $$; }
  | _SYMB_32 _SYMB_0 Exp _SYMB_1 Stm {  $$ = new SWhile($3, $5); $$->line_number = yy_mylinenumber; YY_RESULT_Stm_= $$; }
  | _SYMB_2 ListStm _SYMB_3 {  $$ = new SBlock($2); $$->line_number = yy_mylinenumber; YY_RESULT_Stm_= $$; }
  | _SYMB_26 _SYMB_0 Exp _SYMB_1 Stm _SYMB_24 Stm {  $$ = new SIfElse($3, $5, $7); $$->line_number = yy_mylinenumber; YY_RESULT_Stm_= $$; }
;
ListStm : /* empty */ {  $$ = new ListStm(); YY_RESULT_ListStm_= $$; } 
  | ListStm Stm {  $1->push_back($2) ; $$ = $1 ; YY_RESULT_ListStm_= $$; }
;
Exp15 : _SYMB_30 {  $$ = new ETrue(); $$->line_number = yy_mylinenumber; YY_RESULT_Exp_= $$; } 
  | _SYMB_25 {  $$ = new EFalse(); $$->line_number = yy_mylinenumber; YY_RESULT_Exp_= $$; }
  | _INTEGER_ {  $$ = new EInt($1); $$->line_number = yy_mylinenumber; YY_RESULT_Exp_= $$; }
  | _DOUBLE_ {  $$ = new EDouble($1); $$->line_number = yy_mylinenumber; YY_RESULT_Exp_= $$; }
  | _STRING_ {  $$ = new EString($1); $$->line_number = yy_mylinenumber; YY_RESULT_Exp_= $$; }
  | _SYMB_33 {  $$ = new EId($1); $$->line_number = yy_mylinenumber; YY_RESULT_Exp_= $$; }
  | _SYMB_33 _SYMB_0 ListExp _SYMB_1 {  std::reverse($3->begin(),$3->end()) ;$$ = new EApp($1, $3); $$->line_number = yy_mylinenumber; YY_RESULT_Exp_= $$; }
  | _SYMB_0 Exp _SYMB_1 {  $$ = $2; YY_RESULT_Exp_= $$; }
;
Exp14 : Exp15 _SYMB_7 {  $$ = new EPIncr($1); $$->line_number = yy_mylinenumber; YY_RESULT_Exp_= $$; } 
  | Exp15 _SYMB_8 {  $$ = new EPDecr($1); $$->line_number = yy_mylinenumber; YY_RESULT_Exp_= $$; }
  | Exp15 {  $$ = $1; YY_RESULT_Exp_= $$; }
;
Exp13 : _SYMB_7 Exp14 {  $$ = new EIncr($2); $$->line_number = yy_mylinenumber; YY_RESULT_Exp_= $$; } 
  | _SYMB_8 Exp14 {  $$ = new EDecr($2); $$->line_number = yy_mylinenumber; YY_RESULT_Exp_= $$; }
  | Exp14 {  $$ = $1; YY_RESULT_Exp_= $$; }
;
Exp12 : Exp12 _SYMB_9 Exp13 {  $$ = new ETimes($1, $3); $$->line_number = yy_mylinenumber; YY_RESULT_Exp_= $$; } 
  | Exp12 _SYMB_10 Exp13 {  $$ = new EDiv($1, $3); $$->line_number = yy_mylinenumber; YY_RESULT_Exp_= $$; }
  | Exp13 {  $$ = $1; YY_RESULT_Exp_= $$; }
;
Exp11 : Exp11 _SYMB_11 Exp12 {  $$ = new EPlus($1, $3); $$->line_number = yy_mylinenumber; YY_RESULT_Exp_= $$; } 
  | Exp11 _SYMB_12 Exp12 {  $$ = new EMinus($1, $3); $$->line_number = yy_mylinenumber; YY_RESULT_Exp_= $$; }
  | Exp12 {  $$ = $1; YY_RESULT_Exp_= $$; }
;
Exp9 : Exp9 _SYMB_13 Exp10 {  $$ = new ELt($1, $3); $$->line_number = yy_mylinenumber; YY_RESULT_Exp_= $$; } 
  | Exp9 _SYMB_14 Exp10 {  $$ = new EGt($1, $3); $$->line_number = yy_mylinenumber; YY_RESULT_Exp_= $$; }
  | Exp9 _SYMB_15 Exp10 {  $$ = new ELtEq($1, $3); $$->line_number = yy_mylinenumber; YY_RESULT_Exp_= $$; }
  | Exp9 _SYMB_16 Exp10 {  $$ = new EGtEq($1, $3); $$->line_number = yy_mylinenumber; YY_RESULT_Exp_= $$; }
  | Exp10 {  $$ = $1; YY_RESULT_Exp_= $$; }
;
Exp8 : Exp8 _SYMB_17 Exp9 {  $$ = new EEq($1, $3); $$->line_number = yy_mylinenumber; YY_RESULT_Exp_= $$; } 
  | Exp8 _SYMB_18 Exp9 {  $$ = new ENEq($1, $3); $$->line_number = yy_mylinenumber; YY_RESULT_Exp_= $$; }
  | Exp9 {  $$ = $1; YY_RESULT_Exp_= $$; }
;
Exp4 : Exp4 _SYMB_19 Exp5 {  $$ = new EAnd($1, $3); $$->line_number = yy_mylinenumber; YY_RESULT_Exp_= $$; } 
  | Exp5 {  $$ = $1; YY_RESULT_Exp_= $$; }
;
Exp3 : Exp3 _SYMB_20 Exp4 {  $$ = new EOr($1, $3); $$->line_number = yy_mylinenumber; YY_RESULT_Exp_= $$; } 
  | Exp4 {  $$ = $1; YY_RESULT_Exp_= $$; }
;
Exp2 : Exp3 _SYMB_6 Exp2 {  $$ = new EAss($1, $3); $$->line_number = yy_mylinenumber; YY_RESULT_Exp_= $$; } 
  | Exp3 {  $$ = $1; YY_RESULT_Exp_= $$; }
;
Exp : Exp1 {  $$ = $1; YY_RESULT_Exp_= $$; } 
;
Exp1 : Exp2 {  $$ = $1; YY_RESULT_Exp_= $$; } 
;
Exp5 : Exp6 {  $$ = $1; YY_RESULT_Exp_= $$; } 
;
Exp6 : Exp7 {  $$ = $1; YY_RESULT_Exp_= $$; } 
;
Exp7 : Exp8 {  $$ = $1; YY_RESULT_Exp_= $$; } 
;
Exp10 : Exp11 {  $$ = $1; YY_RESULT_Exp_= $$; } 
;
ListExp : /* empty */ {  $$ = new ListExp(); YY_RESULT_ListExp_= $$; } 
  | Exp {  $$ = new ListExp() ; $$->push_back($1); YY_RESULT_ListExp_= $$; }
  | Exp _SYMB_4 ListExp {  $3->push_back($1) ; $$ = $3 ; YY_RESULT_ListExp_= $$; }
;
Type : _SYMB_22 {  $$ = new Type_bool(); $$->line_number = yy_mylinenumber; YY_RESULT_Type_= $$; } 
  | _SYMB_27 {  $$ = new Type_int(); $$->line_number = yy_mylinenumber; YY_RESULT_Type_= $$; }
  | _SYMB_23 {  $$ = new Type_double(); $$->line_number = yy_mylinenumber; YY_RESULT_Type_= $$; }
  | _SYMB_31 {  $$ = new Type_void(); $$->line_number = yy_mylinenumber; YY_RESULT_Type_= $$; }
  | _SYMB_29 {  $$ = new Type_string(); $$->line_number = yy_mylinenumber; YY_RESULT_Type_= $$; }
;
ListId : _SYMB_33 {  $$ = new ListId() ; $$->push_back($1); YY_RESULT_ListId_= $$; } 
  | _SYMB_33 _SYMB_4 ListId {  $3->push_back($1) ; $$ = $3 ; YY_RESULT_ListId_= $$; }
;

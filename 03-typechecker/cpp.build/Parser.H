#ifndef PARSER_HEADER_FILE
#define PARSER_HEADER_FILE

#include<vector>
#include<string>


class Program;
class Def;
class ListDef;
class Arg;
class ListArg;
class Stm;
class ListStm;
class Exp;
class ListExp;
class Type;
class ListId;

typedef union
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
} YYSTYPE;

Program* pProgram(FILE *inp);
Program* pProgram(const char *str);
Def* pDef(FILE *inp);
Def* pDef(const char *str);
ListDef* pListDef(FILE *inp);
ListDef* pListDef(const char *str);
Arg* pArg(FILE *inp);
Arg* pArg(const char *str);
ListArg* pListArg(FILE *inp);
ListArg* pListArg(const char *str);
Stm* pStm(FILE *inp);
Stm* pStm(const char *str);
ListStm* pListStm(FILE *inp);
ListStm* pListStm(const char *str);
Exp* pExp(FILE *inp);
Exp* pExp(const char *str);
ListExp* pListExp(FILE *inp);
ListExp* pListExp(const char *str);
Type* pType(FILE *inp);
Type* pType(const char *str);
ListId* pListId(FILE *inp);
ListId* pListId(const char *str);



#define _ERROR_ 258
#define _SYMB_0 259
#define _SYMB_1 260
#define _SYMB_2 261
#define _SYMB_3 262
#define _SYMB_4 263
#define _SYMB_5 264
#define _SYMB_6 265
#define _SYMB_7 266
#define _SYMB_8 267
#define _SYMB_9 268
#define _SYMB_10 269
#define _SYMB_11 270
#define _SYMB_12 271
#define _SYMB_13 272
#define _SYMB_14 273
#define _SYMB_15 274
#define _SYMB_16 275
#define _SYMB_17 276
#define _SYMB_18 277
#define _SYMB_19 278
#define _SYMB_20 279
#define _SYMB_21 280
#define _SYMB_22 281
#define _SYMB_23 282
#define _SYMB_24 283
#define _SYMB_25 284
#define _SYMB_26 285
#define _SYMB_27 286
#define _SYMB_28 287
#define _SYMB_29 288
#define _SYMB_30 289
#define _SYMB_31 290
#define _SYMB_32 291
#define _SYMB_33 292
#define _STRING_ 293
#define _INTEGER_ 294
#define _DOUBLE_ 295

extern YYSTYPE yylval;

#endif

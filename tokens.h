#ifndef tokens_h
#define tokens_h
/* tokens.h -- List of labelled tokens and stuff
 *
 * Generated from: Practica.g
 *
 * Terence Parr, Will Cohen, and Hank Dietz: 1989-2001
 * Purdue University Electrical Engineering
 * ANTLR Version 1.33MR33
 */
#define zzEOF_TOKEN 1
#define ASIG 2
#define OB 3
#define CB 4
#define OA 5
#define CA 6
#define OP 7
#define CP 8
#define COMA 9
#define NOT 10
#define AND 11
#define OR 12
#define EQ 13
#define NEQ 14
#define BT 15
#define LT 16
#define CONC 17
#define POP 18
#define PUSH 19
#define NORMALIZE 20
#define CHECK 21
#define ITH 22
#define EMPTY 23
#define AMEND 24
#define PLOT 25
#define LOGPLOT 26
#define WHILE 27
#define ENDWHILE 28
#define IF 29
#define ENDIF 30
#define ID 31
#define NUM 32
#define SPACE 33

#ifdef __USE_PROTOS
void plots(AST**_root);
#else
extern void plots();
#endif

#ifdef __USE_PROTOS
void linterpretation(AST**_root);
#else
extern void linterpretation();
#endif

#ifdef __USE_PROTOS
void instruction(AST**_root);
#else
extern void instruction();
#endif

#ifdef __USE_PROTOS
void returnList(AST**_root);
#else
extern void returnList();
#endif

#ifdef __USE_PROTOS
void plot(AST**_root);
#else
extern void plot();
#endif

#ifdef __USE_PROTOS
void def(AST**_root);
#else
extern void def();
#endif

#ifdef __USE_PROTOS
void literal(AST**_root);
#else
extern void literal();
#endif

#ifdef __USE_PROTOS
void pair1(AST**_root);
#else
extern void pair1();
#endif

#ifdef __USE_PROTOS
void pair2(AST**_root);
#else
extern void pair2();
#endif

#ifdef __USE_PROTOS
void booleanExpr(AST**_root);
#else
extern void booleanExpr();
#endif

#ifdef __USE_PROTOS
void booleanExpr2(AST**_root);
#else
extern void booleanExpr2();
#endif

#ifdef __USE_PROTOS
void ith(AST**_root);
#else
extern void ith();
#endif

#ifdef __USE_PROTOS
void ith2(AST**_root);
#else
extern void ith2();
#endif

#ifdef __USE_PROTOS
void consequenceIf(AST**_root);
#else
extern void consequenceIf();
#endif

#ifdef __USE_PROTOS
void consequenceWhile(AST**_root);
#else
extern void consequenceWhile();
#endif

#ifdef __USE_PROTOS
void id(AST**_root);
#else
extern void id();
#endif

#endif
extern SetWordType zzerr1[];
extern SetWordType zzerr2[];
extern SetWordType zzerr3[];
extern SetWordType setwd1[];
extern SetWordType zzerr4[];
extern SetWordType zzerr5[];
extern SetWordType zzerr6[];
extern SetWordType zzerr7[];
extern SetWordType zzerr8[];
extern SetWordType setwd2[];
extern SetWordType zzerr9[];
extern SetWordType zzerr10[];
extern SetWordType zzerr11[];
extern SetWordType setwd3[];
extern SetWordType setwd4[];

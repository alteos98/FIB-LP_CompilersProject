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
#define CONC 15
#define POP 16
#define PUSH 17
#define NORMALIZE 18
#define CHECK 19
#define ITH 20
#define EMPTY 21
#define AMEND 22
#define PLOT 23
#define LOGPLOT 24
#define WHILE 25
#define ENDWHILE 26
#define IF 27
#define ENDIF 28
#define ID 29
#define NUM 30
#define SPACE 31

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
void booleanExpr1(AST**_root);
#else
extern void booleanExpr1();
#endif

#ifdef __USE_PROTOS
void booleanExpr2(AST**_root);
#else
extern void booleanExpr2();
#endif

#ifdef __USE_PROTOS
void booleanExpr3(AST**_root);
#else
extern void booleanExpr3();
#endif

#ifdef __USE_PROTOS
void ith(AST**_root);
#else
extern void ith();
#endif

#endif
extern SetWordType zzerr1[];
extern SetWordType zzerr2[];
extern SetWordType zzerr3[];
extern SetWordType zzerr4[];
extern SetWordType setwd1[];
extern SetWordType zzerr5[];
extern SetWordType zzerr6[];
extern SetWordType zzerr7[];
extern SetWordType setwd2[];
extern SetWordType zzerr8[];
extern SetWordType zzerr9[];
extern SetWordType zzerr10[];
extern SetWordType setwd3[];

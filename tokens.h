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
#define ID 2
#define NUM 3
#define ASIG 4
#define LIST_L 5
#define LIST_R 6
#define PAIR_L 7
#define PAIR_R 8
#define PAR_L 9
#define PAR_R 10
#define NOT 11
#define AND 12
#define OR 13
#define CONC 14
#define POP 15
#define PUSH 16
#define NORMALIZE 17
#define CHECK 18
#define ITH 19
#define AMEND 20
#define PLOT 21
#define LOGPLOT 22
#define WHILE 23
#define ENDWHILE 24
#define IF 25
#define ENDIF 26
#define SPACE 27

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
void expr(AST**_root);
#else
extern void expr();
#endif

#endif
extern SetWordType setwd1[];

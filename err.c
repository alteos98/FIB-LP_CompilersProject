/*
 * A n t l r  S e t s / E r r o r  F i l e  H e a d e r
 *
 * Generated from: Practica.g
 *
 * Terence Parr, Russell Quong, Will Cohen, and Hank Dietz: 1989-2001
 * Parr Research Corporation
 * with Purdue University Electrical Engineering
 * With AHPCRC, University of Minnesota
 * ANTLR Version 1.33MR33
 */

#define ANTLR_VERSION	13333
#include "pcctscfg.h"
#include "pccts_stdio.h"

#include <string>
#include <iostream>
#include <map>
#include <list>
using namespace std;

// struct to store information about tokens
typedef struct {
  string kind; //tipus
  string text; //valor
} Attrib;

// function to fill token information (predeclaration)
void zzcr_attr(Attrib *attr, int type, char *text);

// fields for AST nodes
#define AST_FIELDS string kind; string text;
#include "ast.h"

// macro to create a new AST node (and function predeclaration)
#define zzcr_ast(as,attr,ttype,textt) as=createASTnode(attr,ttype,textt)
AST* createASTnode(Attrib* attr, int ttype, char *textt);
#define zzSET_SIZE 4
#include "antlr.h"
#include "ast.h"
#include "tokens.h"
#include "dlgdef.h"
#include "err.h"

ANTLRChar *zztokens[28]={
	/* 00 */	"Invalid",
	/* 01 */	"@",
	/* 02 */	"ID",
	/* 03 */	"NUM",
	/* 04 */	"ASIG",
	/* 05 */	"LIST_L",
	/* 06 */	"LIST_R",
	/* 07 */	"PAIR_L",
	/* 08 */	"PAIR_R",
	/* 09 */	"PAR_L",
	/* 10 */	"PAR_R",
	/* 11 */	"NOT",
	/* 12 */	"AND",
	/* 13 */	"OR",
	/* 14 */	"CONC",
	/* 15 */	"POP",
	/* 16 */	"PUSH",
	/* 17 */	"NORMALIZE",
	/* 18 */	"CHECK",
	/* 19 */	"ITH",
	/* 20 */	"AMEND",
	/* 21 */	"PLOT",
	/* 22 */	"LOGPLOT",
	/* 23 */	"WHILE",
	/* 24 */	"ENDWHILE",
	/* 25 */	"IF",
	/* 26 */	"ENDIF",
	/* 27 */	"SPACE"
};
SetWordType setwd1[28] = {0x0,0xf,0xc,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x8,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0};

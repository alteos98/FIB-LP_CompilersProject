/*
 * A n t l r  T r a n s l a t i o n  H e a d e r
 *
 * Terence Parr, Will Cohen, and Hank Dietz: 1989-2001
 * Purdue University Electrical Engineering
 * With AHPCRC, University of Minnesota
 * ANTLR Version 1.33MR33
 *
 *   antlr -gt Practica.g
 *
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
#define GENAST

#include "ast.h"

#define zzSET_SIZE 8
#include "antlr.h"
#include "tokens.h"
#include "dlgdef.h"
#include "mode.h"

/* MR23 In order to remove calls to PURIFY use the antlr -nopurify option */

#ifndef PCCTS_PURIFY
#define PCCTS_PURIFY(r,s) memset((char *) &(r),'\0',(s));
#endif

#include "ast.c"
zzASTgvars

ANTLR_INFO

#include <cstdlib>
#include <cmath>

// variable global que relaciona strings i llistes de parells
map<string, list<pair<int, int> > > dps;
//map<string,int> m;

// function to fill token information
void zzcr_attr(Attrib *attr, int type, char *text) {
  attr->kind = text;
  attr->text = "";
}

AST* createASTstring(AST* child, char* kind) {
  AST* as = new AST;
  as->kind = kind; 
  as->text = "";
  as->right = NULL; 
  as->down = child;
  return as;
}

// function to create a new AST node
AST* createASTnode(Attrib* attr, int type, char* text) {
  AST* as = new AST;
  as->kind = attr->kind; 
  as->text = attr->text;
  as->right = NULL; 
  as->down = NULL;
  return as;
}

/// get nth child of a tree. Count starts at 0.
/// if no such child, returns NULL
AST* child(AST *a,int n) {
  AST *c=a->down;
  for (int i=0; c!=NULL && i<n; i++) c=c->right;
  return c;
} 

/// print AST, recursively, with indentation
void ASTPrintIndent(AST *a,string s)
{
  if (a==NULL) return;
  
  cout<<a->kind;
  if (a->text!="") cout<<"("<<a->text<<")";
  cout<<endl;
  
  AST *i = a->down;
  while (i!=NULL && i->right!=NULL) {
    cout<<s+"  \\__";
    ASTPrintIndent(i,s+"  |"+string(i->kind.size()+i->text.size(),' '));
    i=i->right;
  }
  
  if (i!=NULL) {
    cout<<s+"  \\__";
    ASTPrintIndent(i,s+"   "+string(i->kind.size()+i->text.size(),' '));
    i=i->right;
  }
}

/// print AST 
void ASTPrint(AST *a)
{
  while (a!=NULL) {
    cout<<" ";
    ASTPrintIndent(a,"");
    a=a->right;
  }
}

// prints all the list passed as a parameter
void printListOfPairs(list<pair<int, int> > listOfPairs) {
  list<pair<int, int> >::iterator it = listOfPairs.begin();
  while (it != listOfPairs.end()) {
    if (it != listOfPairs.begin())
    cout << ", ";
    cout << "(" << (*it).first << ", " << (*it).second << ")";
    it = next(it);
  }
  cout << endl;
}

// compares two pairs only looking for the first component
bool compareTwoPairs(pair<int, int> pairOne, pair<int, int> pairTwo) {
  if (pairOne.first < pairTwo.first)
  return true;
  else
  return false;
}

// returns the number that it's in the node passed as a parameter
int evaluateInt(AST* a) {
return atoi(a->kind.c_str());
}

// evaluates the tree returning the correspondent dataset
list<pair<int, int> > evaluateList(AST* a) {
list<pair<int, int> > listOfPairs;

  if (a == NULL) {
//return listOfPairs;
}

  else if (a->kind == "Pair") {
listOfPairs.push_back(make_pair(evaluateInt(child(a,0)), evaluateInt(child(a,1))));
}

  else if (a->kind == "literal") {
int i = 1;
list<pair<int, int> > auxiliarList = evaluateList(child(a,0));

    while (!auxiliarList.empty()) {
listOfPairs.push_back(auxiliarList.front());
auxiliarList = evaluateList(child(a,i));
++i;
}
}

  else if (a->kind == "def") {
int i = 1;
list<pair<int, int> > auxiliarList = evaluateList(child(a,0));

    while (!auxiliarList.empty()) {
while (!auxiliarList.empty()) {
listOfPairs.push_back(auxiliarList.front());
auxiliarList.pop_front();
}
auxiliarList = evaluateList(child(a,i));
++i;
}
}

  else if (a->kind == "POP") {
listOfPairs = evaluateList(child(a,0));
if (!listOfPairs.empty())
listOfPairs.pop_back();
else
cout << "This dataset is empty, it's not possible to do POP" << endl;
}

  else if (a->kind == "PUSH") {
listOfPairs = evaluateList(child(a,0));
listOfPairs.push_back(evaluateList(child(a,1)).front());
}

  else if (a->kind == "NORMALIZE") {
int minX, minY;
listOfPairs = evaluateList(child(a,0));
list<pair<int, int> >::iterator it = listOfPairs.begin();

    // the first element
minX = (*it).first;
minY = (*it).second;

    while (it != listOfPairs.end()) {
if ((*it).first < minX)
minX = (*it).first;
if ((*it).second < minY)
minY = (*it).second;
it = next(it);
}

    // substraction of the minor X and Y
it = listOfPairs.begin();
while (it != listOfPairs.end()) {
(*it).first = (*it).first - minX;
(*it).second = (*it).second - minY;
it = next(it);
}
}

  else if (a->kind == "AMEND") {
list<pair<int, int> > initialList = evaluateList(child(a,0));
list<pair<int, int> >::iterator it = initialList.begin();
list<pair<int, int> >::iterator itAux;
bool sameX;

    while (it != initialList.end()) {
itAux = listOfPairs.begin();
sameX = false;
while (sameX == false and itAux != listOfPairs.end()) {
if ((*it).first == (*itAux).first)
sameX = true;
itAux = next(itAux);
}
if (!sameX)
listOfPairs.push_back((*it));
it = next(it);
}
}

  else if (a->kind == "ITH") {
int position = evaluateInt(child(a,0));
list<pair<int, int> > initialList = evaluateList(child(a,1));
list<pair<int, int> >::iterator it = initialList.begin();

    // en cas de que s'intenti accedir a una posició fora de rang ho mostrarem per pantalla
if (position > initialList.size()) {
cout << "Index " << position << " out of range" << endl;
return listOfPairs;
}

    // find the 'position'-th element
for (int i=0; i < position; ++i) {
it = next(it);
}
listOfPairs.push_back((*it));
}

  else { // ID
map<string,list<pair<int, int> > >::iterator it = dps.find(a->kind);
if (it != dps.end())
listOfPairs = dps[a->kind];
else
cout << a->kind + " was not declared" << endl;
}

	return listOfPairs;
}

// evaluates the boolean function
bool evaluateCondition(AST* a) {
bool result;

  if (a->kind == "NOT")
result = !evaluateCondition(child(a,0));

  else if (a->kind == "CHECK") {
result = true;
list<pair<int, int> > listOfPairs = evaluateList(child(a,0));
listOfPairs.sort(compareTwoPairs);

    list<pair<int, int> >::iterator it = listOfPairs.begin();
list<pair<int, int> >::iterator itAux = listOfPairs.begin();
it = next(it);
while (result and it != listOfPairs.end()) {
if ((*it).first == (*itAux).first)
result = false;
it = next(it);
itAux = next(itAux);
}
}

  else if (a->kind == "EMPTY") {
list<pair<int, int> > listOfPairs = evaluateList(child(a,0));
if (listOfPairs.empty())
result = true;
else
result = false;
}

  else if (a->kind == "==") {
list<pair<int, int> > pairOne = evaluateList(child(a,0));
list<pair<int, int> > pairTwo = evaluateList(child(a,1));
if (pairOne.empty() or pairTwo.empty())
result = false;
else if (pairOne.front().first == pairTwo.front().first and pairOne.front().second == pairTwo.front().second)
result = true;
else
result = false;
}

  else if (a->kind == "!=") {
list<pair<int, int> > pairOne = evaluateList(child(a,0));
list<pair<int, int> > pairTwo = evaluateList(child(a,1));
if (pairOne.empty() or pairTwo.empty())
result = false;
else if (pairOne.front().first != pairTwo.front().first or pairOne.front().second != pairTwo.front().second)
result = true;
else
result = false;
}

  else if (a->kind == ">") {
list<pair<int, int> > pairOne = evaluateList(child(a,0));
list<pair<int, int> > pairTwo = evaluateList(child(a,1));
if (pairOne.empty() or pairTwo.empty())
result = false;
else if (pairOne.front().first > pairTwo.front().first or
(pairOne.front().first == pairTwo.front().first and pairOne.front().second > pairTwo.front().second))
result = true;
else
result = false;
}

  else if (a->kind == "<") {
list<pair<int, int> > pairOne = evaluateList(child(a,0));
list<pair<int, int> > pairTwo = evaluateList(child(a,1));
if (pairOne.empty() or pairTwo.empty())
result = false;
else if (pairOne.front().first < pairTwo.front().first or
(pairOne.front().first == pairTwo.front().first and pairOne.front().second < pairTwo.front().second))
result = true;
else
result = false;
}

  else if (a->kind == "AND") {
if (evaluateCondition(child(a,0)) and evaluateCondition(child(a,1)))
result = true;
else
result = false;
}

  else if (a->kind == "OR") {
if (evaluateCondition(child(a,0)) or evaluateCondition(child(a,1)))
result = true;
else
result = false;
}

  else if (a->kind == "parenthesis") {
result = evaluateCondition(child(a,0));
}

  return result;
}

// executes the instructions
void execute(AST *a) {
if (a == NULL)
return;

  else if (a->kind == "list")
return execute(a->down);

	else if (a->kind == "=") {
dps[child(a,0)->kind] = evaluateList(child(a,1));
}

	else if (a->kind == "WHILE") {
while (evaluateCondition(child(a,0))) {
execute(child(a,1));
}
}

	else if (a->kind == "IF") {
if (evaluateCondition(child(a,0))) {
execute(child(a,1));
}
}

	else if (a->kind == "PLOT") {
list<pair<int, int> > listOfPairs = evaluateList(child(a,0));
cout << "PLOT: ";
printListOfPairs(listOfPairs);
}

  // x = x
// y = log2(y)
else if (a->kind == "LOGPLOT") {
list<pair<int, int> > listOfPairs = evaluateList(child(a,0));
list<pair<int, int> >::iterator it = listOfPairs.begin();
while (it != listOfPairs.end()) {
if ((*it).second != 0)
(*it).second = log((*it).second);
it = next(it);
}
cout << "LOGPLOT: ";
printListOfPairs(listOfPairs);
}

	execute(a->right);
}

int main() {
AST *root = NULL;
ANTLR(plots(&root), stdin);
ASTPrint(root);
execute(root->down);
}

void
#ifdef __USE_PROTOS
plots(AST**_root)
#else
plots(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  linterpretation(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzmatch(1); 
  (*_root)=createASTstring(_sibling, "DataPlotsProgram");
 zzCONSUME;

  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd1, 0x1);
  }
}

void
#ifdef __USE_PROTOS
linterpretation(AST**_root)
#else
linterpretation(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    while ( (setwd1[LA(1)]&0x2) ) {
      instruction(zzSTR); zzlink(_root, &_sibling, &_tail);
      zzLOOP(zztasp2);
    }
    zzEXIT(zztasp2);
    }
  }
  (*_root)=createASTstring(_sibling, "list");
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd1, 0x4);
  }
}

void
#ifdef __USE_PROTOS
instruction(AST**_root)
#else
instruction(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  if ( (LA(1)==ID) ) {
    zzmatch(ID); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
    zzmatch(ASIG); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
    returnList(zzSTR); zzlink(_root, &_sibling, &_tail);
  }
  else {
    if ( (setwd1[LA(1)]&0x8) ) {
      plot(zzSTR); zzlink(_root, &_sibling, &_tail);
    }
    else {
      if ( (LA(1)==IF) ) {
        zzmatch(IF); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
        zzmatch(OP);  zzCONSUME;
        booleanExpr1(zzSTR); zzlink(_root, &_sibling, &_tail);
        zzmatch(CP);  zzCONSUME;
        linterpretation(zzSTR); zzlink(_root, &_sibling, &_tail);
        zzmatch(ENDIF);  zzCONSUME;
      }
      else {
        if ( (LA(1)==WHILE) ) {
          zzmatch(WHILE); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
          zzmatch(OP);  zzCONSUME;
          booleanExpr1(zzSTR); zzlink(_root, &_sibling, &_tail);
          zzmatch(CP);  zzCONSUME;
          linterpretation(zzSTR); zzlink(_root, &_sibling, &_tail);
          zzmatch(ENDWHILE);  zzCONSUME;
        }
        else {zzFAIL(1,zzerr1,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
      }
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd1, 0x10);
  }
}

void
#ifdef __USE_PROTOS
returnList(AST**_root)
#else
returnList(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  if ( (setwd1[LA(1)]&0x20) ) {
    def(zzSTR); zzlink(_root, &_sibling, &_tail);
  }
  else {
    if ( (setwd1[LA(1)]&0x40) ) {
      {
        zzBLOCK(zztasp2);
        zzMake0;
        {
        if ( (LA(1)==POP) ) {
          zzmatch(POP); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
        }
        else {
          if ( (LA(1)==NORMALIZE) ) {
            zzmatch(NORMALIZE); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
          }
          else {
            if ( (LA(1)==AMEND) ) {
              zzmatch(AMEND); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
            }
            else {zzFAIL(1,zzerr2,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
          }
        }
        zzEXIT(zztasp2);
        }
      }
      zzmatch(OP);  zzCONSUME;
      returnList(zzSTR); zzlink(_root, &_sibling, &_tail);
      zzmatch(CP);  zzCONSUME;
    }
    else {
      if ( (LA(1)==PUSH) ) {
        zzmatch(PUSH); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
        zzmatch(OP);  zzCONSUME;
        returnList(zzSTR); zzlink(_root, &_sibling, &_tail);
        zzmatch(COMA);  zzCONSUME;
        pair1(zzSTR); zzlink(_root, &_sibling, &_tail);
        zzmatch(CP);  zzCONSUME;
      }
      else {zzFAIL(1,zzerr3,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd1, 0x80);
  }
}

void
#ifdef __USE_PROTOS
plot(AST**_root)
#else
plot(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    if ( (LA(1)==PLOT) ) {
      zzmatch(PLOT); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
    }
    else {
      if ( (LA(1)==LOGPLOT) ) {
        zzmatch(LOGPLOT); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
      }
      else {zzFAIL(1,zzerr4,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
    zzEXIT(zztasp2);
    }
  }
  zzmatch(OP);  zzCONSUME;
  returnList(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzmatch(CP);  zzCONSUME;
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x1);
  }
}

void
#ifdef __USE_PROTOS
def(AST**_root)
#else
def(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  literal(zzSTR); zzlink(_root, &_sibling, &_tail);
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    while ( (LA(1)==CONC) ) {
      zzmatch(CONC);  zzCONSUME;
      literal(zzSTR); zzlink(_root, &_sibling, &_tail);
      zzLOOP(zztasp2);
    }
    zzEXIT(zztasp2);
    }
  }
  (*_root)=createASTstring(_sibling, "def");
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x2);
  }
}

void
#ifdef __USE_PROTOS
literal(AST**_root)
#else
literal(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  if ( (LA(1)==OB) ) {
    {
      zzBLOCK(zztasp2);
      zzMake0;
      {
      zzmatch(OB);  zzCONSUME;
      pair1(zzSTR); zzlink(_root, &_sibling, &_tail);
      {
        zzBLOCK(zztasp3);
        zzMake0;
        {
        while ( (LA(1)==COMA) ) {
          zzmatch(COMA);  zzCONSUME;
          pair1(zzSTR); zzlink(_root, &_sibling, &_tail);
          zzLOOP(zztasp3);
        }
        zzEXIT(zztasp3);
        }
      }
      zzmatch(CB); 
      (*_root)=createASTstring(_sibling, "literal");
 zzCONSUME;

      zzEXIT(zztasp2);
      }
    }
  }
  else {
    if ( (LA(1)==ID) ) {
      zzmatch(ID); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
    }
    else {zzFAIL(1,zzerr5,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x4);
  }
}

void
#ifdef __USE_PROTOS
pair1(AST**_root)
#else
pair1(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  pair2(zzSTR); zzlink(_root, &_sibling, &_tail);
  (*_root)=createASTstring(_sibling, "Pair");
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x8);
  }
}

void
#ifdef __USE_PROTOS
pair2(AST**_root)
#else
pair2(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(OA);  zzCONSUME;
  zzmatch(NUM); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(COMA);  zzCONSUME;
  zzmatch(NUM); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(CA);  zzCONSUME;
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x10);
  }
}

void
#ifdef __USE_PROTOS
booleanExpr0(AST**_root)
#else
booleanExpr0(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(OP);  zzCONSUME;
  booleanExpr1(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzmatch(CP); 
  (*_root)=createASTstring(_sibling, "parenthesis");
 zzCONSUME;

  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x20);
  }
}

void
#ifdef __USE_PROTOS
booleanExpr1(AST**_root)
#else
booleanExpr1(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    {
      zzBLOCK(zztasp3);
      zzMake0;
      {
      if ( (setwd2[LA(1)]&0x40) ) {
        booleanExpr2(zzSTR); zzlink(_root, &_sibling, &_tail);
      }
      else {
        if ( (LA(1)==OP) ) {
          booleanExpr0(zzSTR); zzlink(_root, &_sibling, &_tail);
        }
        else {zzFAIL(1,zzerr6,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
      }
      zzEXIT(zztasp3);
      }
    }
    {
      zzBLOCK(zztasp3);
      zzMake0;
      {
      while ( (setwd2[LA(1)]&0x80) ) {
        {
          zzBLOCK(zztasp4);
          zzMake0;
          {
          if ( (LA(1)==AND) ) {
            zzmatch(AND); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
          }
          else {
            if ( (LA(1)==OR) ) {
              zzmatch(OR); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
            }
            else {zzFAIL(1,zzerr7,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
          }
          zzEXIT(zztasp4);
          }
        }
        {
          zzBLOCK(zztasp4);
          zzMake0;
          {
          if ( (setwd3[LA(1)]&0x1) ) {
            booleanExpr2(zzSTR); zzlink(_root, &_sibling, &_tail);
          }
          else {
            if ( (LA(1)==OP) ) {
              booleanExpr0(zzSTR); zzlink(_root, &_sibling, &_tail);
            }
            else {zzFAIL(1,zzerr8,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
          }
          zzEXIT(zztasp4);
          }
        }
        zzLOOP(zztasp3);
      }
      zzEXIT(zztasp3);
      }
    }
    zzEXIT(zztasp2);
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd3, 0x2);
  }
}

void
#ifdef __USE_PROTOS
booleanExpr2(AST**_root)
#else
booleanExpr2(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    if ( (LA(1)==NOT) ) {
      zzmatch(NOT); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
    }
    else {
      if ( (setwd3[LA(1)]&0x4) ) {
      }
      else {zzFAIL(1,zzerr9,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
    zzEXIT(zztasp2);
    }
  }
  booleanExpr3(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd3, 0x8);
  }
}

void
#ifdef __USE_PROTOS
booleanExpr3(AST**_root)
#else
booleanExpr3(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  if ( (setwd3[LA(1)]&0x10) ) {
    {
      zzBLOCK(zztasp2);
      zzMake0;
      {
      {
        zzBLOCK(zztasp3);
        zzMake0;
        {
        if ( (LA(1)==EMPTY) ) {
          zzmatch(EMPTY); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
        }
        else {
          if ( (LA(1)==CHECK) ) {
            zzmatch(CHECK); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
          }
          else {zzFAIL(1,zzerr10,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
        }
        zzEXIT(zztasp3);
        }
      }
      zzmatch(OP);  zzCONSUME;
      def(zzSTR); zzlink(_root, &_sibling, &_tail);
      zzmatch(CP);  zzCONSUME;
      zzEXIT(zztasp2);
      }
    }
  }
  else {
    if ( (LA(1)==ITH) ) {
      {
        zzBLOCK(zztasp2);
        zzMake0;
        {
        ith(zzSTR); zzlink(_root, &_sibling, &_tail);
        {
          zzBLOCK(zztasp3);
          zzMake0;
          {
          if ( (LA(1)==EQ) ) {
            zzmatch(EQ); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
          }
          else {
            if ( (LA(1)==NEQ) ) {
              zzmatch(NEQ); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
            }
            else {
              if ( (LA(1)==CA) ) {
                zzmatch(CA); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
              }
              else {
                if ( (LA(1)==OA) ) {
                  zzmatch(OA); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
                }
                else {zzFAIL(1,zzerr11,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
              }
            }
          }
          zzEXIT(zztasp3);
          }
        }
        ith(zzSTR); zzlink(_root, &_sibling, &_tail);
        zzEXIT(zztasp2);
        }
      }
    }
    else {zzFAIL(1,zzerr12,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd3, 0x20);
  }
}

void
#ifdef __USE_PROTOS
ith(AST**_root)
#else
ith(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(ITH); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(OP);  zzCONSUME;
  zzmatch(NUM); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(COMA);  zzCONSUME;
  def(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzmatch(CP);  zzCONSUME;
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd3, 0x40);
  }
}

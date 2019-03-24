#header
<<
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
>>

<<
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
>>

#lexclass START
#token ASIG "\="
#token OB "\["
#token CB "\]"
#token OA "\<"
#token CA "\>"
#token OP "\("
#token CP "\)"
#token COMA "\,"

#token NOT "NOT"
#token AND "AND"
#token OR "OR"
#token EQ "\=="
#token NEQ "\!="

#token CONC "\·"
#token POP "POP"
#token PUSH "PUSH"
#token NORMALIZE "NORMALIZE"
#token CHECK "CHECK"
#token ITH "ITH"
#token EMPTY "EMPTY"
#token AMEND "AMEND"
#token PLOT "PLOT"
#token LOGPLOT "LOGPLOT"
#token WHILE "WHILE"
#token ENDWHILE "ENDWHILE"
#token IF "IF"
#token ENDIF "ENDIF"

#token ID "[a-zA-Z]+[a-zA-Z0-9]*"
#token NUM "[0-9]+"
#token SPACE "[\ \n]" << zzskip();>>

plots: linterpretation "@"! <<#0=createASTstring(_sibling, "DataPlotsProgram");>> ;
linterpretation: (instruction)* <<#0=createASTstring(_sibling, "list");>> ;
instruction: ID ASIG^ returnList
	| plot
	| IF^ OP! booleanExpr1 CP! linterpretation ENDIF!
  | WHILE^ OP! booleanExpr1 CP! linterpretation ENDWHILE!
	;
returnList: def
	| (POP^ | NORMALIZE^ | AMEND^) OP! returnList CP! 
	| PUSH^ OP! returnList COMA! pair1 CP!
	;
plot: (PLOT^ | LOGPLOT^) OP! returnList CP! ;

def: literal (CONC! literal)* <<#0=createASTstring(_sibling, "def");>> ;
literal: (OB! pair1 (COMA! pair1)* CB! <<#0=createASTstring(_sibling, "literal");>>) | ID ;
pair1: pair2 <<#0=createASTstring(_sibling, "Pair");>> ;
pair2: OA! NUM COMA! NUM CA! ;

booleanExpr0: OP! booleanExpr1 CP! <<#0=createASTstring(_sibling, "parenthesis");>> ;
booleanExpr1: ((booleanExpr2 | booleanExpr0) ((AND^ | OR^) (booleanExpr2 | booleanExpr0))*) ;
booleanExpr2: (NOT^ | ) booleanExpr3;
booleanExpr3: ((EMPTY^ | CHECK^) OP! def CP!) | (ith (EQ^ | NEQ^ | CA^ | OA^) ith) ;
ith: ITH^ OP! NUM COMA! def CP! ;
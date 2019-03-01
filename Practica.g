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
  if (type == NUM) {
    attr->kind = "intconst";
    attr->text = text;
  }
  else if (type == ID) {
  	attr->kind = "charconst";
  	attr->text = text;
  }
  else {
    attr->kind = text;
    attr->text = "";
  }
}

AST* createASTstring(AST* child, char* kind) {
	AST* as = new AST;
	as->kind = kind; 
	as->text = NULL;
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

int evaluate(AST *a) {
	if (a == NULL) return 0;
	else if (a->kind == "intconst")
		return atoi(a->text.c_str());
	/*else if (a->kind == "+")
		return evaluate(child(a,0)) + evaluate(child(a,1));
	else if (a->kind == "-")
		return evaluate(child(a,0)) - evaluate(child(a,1));
	else if (a->kind == "*")
		return evaluate(child(a,0)) * evaluate(child(a,1));
	else if (a->kind == "/")
		return evaluate(child(a,0)) / evaluate(child(a,1));
	else if (a->kind == "charconst")
		return m[a->text];*/
}

void execute(AST *a) {
	if (a == NULL)
		return;
	/*else if (a->kind == "=")
		m[child(a,0)->text] = evaluate(child(a,1));
	else if (a->kind == "write")
		cout << evaluate(child(a,0)) << endl;*/
	
	execute(a->right);
}

int main() {
  AST *root = NULL;
  ANTLR(plots(&root), stdin);
  ASTPrint(root);
  execute(root);
  //cout << evaluate(root) << endl;
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
#token WHILE "\WHILE"
#token ENDWHILE "ENDWHILE"
#token IF "\IF"
#token ENDIF "ENDIF"

#token ID "[a-zA-Z]+[a-zA-Z0-9]*"
#token NUM "[0-9]+"
#token SPACE "[\ \n]" << zzskip();>>

plots: linterpretation "@"! <<#0=createASTstring(_sibling, "DataPlotsProgram");>>;
linterpretation: (instruction)* ;
instruction: ID ASIG^ returnList
	| plot
	| IF^ OP! booleanExpr ((AND | OR) booleanExpr)* CP! (instruction)* ENDIF!
	| WHILE^ OP! booleanExpr ((AND | OR) booleanExpr)* CP! (instruction)* ENDWHILE!
	;
returnList: def
	| (POP^ | NORMALIZE^ | AMEND^) OP! returnList CP!
	| PUSH^ OP! returnList COMA! onePair CP!
	;
plot: (PLOT^ | LOGPLOT^) OP! returnList CP! ;

onePair: OA NUM COMA NUM CA ;
def: ((expr | ID) (CONC (expr | ID))*) ;
expr: OB! OA! NUM COMA! NUM CA! (COMA! OA! NUM COMA! NUM CA!)* CB! ;
booleanExpr: (NOT | ) ((EMPTY | CHECK) OP! ID CP! | ith (EQ | NEQ | CA | OA) ith) ;
ith: ITH OP! NUM COMA! ID CP! ;

//program: (instruction)* ;
//instruction: ID ASIG^ expr | WRITE^ expr ;
//expr: term ((PLUS^ | MINUS^) term)* ;
//term: atom ((MULT^ | DIV^) atom)* ;
//atom: NUM | ID ;

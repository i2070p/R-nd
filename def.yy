%{
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <stack>
#include <vector>
#include <string>
#include "StackAdapter.h"
#include <iostream>
#include "Element.h"
#include <map>
#define INFILE_ERROR 1
#define OUTFILE_ERROR 2

extern "C" int yylex();
extern "C" int yyerror(const char *msg, ...);

using namespace std;


ofstream trojki;
ofstream spim;
int adr = 0;
int tmp = 0; 
int labelId = 0;
string tmpVar;

StackAdapter<Element*> mStack; 
StackAdapter<string> lStack; 
map<string, Element*> vMap;
vector<string> spimCode;
map<string, int> adressMap;
void singleCalc(string op);
%}

%union {
 char *text;
 int	ival;
 float fval;
};


%token NOT_EQUAL
%token GREATER_EQUAL
%token LESS_EQUAL
%token ASSIGNMENT
%token B_XOR
%token B_OR
%token B_AND
%token B_NOT
%token MOD
%token WHILE
%token O_IF
%token ELSE
%token ELSEIF
%token FOR
%token PRINT
%token RETURN
%token BREAK
%token GT
%token V_TRUE
%token V_FALSE
%token V_NIL
%token RUNNER
%token BEGIN_BLOCK
%token END_BLOCK
%token TYPE_INT
%token TYPE_FLOAT
%token TYPE_VOID


%token TO
%token DOWNTO

%token <text> STR
%token <ival> INTEGER
%token <fval> FLOAT
%token <text> NAME

%left '+' '-'
%left '*' '/'
%start begin

%%

begin
    :RUNNER BEGIN_BLOCK lines END_BLOCK {
         
    }
    ;

lines
    :lines line {}
    |line {}
    ;

line
    :declaration ';' { }
    |assignment ';' {}
    |if_opr block {
	string label = lStack.pop();
	stringstream line;
	line << label << ":";
	spimCode.push_back(line.str());
    }
    |if_opr block if_else_opr block {
	string label = lStack.pop();
	stringstream line;
	line << label << ":";
	spimCode.push_back(line.str());
    }
    ;

block
    : BEGIN_BLOCK lines END_BLOCK {}
    | line { }

if_opr
    : O_IF '(' condition ')' {}

if_else_opr
    : ELSE {	
	string label = lStack.pop();
	stringstream line;
	line << label << ":";
	spimCode.push_back(line.str());
}

condition
    : wyr condition_opr wyr {
	singleCalc(tmpVar);  
    }

condition_opr
    :'=' {
	tmpVar="bne";
    }

assignment
    :NAME ASSIGNMENT wyr {
  	Element* el = mStack.pop();

	stringstream line;
	line << "l" << (el->isInt() ? "i" : "w") << " $t0" << ", " << el->toString();
	spimCode.push_back(line.str());
	line.str("");
	line << "sw " << "$t0" << ", " << $1;
 	spimCode.push_back(line.str());
 	line.str("");
 	trojki << $1 << "=" << el->toString();	 
    }
    | declaration ASSIGNMENT wyr {
  	Element* el = mStack.pop();

	stringstream line;
	line << "l"<< (el->isInt() ? "i" : "w") << " $t0" << ", " << el->toString();
	spimCode.push_back(line.str());
	line.str("");
	line << "sw " << "$t0" << ", " << tmpVar;
 	spimCode.push_back(line.str());
 	line.str("");
 	trojki << tmpVar << "=" << el->toString();	 
    }
    ;    

declaration
    :NAME ':' type {
	adressMap[$1] = adr++;
	tmpVar=$1;
    } 
    ;
    
type
    :TYPE_INT {}
    |TYPE_FLOAT {}
    ;    
    
wyr
	:wyr '+' skladnik	{
		singleCalc("add");            
    }
	|wyr '-' skladnik	{
		singleCalc("sub");          
    }
	|skladnik		{}
	;
skladnik
	:skladnik '*' czynnik	{
		singleCalc("mul");
    }
	|skladnik '/' czynnik	{
		singleCalc("div");
    }
	|czynnik		{}
	;
czynnik
	:NAME			{mStack.push( new Element($1));} 
	|INTEGER		{mStack.push( new Element($1));}
	|FLOAT			{mStack.push( new Element($1));}
	|'(' wyr ')'		{}
	;
	
%%
int main(int argc, char *argv[])
{
      	trojki.open ("example.txt");
		
	yyparse();
	trojki.close();

	spim.open("spim.txt");	
	spim << ".data\n";
	for(map<string,int>::iterator it = adressMap.begin(); it != adressMap.end(); ++it) {
  		spim << it->first << ": .word 1\n";
	}
	spim << ".text\n";
	for(std::vector<string>::size_type i = 0; i != spimCode.size(); i++) {
    		spim << spimCode[i] << "\n";
	}
	spim.close();
	return 0;
}

void singleCalc(string op)
{
	stringstream sTmp;
	stringstream line;

	Element* first = mStack.pop();
	Element* second = mStack.pop();
 
	sTmp << "$tmp" << tmp++; 
	line << "l" << (first->isInt() ? "i" : "w") << " $t0, " << first->toString();
	spimCode.push_back(line.str());
	line.str("");

	line << "l" << (second->isInt() ? "i" : "w") << " $t1, " << second->toString();
	spimCode.push_back(line.str());
	line.str("");

	if (op!="bne") {
		line << op << " $t0, $t0, $t1";
		spimCode.push_back(line.str());
		line.str("");
	} else {
		line << "label" << labelId++;
		string label = line.str();
		lStack.push(label);
		line.str("");
		line << op << " $t0, $t1, " << label;
		spimCode.push_back(line.str());
		line.str("");
	}

	line << "sw $t0, " << sTmp.str();
	spimCode.push_back(line.str());
	line.str("");

	mStack.push(new Element(sTmp.str()));
	adressMap[sTmp.str()] = adr++;
	trojki  << mStack.top()->toString() << " = " << first->toString() << " " << op << " " << second->toString() << endl;
	
}

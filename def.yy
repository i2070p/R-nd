%{
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <stack>
#include <vector>
#include <string>   
#include "StackAdapter.h"
#include <iostream>   
#include "Elements/ElementFactory.h" 
#include "Operations/Builder.h"      
#include <map>    
#define INFILE_ERROR 1   
#define OUTFILE_ERROR 2  
     
extern "C" int yylex();
extern "C" int yyerror(const char *msg, ...);

using namespace std;
 
    
ofstream trojki;
ofstream spim;
 
Builder builder;

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
    :RUNNER BEGIN_BLOCK lines END_BLOCK { builder.startGenerate(); cout << "beg\n";
         
    }
    | '{' STR '}' {

    }
    ; 
 
lines   
    :lines line {}
    |line {}   
    ; 
  
line
    :declaration ';' { builder.addExpressionToSimpleOperation(); }
    |assignment ';' { builder.addExpressionToSimpleOperation(); }
    |if_opr block { builder.endIf();
    }  
    |if_opr block if_else_opr block { 
        builder.endIf(); 
    }   
    ; 
 
block 
    : BEGIN_BLOCK lines END_BLOCK { } 
    | line { }     
       
if_opr 
    : O_IF '(' condition ')' { builder.startIf(); } 

if_else_opr         
    : ELSE {	
    builder.addElse();
}
 
condition
    : fin_wyr condition_opr fin_wyr {
        builder.buildCondition();
    }
 
condition_opr 
    :'=' {
        builder.addSignToCondition(ElementFactory::createElement(EQUAL));
    } 

assignment 
    :NAME ASSIGNMENT fin_wyr {
        builder.buildAssignment($1);
    }
    ;    

declaration
    :NAME ':' type {
        builder.buildDeclaration($1);
 
    } 
    |declaration ASSIGNMENT fin_wyr {
	 
    }     
    ;
    
type
    :TYPE_INT {builder.setDataType(T_INT);}
    |TYPE_FLOAT {builder.setDataType(T_FLOAT);}
    ;    
  
fin_wyr 
    :wyr {
        builder.finishExpression();
    }
  
wyr
	:wyr '+' skladnik	{
         builder.addToExpression(ElementFactory::createElement(ADDITION));
    }
	|wyr '-' skladnik	{  
      builder.addToExpression(ElementFactory::createElement(SUBTRACTION)); 
    }
	|skladnik		{}
	;  
skladnik    
	:skladnik '*' czynnik	{
builder.addToExpression(ElementFactory::createElement(MULTIPLICATION));
    }
	|skladnik '/' czynnik	{  
builder.addToExpression(ElementFactory::createElement(DIVISION));
    }
	|czynnik		{}
	; 
czynnik  
	:NAME			{builder.addToExpression(ElementFactory::createElement($1));} 
	|INTEGER		{builder.addToExpression(ElementFactory::createElement($1));}
	|FLOAT			{builder.addToExpression(ElementFactory::createElement($1));}
	|'(' wyr ')'		{}
	;
	   
%%  
int main(int argc, char *argv[])
{		
	yyparse();
	return 0;
}

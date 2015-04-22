%{
#include <stdlib.h>
#include <string.h>
#include "def.tab.hh"
#define INFILE_ERROR 1
#define OUTFILE_ERROR 2
extern int yylineno;
int yyerror(const char *msg, ...);
void writeLexValue(char *);
%}

%%
\+			{return '+';} 
\*			{return '*';}
\(			{return '(';}
\)			{return ')';}
\[			{return '[';}
\]			{return ']';}
\/			{return '/';}
\-			{return '-';}
\=			{return '=';}
\;			{return ';';}
\:			{return ':';}

\{			{return '{';}
\}			{return '}';}

\<\<			{return BEGIN_BLOCK;}
\>\>			{return END_BLOCK;}


\!\=	    {return NOT_EQUAL;}	
\<			{return '<';}
\>			{return '>';}
\>\=		{return GREATER_EQUAL;}	
\<\=		{return LESS_EQUAL;}	
\<\-        {return ASSIGNMENT;}	    
xor			{return B_XOR;}	
or			{return B_OR;}	
and			{return B_AND;}	
not		    {return B_NOT;}	
mod			{return MOD;}	
\@\?		{return O_IF;}	
\@\!        {return ELSE;}	
\@\!\?      {return ELSEIF;}	
\@f		    {return FOR;}	
\@w         {return WHILE;}	
\@r         {return RETURN;}	
\@b         {return BREAK;}	
\@gt        {return GT;}	
\@p         {return PRINT;}	
true        {return V_TRUE;}	
false       {return V_FALSE;}	
nil         {return V_NIL;}	

int         {return TYPE_INT;}

float             {
                return TYPE_FLOAT;                          
}

runner {
	return RUNNER;
}

void             {
                return TYPE_VOID;                          
}

to          {
				return TO;
}
	 
downto      {
				return DOWNTO;
}	

0|[1-9][0-9]*	{
				yylval.ival = atoi(yytext);
				return INTEGER;
}	

[A-Za-z_][A-Za-z0-9_]*	{
				yylval.text = strdup(yytext);
				return NAME;
}	

[0-9]+\.[0-9]+	{
				yylval.fval = atof(yytext);
				return FLOAT;
}	

\".*\"    		{
				yylval.text = strdup(yytext);
				return STR;                                         
}


\ |\t			{}
\n			{yylineno++;}
.			{yyerror("B³¹d leksykalny\n");}
%%
int yyerror(const char *msg, ...)
{
	printf("%d: %s", yylineno, msg);
	exit(1);
}

void writeLexValue(char *value)
{
	fprintf(yyout, "%d::%s\n", yylineno, value);
}

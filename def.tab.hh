/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     NOT_EQUAL = 258,
     GREATER_EQUAL = 259,
     LESS_EQUAL = 260,
     ASSIGNMENT = 261,
     B_XOR = 262,
     B_OR = 263,
     B_AND = 264,
     B_NOT = 265,
     MOD = 266,
     WHILE = 267,
     O_IF = 268,
     ELSE = 269,
     ELSEIF = 270,
     FOR = 271,
     PRINT = 272,
     RETURN = 273,
     BREAK = 274,
     GT = 275,
     V_TRUE = 276,
     V_FALSE = 277,
     V_NIL = 278,
     RUNNER = 279,
     BEGIN_BLOCK = 280,
     END_BLOCK = 281,
     TYPE_INT = 282,
     TYPE_FLOAT = 283,
     TYPE_VOID = 284,
     TO = 285,
     DOWNTO = 286,
     STR = 287,
     INTEGER = 288,
     FLOAT = 289,
     NAME = 290
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 2068 of yacc.c  */
#line 36 "def.yy"

 char *text;
 int	ival;
 float fval;



/* Line 2068 of yacc.c  */
#line 93 "def.tab.hh"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


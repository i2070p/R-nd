/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

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

#ifndef YY_YY_DEF_TAB_HH_INCLUDED
# define YY_YY_DEF_TAB_HH_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
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
    READ = 273,
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
    TYPE_STR = 285,
    ARRAY = 286,
    TO = 287,
    DOWNTO = 288,
    COMMENT = 289,
    STR = 290,
    INTEGER = 291,
    FLOAT = 292,
    NAME = 293
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 29 "def.yy" /* yacc.c:1909  */

 char *text; 
 int	ival;
 float fval;

#line 99 "def.tab.hh" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_DEF_TAB_HH_INCLUDED  */

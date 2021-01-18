/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
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
    T_INT_CONST = 258,
    T_FLOAT = 259,
    T_ID = 260,
    T_SENTENCE = 261,
    T_FUNC = 262,
    T_END = 263,
    T_INT = 264,
    T_BOOL = 265,
    T_TRUE = 266,
    T_FALSE = 267,
    T_NULL = 268,
    T_TYPE = 269,
    T_EQUAL = 270,
    T_RETURN = 271,
    T_WRITE = 272,
    T_ALLOCATE = 273,
    T_OF_LENGTH = 274,
    T_IF = 275,
    T_THEN = 276,
    T_ELSE = 277,
    T_WHILE = 278,
    T_DO = 279,
    T_ARRAY_OF = 280,
    T_RECORD_OF = 281,
    T_VAR = 282,
    T_COLON = 283,
    T_SEMICOLON = 284,
    T_COMMA = 285,
    T_DOT = 286,
    T_OPEN_CURLY_B = 287,
    T_CLOSE_CURLY_B = 288,
    T_OPEN_B = 289,
    T_CLOSE_B = 290,
    T_OPEN_P = 291,
    T_CLOSE_P = 292,
    T_EQ = 293,
    T_NEQ = 294,
    T_GREATER = 295,
    T_LESS = 296,
    T_TILDE_LEFT = 297,
    T_TILDE_RIGHT = 298,
    T_GEQ = 299,
    T_LEQ = 300,
    T_AND = 301,
    T_DEQUAL = 302,
    T_NOT = 303,
    T_PIPE = 304,
    T_PLUS_E = 305,
    T_MINUS_E = 306,
    T_TIMES_E = 307,
    T_DIV_E = 308,
    T_INC = 309,
    T_DEC = 310,
    T_PLUS = 311,
    T_MINUS = 312,
    T_DIV = 313,
    T_TIMES = 314,
    T_BREAK = 315,
    T_CONTINUE = 316,
    T_FOR = 317,
    T_TO = 318,
    T_CLASS = 319,
    T_R_ARROW = 320,
    T_INLINE = 321,
    T_INLINE_NOT = 322,
    T_D_L_ARROW = 323,
    T_IN = 324,
    T_QUESTION = 325,
    T_ENUM = 326,
    T_STRING = 327,
    T_CHAR = 328,
    T_CONST = 329,
    T_EXTEND = 330,
    T_FREE = 331,
    T_LESS_MINUS = 332,
    T_PUBLIC = 333,
    T_PRIVATE = 334,
    T_SWITCH = 335,
    T_CASE = 336,
    T_TEMPLATE = 337,
    NEGATIVE = 338
  };
#endif
/* Tokens.  */
#define T_INT_CONST 258
#define T_FLOAT 259
#define T_ID 260
#define T_SENTENCE 261
#define T_FUNC 262
#define T_END 263
#define T_INT 264
#define T_BOOL 265
#define T_TRUE 266
#define T_FALSE 267
#define T_NULL 268
#define T_TYPE 269
#define T_EQUAL 270
#define T_RETURN 271
#define T_WRITE 272
#define T_ALLOCATE 273
#define T_OF_LENGTH 274
#define T_IF 275
#define T_THEN 276
#define T_ELSE 277
#define T_WHILE 278
#define T_DO 279
#define T_ARRAY_OF 280
#define T_RECORD_OF 281
#define T_VAR 282
#define T_COLON 283
#define T_SEMICOLON 284
#define T_COMMA 285
#define T_DOT 286
#define T_OPEN_CURLY_B 287
#define T_CLOSE_CURLY_B 288
#define T_OPEN_B 289
#define T_CLOSE_B 290
#define T_OPEN_P 291
#define T_CLOSE_P 292
#define T_EQ 293
#define T_NEQ 294
#define T_GREATER 295
#define T_LESS 296
#define T_TILDE_LEFT 297
#define T_TILDE_RIGHT 298
#define T_GEQ 299
#define T_LEQ 300
#define T_AND 301
#define T_DEQUAL 302
#define T_NOT 303
#define T_PIPE 304
#define T_PLUS_E 305
#define T_MINUS_E 306
#define T_TIMES_E 307
#define T_DIV_E 308
#define T_INC 309
#define T_DEC 310
#define T_PLUS 311
#define T_MINUS 312
#define T_DIV 313
#define T_TIMES 314
#define T_BREAK 315
#define T_CONTINUE 316
#define T_FOR 317
#define T_TO 318
#define T_CLASS 319
#define T_R_ARROW 320
#define T_INLINE 321
#define T_INLINE_NOT 322
#define T_D_L_ARROW 323
#define T_IN 324
#define T_QUESTION 325
#define T_ENUM 326
#define T_STRING 327
#define T_CHAR 328
#define T_CONST 329
#define T_EXTEND 330
#define T_FREE 331
#define T_LESS_MINUS 332
#define T_PUBLIC 333
#define T_PRIVATE 334
#define T_SWITCH 335
#define T_CASE 336
#define T_TEMPLATE 337
#define NEGATIVE 338

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 20 "../frontend/parser/shere_khan.y" /* yacc.c:1909  */

   int intconst;
   float floatconst;
   char *stringconst;
   struct expression *expression;
   struct type *type;
   struct function *function;
   struct head *head;
   struct tail *tail;
   struct par_decl_list *par_decl_list;
   struct var_decl_list *var_decl_list;
   struct var_type *var_type;
   struct decl_list *decl_list;
   struct body *body;
   struct declaration *declaration;
   struct statement_list *statement_list;
   struct statement *statement;
   struct variable *variable;
   struct term *term;
   struct act_list *act_list;
   struct expression_list *exp_list;
   struct inline_hint *inline_hint;
   struct enum_list *enum_list;
   struct parent_list *parent_list;
   struct access_modifier *access_modifier;
   struct switch_stm *switch_stm;
   struct switch_case *switch_case;
   struct template *template;
   struct temp_list *temp_list;
   struct temp_decl_list *temp_decl_list;
   struct temp_args *temp_args;
   struct temp_args_list *temp_args_list;
   struct temp_args_decl_list *temp_args_decl_list;

#line 255 "y.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */

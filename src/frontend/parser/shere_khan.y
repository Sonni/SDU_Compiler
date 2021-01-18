%{
#include <stdio.h>
#include <stdlib.h>
#include "../frontend/parser/tree.h"
#include "../utils/definitions.h"

extern char *yytext;
extern body *program;
extern int line_num;

int yylex();
void yyerror(char const *err)
{
   fprintf(stderr, RED "%s: ", err);
   fprintf(stderr, NORM "before %s at line %d \n", yytext, line_num);
   exit(1);
}
%}

%union {
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
}

%token <intconst> T_INT_CONST
%token <floatconst> T_FLOAT
%token <stringconst> T_ID
%token <stringconst> T_SENTENCE

%token T_FUNC
%token T_END
%token T_INT T_BOOL T_FLOAT
%token T_TRUE T_FALSE
%token T_NULL
%token T_TYPE
%token T_EQUAL
%token T_RETURN
%token T_WRITE
%token T_ALLOCATE
%token T_OF_LENGTH
%token T_IF T_THEN T_ELSE
%token T_WHILE T_DO
%token T_ARRAY_OF T_RECORD_OF
%token T_VAR
%token T_COLON T_SEMICOLON
%token T_COMMA T_DOT
%token T_OPEN_CURLY_B T_CLOSE_CURLY_B
%token T_OPEN_B T_CLOSE_B
%token T_OPEN_P T_CLOSE_P
%token T_EQ T_NEQ
%token T_GREATER T_LESS
%token T_TILDE_LEFT
%token T_TILDE_RIGHT
%token T_GEQ T_LEQ
%token T_AND
%token T_DEQUAL
%token T_NOT T_PIPE
%token T_PLUS_E T_MINUS_E T_TIMES_E T_DIV_E
%token T_INC T_DEC
%token T_PLUS T_MINUS
%token T_DIV T_TIMES
%token T_BREAK T_CONTINUE
%token T_FOR T_TO
%token T_CLASS
%token T_R_ARROW
%token T_INLINE T_INLINE_NOT
%token T_D_L_ARROW T_IN
%token T_QUESTION
%token T_ENUM
%token T_STRING
%token T_CHAR
%token T_CONST
%token T_EXTEND
%token T_FREE
%token T_LESS_MINUS
%token T_PUBLIC
%token T_PRIVATE
%token T_SWITCH
%token T_CASE
%token T_TEMPLATE

%type <expression> program expression
%type <type> type
%type <function> function
%type <par_decl_list> par_decl_list
%type <var_decl_list> var_decl_list
%type <var_type> var_type
%type <decl_list> decl_list
%type <body> body;
%type <declaration> declaration;
%type <statement_list> statement_list;
%type <statement> statement;
%type <variable> variable;
%type <term> term;
%type <act_list> act_list;
%type <exp_list> exp_list;
%type <head> head;
%type <tail> tail;
%type <inline_hint> inline_hint;
%type <enum_list> enum_list;
%type <parent_list> parent_list;
%type <access_modifier> access_modifier;
%type <switch_stm> switch_stm;
%type <switch_case> switch_case;
%type <template> template;
%type <temp_list> temp_list;
%type <temp_decl_list> temp_decl_list;
%type <temp_args> temp_args;
%type <temp_args_list> temp_args_list;
%type <temp_args_decl_list> temp_args_decl_list;

%start program

%nonassoc T_THEN
%nonassoc T_ELSE

//Precedences low to high
%left T_AND T_PIPE
%left T_DEQUAL T_NEQ
%left T_GEQ T_LEQ T_LESS T_GREATER
%left T_PLUS T_MINUS
%left T_DIV T_TIMES

%nonassoc NEGATIVE


%%
program: body
         {program = $1;}
;

function : template inline_hint head body tail
           {$$ = make_function($1, $2, $3, $4, $5);}
;

template : T_TEMPLATE T_TILDE_LEFT temp_list T_TILDE_RIGHT
           { $$ = make_template($3); }
         |
           { $$ = make_template_empty(); }
;

temp_list : temp_decl_list
            { $$ = make_tl($1); }
          | /* Empty */
            { $$ = make_tl_empty(); }
;

temp_decl_list : T_ID T_COMMA temp_decl_list
                 { $$ = make_tdl_id_list($1, $3); }
               | T_ID
                 { $$ = make_tdl_id($1); }
;

inline_hint : T_INLINE
              {$$ = make_inline_hint(IL_IL);}
            | T_INLINE_NOT
              {$$ = make_inline_hint(IL_NOT);}
            |
              {$$ = make_inline_hint(IL_EMPTY);}
;

tail : T_END T_ID
       {$$ = make_tail($2);}
;

head : T_FUNC T_ID T_OPEN_P par_decl_list T_CLOSE_P T_COLON type
       {$$ = make_head($2, $4, $7);}
;

body : decl_list statement_list
       {$$ = make_body($1, $2);}
;

type : T_ID temp_args
       {$$ = make_type_id($1, $2);}
     | T_INT
       {$$ = make_type_int();}
     | T_FLOAT
       {$$ = make_type_float();}
     | T_BOOL
       {$$ = make_type_bool();}
     | T_STRING
       {$$ = make_type_string();}
     | T_ARRAY_OF type
       {$$ = make_type_array($2);}
     | T_RECORD_OF T_OPEN_CURLY_B var_decl_list T_CLOSE_CURLY_B
       {$$ = make_type_record($3);}
     | template T_CLASS parent_list T_OPEN_CURLY_B decl_list T_CLOSE_CURLY_B
       {$$ = make_type_class($1, $3, $5);}
;

par_decl_list : var_decl_list
                {$$ = make_pdl_vdl($1);}
              | /* Empty */
                {$$ = make_pdl_empty();}
;

var_decl_list : var_type T_COMMA var_decl_list
                {$$ = make_vdl_vt_list($1, $3);}
              | var_type
                {$$ = make_vdl_vt($1);}
;

var_type : T_ID T_COLON type
           {$$ = make_var_type($1, $3);}
;

statement_list : statement
                 {$$ = make_sl_statement($1);}
               | statement_list statement
                 {$$ = make_sl_statement_list($2, $1);}
;

statement : T_RETURN expression T_SEMICOLON
            {$$ = make_statement_return($2);}
          | T_WRITE expression T_SEMICOLON
            {$$ = make_statement_write($2);}
          | T_ALLOCATE variable T_SEMICOLON
            {$$ = make_statement_allocate($2);}
          | T_ALLOCATE variable T_OF_LENGTH expression T_SEMICOLON
            {$$ = make_statement_allocate_length($2, $4);}
          | variable T_EQUAL expression T_SEMICOLON
            {$$ = make_statement_assignment($1, $3);}
         | variable T_EQUAL T_OPEN_CURLY_B exp_list T_CLOSE_CURLY_B T_SEMICOLON
            {$$ = make_statement_assignment_list($1, $4);}
          | T_IF expression T_THEN statement
            {$$ = make_statement_if($2, $4);}
          | T_IF expression T_THEN statement T_ELSE statement
            {$$ = make_statement_else($2, $4, $6);}
          | T_WHILE expression T_DO statement
            {$$ = make_statement_while($2, $4, 0);}
          | T_DO statement T_WHILE expression T_SEMICOLON
            {$$ = make_statement_while($4, $2, 1);}
          | T_OPEN_CURLY_B statement_list T_CLOSE_CURLY_B
            {$$ = make_statement_sl($2);}
          | variable T_PLUS_E expression T_SEMICOLON
            {$$ = make_statement_op_assignment(EXP_PLUS, $1, $3);}
          | variable T_MINUS_E expression T_SEMICOLON
            {$$ = make_statement_op_assignment(EXP_MINUS, $1, $3);}
          | variable T_TIMES_E expression T_SEMICOLON
            {$$ = make_statement_op_assignment(EXP_TIMES, $1, $3);}
          | variable T_DIV_E expression T_SEMICOLON
            {$$ = make_statement_op_assignment(EXP_DIV, $1, $3);}
          | variable T_INC T_SEMICOLON
            { $$ = make_statement_assignment($1, make_exp(EXP_PLUS,
                                            make_exp_term(make_term_var($1)),
                                            make_exp_term(make_term_int(1))));}
          | variable T_DEC T_SEMICOLON
            {$$ = make_statement_assignment($1, make_exp(EXP_MINUS,
                                            make_exp_term(make_term_var($1)),
                                            make_exp_term(make_term_int(1))));}
          | T_BREAK T_SEMICOLON
            {$$ = make_statement_keyword(STM_BREAK);}
          | T_CONTINUE T_SEMICOLON
            {$$ = make_statement_keyword(STM_CONTINUE);}
          | T_FOR statement expression T_SEMICOLON statement T_DO statement
            {$$ = make_statement_for_loop($2, $3, $5, $7);}
          | T_FOR statement T_TO expression T_DO statement
            {$$ = make_statement_to_loop($2, $4, $6);}
          | variable T_D_L_ARROW T_IN T_SEMICOLON
            {$$ = make_statement_in($1); }
          | variable T_EQUAL expression T_QUESTION expression
                            T_COLON expression T_SEMICOLON
            {$$ = make_statement_cond_exp($1, $3, $5, $7);}
          | T_FREE T_OPEN_P variable T_CLOSE_P T_SEMICOLON
            {$$ = make_statement_free($3);}
          | variable T_LESS_MINUS expression T_SEMICOLON
            {$$ = make_statement_assign_val($1, $3);}
          | T_SWITCH T_OPEN_P expression T_CLOSE_P
            T_OPEN_CURLY_B switch_stm T_CLOSE_CURLY_B
            {$$ = make_statement_switch($3, $6);}
;

expression : expression T_TIMES expression
             {$$ = make_exp(EXP_TIMES, $1, $3);}
           | expression T_DIV expression
             {$$ = make_exp(EXP_DIV, $1, $3);}
           | expression T_PLUS expression
             {$$ = make_exp(EXP_PLUS, $1, $3);}
           | expression T_MINUS expression
             {$$ = make_exp(EXP_MINUS, $1, $3);}
           | expression T_DEQUAL expression
             {$$ = make_exp(EXP_EQ, $1, $3);}
           | expression T_NEQ expression
             {$$ = make_exp(EXP_NEQ, $1, $3);}
           | expression T_GREATER expression
             {$$ = make_exp(EXP_GREATER, $1, $3);}
           | expression T_LESS expression
             {$$ = make_exp(EXP_LESS, $1, $3);}
           | expression T_GEQ expression
             {$$ = make_exp(EXP_GEQ, $1, $3);}
           | expression T_LEQ expression
             {$$ = make_exp(EXP_LEQ, $1, $3);}
           | expression T_AND expression
             {$$ = make_exp(EXP_AND, $1, $3);}
           | expression T_PIPE T_PIPE expression
             {$$ = make_exp(EXP_OR, $1, $4);}
           | term
             {$$ = make_exp_term($1);}
           | T_MINUS expression %prec NEGATIVE
             {$$ = make_exp(EXP_MINUS, make_exp_term(make_term_int(0)), $2);}
;

decl_list : declaration decl_list
            {$$ = make_dl_declaration($1, $2);}
          | /* Empty */
            {$$ = make_dl_empty();}
;

declaration : access_modifier T_TYPE T_ID T_EQUAL type T_SEMICOLON
              {$$ = make_declaration_type($1, $3, $5);}
            | access_modifier function
              {$$ = make_declaration_function($1, $2);}
            | access_modifier T_VAR var_decl_list T_SEMICOLON
              {$$ = make_declaration_vdl($1, $3);}
            | T_ENUM T_OPEN_CURLY_B enum_list T_CLOSE_CURLY_B T_SEMICOLON
              {$$ = make_decl_enum($3);}
            | access_modifier T_CONST type T_ID T_EQUAL expression T_SEMICOLON
              {$$ = make_decl_const($1, $3, $4, $6);}
;

enum_list : T_ID
            {$$ = make_enum_id($1);}
          | T_ID T_COMMA enum_list
            {$$ = make_enum_list($1, $3);}
;

variable : T_ID
           {$$ = make_var_id($1);}
         | variable T_OPEN_B exp_list T_CLOSE_B
           {$$ = make_var_exp($1, $3);}
         | variable T_DOT T_ID
           {$$ = make_var_dot($1, $3);}
         | variable T_R_ARROW T_ID
           {$$ = make_var_arrow($1, $3);}
         | variable T_R_ARROW T_ID T_OPEN_P act_list T_CLOSE_P
           {$$ = make_var_func($1, $3, $5);}
;

term : variable
       {$$ = make_term_var($1);}
     | T_ID temp_args T_OPEN_P act_list T_CLOSE_P
       {$$ = make_term_al($1, $2, $4);}
     | T_OPEN_P expression T_CLOSE_P
       {$$ = make_term_parentheses($2);}
     | T_NOT term
       {$$ = make_term_not($2);}
     | T_PIPE expression T_PIPE
       {$$ = make_term_absolute($2);}
     | T_INT_CONST
       {$$ = make_term_int($1);}
     | T_FLOAT
       {$$ = make_term_float($1);}
     | T_TRUE
       {$$ = make_term(TERM_TRUE);}
     | T_FALSE
       {$$ = make_term(TERM_FALSE);}
     | T_NULL
       {$$ = make_term(TERM_NULL);}
     | T_SENTENCE
       {$$ = make_term_string($1);}
;

temp_args : T_TILDE_LEFT temp_args_list T_TILDE_RIGHT
            { $$ = make_temp_args($2); }
          |
            { $$ = make_temp_args_empty(); }
;

temp_args_list : temp_args_decl_list
                 { $$ = make_temp_args_list($1); }
               |
                 { $$ = make_temp_args_list_empty(); }
;

temp_args_decl_list : type T_COMMA temp_args_decl_list
                      { $$ = make_temp_args_decl_type_list($1, $3); }
                    | type
                      { $$ = make_temp_args_decl_type($1); }
;

act_list : exp_list
           {$$ = make_act_list($1);}
         | /* Empty */
           {$$ = make_act_list_empty();}
;

exp_list : expression
           {$$ = make_exp_list_exp($1);}
         | expression T_COMMA exp_list
           {$$ = make_exp_list_list($1, $3);}
;


parent_list : T_EXTEND T_ID       // Only one parent is allowed
              {$$ = make_parent_list($2);}
            |
              {$$ = make_parent_list_empty();}
;

access_modifier : T_PUBLIC
                  {$$ = make_access_modifier(AM_PUBLIC);}
                | T_PRIVATE
                  {$$ = make_access_modifier(AM_PRIVATE);}
                |
                  {$$ = make_access_modifier(AM_PUBLIC);}
;

switch_stm : switch_case
             {$$ = make_switch_stm_case($1);}
           | switch_case switch_stm
             {$$ = make_switch_stm_list($1, $2);}
;

switch_case : T_CASE expression T_COLON statement
              {$$ = make_switch_case($2, $4);}
;

%%

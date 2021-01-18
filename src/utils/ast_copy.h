#ifndef COMPILER_AST_COPY_H
#define COMPILER_AST_COPY_H

/*
 * ast_copy copies part of the AST and returns the result.
 */

#include "../frontend/parser/tree.h"
#include "../frontend/symbol/symbol.h"

function *called_func;
char *suffix;

var_decl_list *ast_copy_vdl(var_decl_list *vdl, symbol_table *new_st);
function *ast_copy_function2(function *f, symbol_table *new_st);

function *ast_copy_function(function *f, symbol_table *new_st);
head *ast_copy_head(head *h, symbol_table *new_st);
body *ast_copy_body(body *b, symbol_table *new_st);

par_decl_list *ast_copy_par_decl_list(par_decl_list *pdl,
                                      symbol_table *new_st);

type *ast_copy_type(type *t, symbol_table *new_st);
var_decl_list *ast_copy_var_decl_list(var_decl_list *vdl,
                                      symbol_table *new_st);
var_type *ast_copy_var_type(var_type *vt, symbol_table *new_st);
decl_list *ast_copy_decl_list(decl_list *dl, symbol_table *new_st);
declaration *ast_copy_declaration(declaration *d, symbol_table *new_st);
statement_list *ast_copy_statement_list(statement_list *sl,
                                        symbol_table *new_st);
statement *ast_copy_statement(statement *stm, symbol_table *new_st);
variable *ast_copy_variable(variable *var, symbol_table *new_st);
expression *ast_copy_exp(expression *exp, symbol_table *new_st);
term *ast_copy_term(term *t, symbol_table *new_st);
expression_list *ast_copy_exp_list(expression_list *el,
                                   symbol_table *new_st);
act_list *ast_copy_act_list(act_list *al, symbol_table *new_st);

switch_stm *ast_copy_switch(switch_stm *s_stm, symbol_table *new_st);
switch_case *ast_copy_switch_case(switch_case *s_case, symbol_table *new_st);
#endif

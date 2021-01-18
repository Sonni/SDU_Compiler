#ifndef COMPILER_FREE_AST_H
#define COMPILER_FREE_AST_H

#include "../../frontend/parser/tree.h"

void free_ast(body *b);

void free_function(function *f);
void free_body(body *b);
void free_head(head *h);
void free_tail(tail *t);
void free_type(type *t);
void free_par_decl_list(par_decl_list *pdl);
void free_var_decl_list(var_decl_list *vdl);
void free_var_type(var_type *vt);
void free_statement_list(statement_list *sl);
void free_statement(statement *stm);
void free_decl_list(decl_list *dl);
void free_declaration(declaration *d);
void free_variable(variable *var);
void free_term(term *t);
void free_act_list(act_list *al);
void free_exp_list(expression_list *exp_list);
void free_exp(expression *exp);
void free_class_decl_list(class_decl_list *cdl);
void free_class_decl(class_decl *cdl);

#endif

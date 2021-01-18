#ifndef COMPILER_WEED_H
#define COMPILER_WEED_H

/*
 * The first weed phase will make sure each function contains a return stm
 * and every function has a tail with id = head->id.
 * The weeder also check if continue and break stms are inside loops.
 */

#include "../parser/tree.h"

void first_weed(body *b);

void f_weed_function(function *f);
void f_weed_body(body *b);
void f_weed_head(head *h);
void f_weed_type(type *t);
void f_weed_par_decl_list(par_decl_list *pdl);
void f_weed_var_decl_list(var_decl_list *vdl);
void f_weed_var_type(var_type *vt);
void f_weed_statement_list(statement_list *sl);
void f_weed_statement(statement *stm);
void f_weed_decl_list(decl_list *dl);
void f_weed_declaration(declaration *d);
void f_weed_variable(variable *var);
void f_weed_term(term *t);
void f_weed_act_list(act_list *al);
void f_weed_exp_list(expression_list *exp_list);
void f_weed_exp(expression *exp);
void f_weed_switch(switch_stm *s_stm);
void f_weed_switch_case(switch_case *s_case);

#endif


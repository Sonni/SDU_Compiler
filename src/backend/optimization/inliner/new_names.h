#ifndef COMPILER_NEW_NAMES_H
#define COMPILER_NEW_NAMES_H

#include "../../../frontend/parser/tree.h"
#include "../../../frontend/symbol/symbol.h"


void new_name(body *b);

void new_name_function(function *f);
void new_name_body(body *b);
void new_name_head(head *h);
void new_name_type(type *t);
void new_name_par_decl_list(par_decl_list *pdl);
void new_name_var_decl_list(var_decl_list *vdl);
void new_name_var_type(var_type *vt);
void new_name_statement_list(statement_list *sl);
void new_name_statement(statement *stm);
void new_name_decl_list(decl_list *dl);
void new_name_declaration(declaration *d);
void new_name_variable(variable *var);
void new_name_term(term *t);
void new_name_act_list(act_list *al);
void new_name_exp_list(expression_list *exp_list);
void new_name_exp(expression *exp);
void new_name_switch(switch_stm *s_stm);
void new_name_switch_case(switch_case *s_case);

#endif

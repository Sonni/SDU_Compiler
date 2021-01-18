#ifndef COMPILER_CHECK_H
#define COMPILER_CHECK_H
#include "../parser/tree.h"

bool print_errors;

struct vector *temp_func_calls;

void check(body *b);

void check_function(function *f);
void check_body(body *b);
void check_head(head *h);
void check_type(type *t);
void check_par_decl_list(par_decl_list *pdl);
void check_var_decl_list(var_decl_list *vdl);
void check_var_type(var_type *vt);
void check_statement_list(statement_list *sl);
void check_statement(statement **stm_p);
void check_decl_list(decl_list *dl);
void check_declaration(declaration *d);
void check_variable(variable *var);
int check_term(term *t);
void check_act_list(act_list *al);
void check_exp_list(expression_list *exp_list);
void check_exp(expression *exp);
void check_switch(switch_stm *s_stm);
void check_switch_case(switch_case *s_case);

bool check_exp_is_templated(expression *exp);

#endif

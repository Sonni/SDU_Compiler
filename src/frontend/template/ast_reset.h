#ifndef COMPILER_AST_RESET
#define COMPILER_AST_RESET

#include "../parser/tree.h"

void reset(body *b);

void reset_function(function *f);
void reset_body(body *b);
void reset_head(head *h);
void reset_type(type *t);
void reset_par_decl_list(par_decl_list *pdl);
void reset_var_decl_list(var_decl_list *vdl);
void reset_var_type(var_type *vt);
void reset_statement_list(statement_list *sl);
void reset_statement(statement *stm_p);
void reset_decl_list(decl_list *dl);
void reset_declaration(declaration *d);
void reset_variable(variable *var);
void reset_term(term *t);
void reset_act_list(act_list *al);
void reset_exp_list(expression_list *exp_list);
void reset_exp(expression *exp);
void reset_switch(switch_stm *s_stm);
void reset_switch_case(switch_case *s_case);

#endif

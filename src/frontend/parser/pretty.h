#ifndef COMPILER_PRETTY_H
#define COMPILER_PRETTY_H
#include "tree.h"

void pretty(body *b);

void pretty_function(function *f);
void pretty_body(body *b);
void pretty_head(head *h);
void pretty_tail(tail *t);
void pretty_type(type *t);
void pretty_par_decl_list(par_decl_list *pdl);
void pretty_var_decl_list(var_decl_list *vdl);
void pretty_var_type(var_type *vt);
void pretty_statement_list(statement_list *sl);
void pretty_statement(statement *stm);
void pretty_decl_list(decl_list *dl);
void pretty_declaration(declaration *d);
void pretty_variable(variable *var);
void pretty_term(term *t);
void pretty_act_list(act_list *al);
void pretty_exp_list(expression_list *exp_list);
void pretty_exp(expression *exp);
void pretty_switch(switch_stm *s_stm);
void pretty_switch_case(switch_case *s_case);
void pretty_template(template *temp);
void pretty_tl(temp_list *tl);
void pretty_tdl(temp_decl_list *tdl);

void pretty_temp_args(temp_args *temp_args);
void pretty_tal(temp_args_list *tal);
void pretty_tadl(temp_args_decl_list *tadl);

void pretty_sym_type(struct symbol_type *s);
void print_indentation();

#endif

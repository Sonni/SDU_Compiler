#ifndef COMPILER_BASIC_CLASS_OP_REWRITE
#define COMPILER_BASIC_CLASS_OP_REWRITE

#include "../parser/tree.h"

void basic_class_op_rewrite(body *b);

void basic_class_op_rewrite_function(function *f);
void basic_class_op_rewrite_body(body *b);
void basic_class_op_rewrite_head(head *h);
void basic_class_op_rewrite_tail(tail *t);
void basic_class_op_rewrite_type(type *t);
void basic_class_op_rewrite_par_decl_list(par_decl_list *pdl);
void basic_class_op_rewrite_var_decl_list(var_decl_list *vdl);
void basic_class_op_rewrite_var_type(var_type *vt);
void basic_class_op_rewrite_statement_list(statement_list *sl);
void basic_class_op_rewrite_statement(statement *stm);
void basic_class_op_rewrite_decl_list(decl_list *dl);
void basic_class_op_rewrite_declaration(declaration *d);
void basic_class_op_rewrite_variable(variable *var);
void basic_class_op_rewrite_term(term *t);
void basic_class_op_rewrite_act_list(act_list *al);
void basic_class_op_rewrite_exp_list(expression_list *exp_list);
void basic_class_op_rewrite_exp(expression *exp);
void basic_class_op_rewrite_switch(switch_stm *s_stm);
void basic_class_op_rewrite_switch_case(switch_case *s_case);

char *class_op_calc_new_name_call(char *old_name, act_list *al);
char *class_op_calc_new_name_func(char *old_name, par_decl_list *pdl);

#endif

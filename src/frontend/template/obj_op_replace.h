#ifndef COMPILER_OBJ_OP_REPLACE
#define COMPILER_OBJ_OP_REPLACE

#include "../parser/tree.h"

void obj_op_replace(body *b);

void obj_op_replace_function(function *f);
void obj_op_replace_body(body *b);
void obj_op_replace_head(head *h);
void obj_op_replace_tail(tail *t);
void obj_op_replace_type(type *t);
void obj_op_replace_par_decl_list(par_decl_list *pdl);
void obj_op_replace_var_decl_list(var_decl_list *vdl);
void obj_op_replace_var_type(var_type *vt);
void obj_op_replace_statement_list(statement_list *sl);
void obj_op_replace_statement(statement *stm);
void obj_op_replace_decl_list(decl_list *dl);
void obj_op_replace_declaration(declaration *d);
void obj_op_replace_variable(variable *var);
void obj_op_replace_term(term *t);
void obj_op_replace_act_list(act_list *al);
void obj_op_replace_exp_list(expression_list *exp_list);
void obj_op_replace_exp(expression *exp);
void obj_op_replace_switch(switch_stm *s_stm);
void obj_op_replace_switch_case(switch_case *s_case);

void gen_func_call(expression *exp, char *func_name, int exp_sym_type);


#endif

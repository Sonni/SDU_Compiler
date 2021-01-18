#ifndef COMPILER_GEN_CLASS
#define COMPILER_GEN_CLASS

#include "../parser/tree.h"

void insert_new_class(declaration *d, decl_list *dl);
vector *gen_class(body *b);
declaration *gen_new_class(declaration *d, type *t);

void gen_class_function(function *f, char *type_name, type *t1);
void gen_class_body(body *b, char *type_name, type *t1);
void gen_class_head(head *h, char *type_name, type *t1);
void gen_class_type(type *t, char *type_name, type *t1);
void gen_class_par_decl_list(par_decl_list *pdl, char *type_name, type *t1);
void gen_class_var_decl_list(var_decl_list *vdl, char *type_name, type *t1);
void gen_class_var_type(var_type *vt, char *type_name, type *t1);
void gen_class_statement_list(statement_list *sl);
void gen_class_statement(statement *stm_p);
void gen_class_decl_list(decl_list *dl, char *type_name, type *t1);
void gen_class_declaration(declaration *d, char *type_name, type *t1);
void gen_class_variable(variable *var);
void gen_class_term(term *t);
void gen_class_act_list(act_list *al);
void gen_class_exp_list(expression_list *exp_list);
void gen_class_exp(expression *exp);
void gen_class_switch(switch_stm *s_stm);
void gen_class_switch_case(switch_case *s_case);

void gen_class_tl(temp_list *tl, declaration *d, temp_args_decl_list *tadl);
void gen_class_tdl(temp_decl_list *tdl, declaration *d, temp_args_decl_list *tadl);
char *gen_final_class_name(char *name, type *t);

#endif

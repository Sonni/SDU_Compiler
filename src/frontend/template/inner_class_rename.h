#ifndef COMPILER_INNER_CLASS_RENAME
#define COMPILER_INNER_CLASS_RENAME

#include "../parser/tree.h"

void inner_class_rename(body *b, char *old_name, char *new_name);

void inner_class_rename_function(function *f, char *old_name, char *new_name);
void inner_class_rename_body(body *b, char *old_name, char *new_name);
void inner_class_rename_head(head *h, char *old_name, char *new_name);
void inner_class_rename_tail(tail *t, char *old_name, char *new_name);
void inner_class_rename_type(type *t, char *old_name, char *new_name);
void inner_class_rename_par_decl_list(par_decl_list *pdl, char *old_name, char *new_name);
void inner_class_rename_var_decl_list(var_decl_list *vdl, char *old_name, char *new_name);
void inner_class_rename_var_type(var_type *vt, char *old_name, char *new_name);
void inner_class_rename_statement_list(statement_list *sl, char *old_name, char *new_name);
void inner_class_rename_statement(statement *stm, char *old_name, char *new_name);
void inner_class_rename_decl_list(decl_list *dl, char *old_name, char *new_name);
void inner_class_rename_declaration(declaration *d, char *old_name, char *new_name);
void inner_class_rename_variable(variable *var, char *old_name, char *new_name);
void inner_class_rename_term(term *t, char *old_name, char *new_name);
void inner_class_rename_act_list(act_list *al, char *old_name, char *new_name);
void inner_class_rename_exp_list(expression_list *exp_list, char *old_name, char *new_name);
void inner_class_rename_exp(expression *exp, char *old_name, char *new_name);
void inner_class_rename_switch(switch_stm *s_stm, char *old_name, char *new_name);
void inner_class_rename_switch_case(switch_case *s_case, char *old_name, char *new_name);



#endif

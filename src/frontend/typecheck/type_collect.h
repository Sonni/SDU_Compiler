#ifndef COMPILER_COLLECT_H
#define COMPILER_COLLECT_H
#include "../parser/tree.h"
#include "../symbol/symbol.h"

struct vector *func_decl_list;
struct vector *temp_class_type;
struct vector *temp_class_decl;


char *get_types(par_decl_list *pdl);
char *get_types_exp_list(act_list *al);

void collect_reset();
void collect(body *b);

void collect_function(function *f, symbol_table *st);
void collect_body(body *b, symbol_table *st);
void collect_head(head *h, symbol_table *st, symbol_table *p);
void collect_type(type *t, symbol_table *st);
int collect_par_decl_list(par_decl_list *pdl, symbol_table *st);
void collect_var_decl_list(var_decl_list *vdl, symbol_table *st,
                           int *num_args);
void collect_var_type(var_type *vt, symbol_table *st, int offset);
void collect_statement_list(statement_list *sl, symbol_table *st);
void collect_statement(statement *stm, symbol_table *st);
void collect_decl_list(decl_list *dl, symbol_table *st);
void collect_declaration(declaration *d, symbol_table *st);
void collect_variable(variable *var, symbol_table *st, statement *stm);
void collect_term(term *t, symbol_table *st, statement *stm);
void collect_act_list(act_list *al, symbol_table *st, statement *stm);
void collect_exp_list(expression_list *exp_list, symbol_table *st, statement *stm);
void collect_exp(expression *exp, symbol_table *st, statement *stm);
void collect_switch(switch_stm *s_stm, symbol_table *st, statement *stm);
void collect_switch_case(switch_case *s_case, symbol_table *st, statement *stm);



void collect_template(template *temp);
void collect_temp_args(temp_args *temp_args, symbol_table *st);

void collect_tal(temp_args_list *tal, symbol_table *st);

void collect_tadl(temp_args_decl_list *tadl, symbol_table *st);

#endif

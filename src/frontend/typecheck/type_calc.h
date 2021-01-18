#ifndef COMPILER_TYPE_CALC
#define COMPILER_TYPE_CALC

#include "../parser/tree.h"

void calc(body *b);

void calc_function(function *f, bool second_pass);
void calc_body(body *b, bool second_pass);
void calc_head(head *h, bool is_templated, bool second_pass, bool was_templated);
void calc_type(type *t);
void calc_par_decl_list(par_decl_list *pdl);
void calc_var_decl_list(var_decl_list *vdl);
void calc_statement_list(statement_list *sl);
void calc_statement(statement *stm);
void calc_var_type(var_type *vt);
void calc_decl_list(decl_list *dl, bool second_pass);
void calc_declaration(declaration *d);

type *find_type(type *t);

void calc_tadl(temp_args_decl_list *tadl);
void calc_tal(temp_args_list *tal);
void calc_temp_args(temp_args *temp_args);
void calc_exp(expression *exp);
void calc_term(term *t);

#endif

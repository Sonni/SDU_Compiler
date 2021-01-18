#ifndef COMPILER_GEN_TEMPLATE
#define COMPILER_GEN_TEMPLATE

#include "../parser/tree.h"

void delete_templates(body *b);
void insert_new_func(function *func, decl_list *dl);

bool is_match(term *t, decl_list *dl);
vector *gen_template();
function *gen_new_function(decl_list *dl, term *t);
char *gen_final_name(function *func, term *t);

void gen_head(head *head, char *type_name, type *type);
void gen_type(type *t1, char *type_name, type *t2);
void gen_body(body *b, char *type_name, type *t1);
void gen_function(function *func, char *type_name, type *t1);

void gen_decl_list(decl_list *dl, char *type_name, type *t1);
void gen_decl(declaration *d, char *type_name, type *t1);


void gen_par_decl_list(par_decl_list *pdl, char *type_name, type *type);
void gen_var_decl_list(var_decl_list *vdl, char *type_name, type *type);

void gen_tl(temp_list *tl, function *func, temp_args_decl_list* tadl);
void gen_tdl(temp_decl_list *tdl, function *func, temp_args_decl_list *tadl);
#endif

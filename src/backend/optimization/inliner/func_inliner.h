#ifndef COMPILER_FUNC_INLINER_H
#define COMPILER_FUNC_INLINER_H


#include "../../../frontend/parser/tree.h"
#include "../../../frontend/symbol/symbol.h"

#define MAX_STM_TIMES_CALLS 200

void inliner_inline(body *b);

void inliner_inline_function(function *f);
void inliner_inline_body(body *b);
void inliner_inline_statement_list(statement_list *sl);
void inliner_inline_statement(statement *stm);
void inliner_inline_decl_list(decl_list *dl);
void inliner_inline_declaration(declaration *d);
void inliner_inline_variable(variable *var);
void inliner_inline_term(term *t);
void inliner_inline_exp(expression *exp);
void inliner_inline_exp_list(expression_list *exp_list);
void inliner_inline_switch(switch_stm *s_stm);
void inliner_inline_switch_case(switch_case *s_case);

void inliner_collect_program(body *b);
void inliner_collect_function(function *f);
void inliner_collect_body(body *b);
void inliner_collect_statement_list(statement_list *sl);
void inliner_collect_statement(statement *stm);
void inliner_collect_decl_list(decl_list *dl);
void inliner_collect_declaration(declaration *d);
void inliner_collect_variable(variable *var);
void inliner_collect_term(term *t);
void inliner_collect_exp(expression *exp);
void inliner_collect_exp_list(expression_list *exp_list);
void inliner_collect_switch(switch_stm *s_stm);
void inliner_collect_switch_case(switch_case *s_case);

void inliner_rm_body(body *b);
decl_list *inliner_rm_decl_list(decl_list *dl);
declaration *inliner_rm_declaration(declaration *d);

#endif

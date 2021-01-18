#ifndef COMPILER_ASM_GEN_H
#define COMPILER_ASM_GEN_H
#include "asm.h"
#include "../../frontend/parser/tree.h"

typedef struct switch_info
{
    statement *stm;
    char *jmp_label;
} switch_info;

abstract_asm *asm_gen(body *b);

abstract_asm *asm_function(function *f);
abstract_asm *asm_body(body *b);
abstract_asm *asm_head(head *h);
abstract_asm *asm_par_decl_list(par_decl_list *pdl);
abstract_asm *asm_var_decl_list(var_decl_list *vdl, int offset);

abstract_asm *asm_type(type *t);
abstract_asm *asm_var_type(var_type *vt);

abstract_asm *asm_statement_list(statement_list *sl);
abstract_asm *asm_statement(statement *stm);
abstract_asm *asm_decl_list(decl_list *dl);
abstract_asm *asm_declaration(declaration *d);
abstract_asm *asm_variable(variable *var);
abstract_asm *asm_term(term *t);
abstract_asm *asm_exp(expression *exp);
abstract_asm *asm_switch(switch_stm *s_stm, operand *switch_val,
                         operand *switch_end);
abstract_asm *asm_switch_case(switch_case *s_case, operand *switch_val);

void asm_and_or(int kind, operand *l, expression *exp, abstract_asm **list,
                abstract_asm **top);
void asm_general_exp(int kind, operand *l, operand *r, abstract_asm *list,
                     abstract_asm *top);
int get_local_vars(decl_list *dl, int offset);
abstract_asm *zero_init_local_vars(decl_list *dl);
abstract_asm *asm_div(operand *l, operand *r);
abstract_asm *asm_printf(operand *arg3, operand *arg2, operand *arg1);

#endif

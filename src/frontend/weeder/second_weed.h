#ifndef COMPILER_SECOND_WEED_H
#define COMPILER_SECOND_WEED_H

/*
 * The second weed phases removes unnecessary code and does algebraic
 * simplifications.
 */

#include "../parser/tree.h"

void second_weed(body *b);

void s_weed_function(function *f);
void s_weed_body(body *b);
void s_weed_head(head *h);
void s_weed_type(type *t);
void s_weed_par_decl_list(par_decl_list *pdl);
void s_weed_var_decl_list(var_decl_list *vdl);
void s_weed_var_type(var_type *vt);
void s_weed_statement_list(statement_list *sl_p);

/*
 * Remove code after return, skip and continue
 */
void s_weed_statement(statement **stm);

void s_weed_decl_list(decl_list *dl);
void s_weed_declaration(declaration *d);
void s_weed_variable(variable *var);

/*
 * !true    -> false
 * !false   -> true
 * (term)   -> term
 * De morgans law
 * |int|    -> abs(int)
 * !!a      -> a
 */
void s_weed_term(term *t);

void s_weed_act_list(act_list *al);
void s_weed_exp_list(expression_list *exp_list);

/*
 * Fold constant
 * Fold compare constants
 *
 * False &&  exp   = false
 * True  &&  exp   = exp
 * True  &&  True  = true
 * False ||  exp   = exp
 * True  ||  exp   = true
 * False ||  False = false
 */
void s_weed_exp(expression *exp);
void s_weed_switch(switch_stm *s_stm);
void s_weed_switch_case(switch_case *s_case);

void s_weed_de_morgan(term *t, int new_kind, bool not_opds);
void fold_constant(expression *exp, int result);
void fold_constant_op(expression *exp, bool condition);

#endif

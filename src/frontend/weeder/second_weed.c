#include <string.h>
#include <stdlib.h>

#include "second_weed.h"
#include "../../utils/string_def.h"

void second_weed(body *b)
{
    s_weed_body(b);
}

void s_weed_function(function *f)
{
    s_weed_head(f->head);
    s_weed_body(f->body);
}

void s_weed_body(body *b)
{
    s_weed_decl_list(b->dl);
    s_weed_statement_list(b->sl);
}

void s_weed_head(head *h)
{
    s_weed_par_decl_list(h->pdl);
    s_weed_type(h->type);
}

void s_weed_type(type *t)
{
    switch (t->kind)
    {
        case TYPE_ID:
        case TYPE_INT:
        case TYPE_FLOAT:
        case TYPE_BOOL:
        case TYPE_STRING:
            break;
        case TYPE_ARRAY:
            s_weed_type(t->val.type);
            break;
        case TYPE_RECORD:
            s_weed_var_decl_list(t->val.vdl);
            break;
        case TYPE_CLASS:
            break;
    }
}

void s_weed_par_decl_list(par_decl_list *pdl)
{
    switch (pdl->kind)
    {
        case PDL_VDL:
            s_weed_var_decl_list(pdl->vdl);
            break;
        case PDL_EMPTY:
            break;
    }
}

void s_weed_var_decl_list(var_decl_list *vdl)
{
    switch (vdl->kind)
    {
        case VDL_VT_VDL:
            s_weed_var_type(vdl->vt);
            s_weed_var_decl_list(vdl->vdl);
            break;
        case VDL_VT:
            s_weed_var_type(vdl->vt);
            break;
    }
}

void s_weed_var_type(var_type *vt)
{
    s_weed_type(vt->type);
}

void s_weed_statement_list(statement_list *sl)
{
    if (sl == NULL)
        return;

    s_weed_statement_list(sl->sl);
    s_weed_statement(&sl->stm);
    if (sl->stm->has_return)
    {
        // Remove code after return statements
        print_warning(NO_EXE_AFTER_RETURN, sl->line_no);
        sl->kind = SL_STM;

    }
    else if (sl->stm->has_skip)
    {
        // Remove code after break and return
        print_warning(NO_EXE_AFTER_RETURN, sl->line_no);
        sl->kind = SL_STM;
    }
    else
    {
        s_weed_statement_list(sl->sl);
    }
    return;
}

void s_weed_statement(statement **stm_p)
{
    statement *stm = (*stm_p);
    switch (stm->kind) {
        case STM_RETURN:
            s_weed_exp(stm->val.exp);
            break;
        case STM_WRITE:
            s_weed_exp(stm->val.exp);
            break;
        case STM_ALLOCATE:
            s_weed_variable(stm->val.var);
            break;
        case STM_ALLOCATE_OF_LENGTH:
            s_weed_variable(stm->val.assignment.var);
            s_weed_exp(stm->val.assignment.exp);
            break;
        case STM_ASSIGNMENT:
            s_weed_variable(stm->val.assignment.var);
            s_weed_exp(stm->val.assignment.exp);
            break;
        case STM_ASSIGN_LIST:
            break;
        case STM_IF:
            s_weed_exp(stm->val.if_then.exp);
            s_weed_statement(&stm->val.if_then.stm);

            if (stm->val.if_then.exp->kind == EXP_TERM)
            {
                if (stm->val.if_then.exp->val.term->kind == TERM_FALSE)
                {
                    // Remove the statement and expression
                    print_warning(IF_TRUE, stm->line_no);

                    stm->kind = -1;
                }
                else if (stm->val.if_then.exp->val.term->kind == TERM_TRUE)
                {
                    // Remove the statement
                    print_warning(IF_FALSE, stm->line_no);
                    (*stm_p) = stm->val.if_then.stm;
                }
            }
            break;
        case STM_ELSE:
            s_weed_exp(stm->val.if_then.exp);
            s_weed_statement(&stm->val.if_else.stm1);
            s_weed_statement(&stm->val.if_else.stm2);
            // Check for
            if (stm->val.if_else.exp->kind == EXP_TERM)
            {
                if (stm->val.if_else.exp->val.term->kind == TERM_FALSE)
                {
                    print_warning(IF_FALSE, stm->line_no);
                    // Remove stm1 and if else
                    (*stm_p) = stm->val.if_else.stm2;

                }
                else if (stm->val.if_else.exp->val.term->kind == TERM_TRUE)
                {
                    print_warning(IF_TRUE, stm->line_no);

                    // Remove stm2 and if else
                    (*stm_p) = stm->val.if_else.stm1;
                }
            }
            break;
        case STM_WHILE:
            s_weed_exp(stm->val.while_do.exp);
            s_weed_statement(&stm->val.while_do.stm);
            if (stm->val.while_do.exp->kind == EXP_TERM)
            {
                if (stm->val.while_do.exp->val.term->kind == TERM_FALSE)
                {
                    stm->kind = -1;
                }
            }
            break;
        case STM_LIST:
            s_weed_statement_list(stm->val.sl);
            if (stm->val.sl->stm != NULL && stm->val.sl->stm->kind == STM_LIST)
            {
                stm->val = stm->val.sl->stm->val;
            }
            break;
        case STM_FREE:
            s_weed_variable(stm->val.var);
            break;
        case STM_BREAK:
        case STM_CONTINUE:
        case STM_IN:
            break;
        case STM_ASSIGN_VAL:
            s_weed_variable(stm->val.assignment.var);
            s_weed_exp(stm->val.assignment.exp);
            break;
        case STM_SWITCH:
            s_weed_exp(stm->val.switch_stm.exp);
            s_weed_switch(stm->val.switch_stm.s_stm);
            break;
    }
}

void s_weed_decl_list(decl_list *dl)
{
    switch (dl->kind)
    {
        case DL_DECL_DL:
            s_weed_decl_list(dl->dl);
            s_weed_declaration(dl->decl);
            break;
        case DL_EMPTY:
            break;
    }
}

void s_weed_declaration(declaration *d)
{
    switch (d->kind)
    {
        case DECL_TYPE:
            s_weed_type(d->val.type.type);
            break;
        case DECL_FUNC:
            s_weed_function(d->val.func);
            break;
        case DECL_VDL:
            s_weed_var_decl_list(d->val.vdl);
            break;
        case DECL_CONST:
            s_weed_type(d->val.con.type);
            s_weed_exp(d->val.con.exp);
            break;
    }
}

void s_weed_variable(variable *var)
{
    switch (var->kind)
    {
        case VAR_ID:
            break;
        case VAR_EXP:
            s_weed_variable(var->val.var_exp.var);
            s_weed_exp_list(var->val.var_exp.exp);
            break;
        case VAR_RECORD:
            s_weed_variable(var->val.var_exp.var);
            break;
        case VAR_CLASS:
            break;
        case VAR_FUNC:
            break;
    }
}

void s_weed_term(term *t)
{
    switch (t->kind)
    {
        case TERM_VAR:
            s_weed_variable(t->val.var);
            break;
        case TERM_AL:
            s_weed_act_list(t->val.term_act_list.al);
            break;
        case TERM_PARENTHESES:
            s_weed_exp(t->val.exp);
            if (t->val.exp->kind == EXP_TERM)
            {
                if (t->val.exp->val.term->kind == TERM_VAR ||
                    t->val.exp->val.term->kind == TERM_PARENTHESES ||
                    t->val.exp->val.term->kind == TERM_INT ||
                    t->val.exp->val.term->kind == TERM_FLOAT ||
                    t->val.exp->val.term->kind == TERM_TRUE ||
                    t->val.exp->val.term->kind == TERM_FALSE ||
                    t->val.exp->val.term->kind == TERM_NULL)
                {
                    t->kind = t->val.exp->val.term->kind;
                    t->val = t->val.exp->val.term->val;
                }
            }
            break;
        case TERM_NOT:
            s_weed_term(t->val.not_term);
            if (t->val.not_term->kind == TERM_NOT)
            {
                t->kind = t->val.not_term->val.not_term->kind;
                t->val = t->val.not_term->val.not_term->val;
            }
            else if (t->val.not_term->kind == TERM_TRUE)
            {
                t->kind = TERM_FALSE;
            }
            else if (t->val.not_term->kind == TERM_FALSE)
            {
                t->kind = TERM_TRUE;
            }
            else if (t->val.not_term->kind == TERM_PARENTHESES)
            {
                if (t->val.not_term->val.exp->kind == EXP_OR)
                {
                    s_weed_de_morgan(t, EXP_AND, false);
                }
                else if (t->val.not_term->val.exp->kind == EXP_AND)
                {
                    s_weed_de_morgan(t, EXP_OR, true);
                }
                else if (t->val.not_term->val.exp->kind == EXP_EQ)
                {
                    s_weed_de_morgan(t, EXP_NEQ, false);
                }
                else if (t->val.not_term->val.exp->kind == EXP_NEQ)
                {
                    s_weed_de_morgan(t, EXP_EQ, false);
                }
                else if (t->val.not_term->val.exp->kind == EXP_GREATER)
                {
                    s_weed_de_morgan(t, EXP_LEQ, false);
                }
                else if (t->val.not_term->val.exp->kind == EXP_GEQ)
                {
                    s_weed_de_morgan(t, EXP_LESS, false);
                }
                else if (t->val.not_term->val.exp->kind == EXP_LESS)
                {
                    s_weed_de_morgan(t, EXP_GEQ, false);
                }
                else if (t->val.not_term->val.exp->kind == EXP_LEQ)
                {
                    s_weed_de_morgan(t, EXP_GREATER, false);
                }
            }
            break;
        case TERM_ABS:
            s_weed_exp(t->val.exp);
            if (t->val.exp->kind == EXP_TERM &&
                t->val.exp->val.term->kind == TERM_INT)
            {
                t->kind = TERM_INT;
                t->val.integer = abs(t->val.exp->val.term->val.integer);
            }
            break;
        case TERM_INT:
        case TERM_FLOAT:
        case TERM_TRUE:
        case TERM_FALSE:
        case TERM_NULL:
        case TERM_STRING:
            break;
    }
}

void s_weed_act_list(act_list *al)
{
    switch (al->kind) {
        case AL_EXP_LIST:
            s_weed_exp_list(al->exp_list);
            break;
        case AL_EMPTY:
            break;
    }
}

void s_weed_exp_list(expression_list *exp_list)
{
    if (exp_list == NULL)
        return;
    s_weed_exp(exp_list->exp);
    s_weed_exp_list(exp_list->exp_list);
    return;
}

void print_exp_warn(int line_no)
{
    print_warning(EXP_MERGE, line_no);
}

void s_weed_exp(expression *exp)
{
    if (exp->kind == EXP_TERM)
    {
        s_weed_term(exp->val.term);
    }

    if (exp->val.op.left != NULL && exp->val.op.right != NULL)
    {
        expression *l = exp->val.op.left;
        expression *r = exp->val.op.right;
        switch (exp->kind)
        {
            case EXP_TERM:
                break;
            case EXP_TIMES:
                s_weed_exp(l);
                s_weed_exp(r);
                fold_constant(exp,
                          l->val.term->val.integer * r->val.term->val.integer);
                break;
            case EXP_DIV:
                s_weed_exp(l);
                s_weed_exp(r);
                fold_constant(exp,
                          l->val.term->val.integer / r->val.term->val.integer);
                break;
            case EXP_PLUS:
                s_weed_exp(l);
                s_weed_exp(r);
                fold_constant(exp,
                          l->val.term->val.integer + r->val.term->val.integer);
                break;
            case EXP_MINUS:
                s_weed_exp(l);
                s_weed_exp(r);
                fold_constant(exp,
                          l->val.term->val.integer - r->val.term->val.integer);
                break;
            case EXP_EQ:
                s_weed_exp(l);
                s_weed_exp(r);
                fold_constant_op(exp,
                       (l->val.term->val.integer == r->val.term->val.integer));
                break;
            case EXP_NEQ:
                s_weed_exp(l);
                s_weed_exp(r);
                fold_constant_op(exp,
                       (l->val.term->val.integer != r->val.term->val.integer));
                break;
            case EXP_GREATER:
                s_weed_exp(l);
                s_weed_exp(r);
                fold_constant_op(exp,
                        (l->val.term->val.integer > r->val.term->val.integer));
                break;
            case EXP_LESS:
                s_weed_exp(l);
                s_weed_exp(r);
                fold_constant_op(exp,
                        (l->val.term->val.integer < r->val.term->val.integer));
                break;
            case EXP_GEQ:
                s_weed_exp(l);
                s_weed_exp(r);
                fold_constant_op(exp,
                       (l->val.term->val.integer >= r->val.term->val.integer));
                break;
            case EXP_LEQ:
                s_weed_exp(l);
                s_weed_exp(r);
                fold_constant_op(exp,
                       (l->val.term->val.integer <= r->val.term->val.integer));
                break;
            case EXP_AND:
                s_weed_exp(l);
                s_weed_exp(r);
                if ((l->val.term->kind == TERM_FALSE ||
                     l->val.term->kind == TERM_TRUE) &&
                    (r->val.term->kind == TERM_FALSE ||
                     r->val.term->kind == TERM_TRUE)) {

                    if (l->val.term->kind == TERM_TRUE &&
                        r->val.term->kind == TERM_TRUE) {
                        exp->kind = EXP_TERM;
                        exp->val.term->kind = TERM_TRUE;
                    } else {
                        exp->kind = EXP_TERM;
                        exp->val.term->kind = TERM_FALSE;
                    }
                    print_exp_warn(exp->line_no);
                }
                else // Either left or right is not a bool
                {
                    if (l->val.term->kind == TERM_FALSE ||
                        l->val.term->kind == TERM_TRUE)
                    {
                        if (l->val.term->kind == TERM_FALSE)
                        {
                            exp->kind = EXP_TERM;
                            exp->val.term->kind = TERM_FALSE;
                        }
                        else if (l->val.term->kind == TERM_TRUE)
                        {
                            exp->kind = r->kind;
                            exp->val = r->val;
                        }
                        print_exp_warn(exp->line_no);
                    }
                    else // right is not a bool
                    {
                        if (r->val.term->kind == TERM_FALSE)
                        {
                            exp->kind = EXP_TERM;
                            exp->val.term->kind = TERM_FALSE;
                        }
                        else if (r->val.term->kind == TERM_TRUE)
                        {
                            exp->kind = l->kind;
                            exp->val = l->val;
                        }
                        print_exp_warn(exp->line_no);
                    }
                }

                break;
            case EXP_OR:
                s_weed_exp(l);
                s_weed_exp(r);

                if (l->val.term->kind == TERM_TRUE ||
                    r->val.term->kind == TERM_TRUE)
                {
                    exp->kind = EXP_TERM;
                    exp->val.term->kind = TERM_TRUE;
                    print_exp_warn(exp->line_no);
                }
                else if (l->val.term->kind == TERM_FALSE &&
                        r->val.term->kind == TERM_FALSE)
                {
                    exp->kind = EXP_TERM;
                    exp->val.term->kind = TERM_FALSE;
                    print_exp_warn(exp->line_no);
                }
                else // Either left or right is not a boolean
                {
                    if (l->val.term->kind == TERM_FALSE ||
                        l->val.term->kind == TERM_TRUE)
                    { // then r is not a bool
                        exp->kind = r->kind;
                        exp->val = r->val;
                    }
                    else if (r->val.term->kind == TERM_FALSE ||
                             r->val.term->kind == TERM_TRUE)
                    {
                        exp->kind = l->kind;
                        exp->val = l->val;
                    }
                    print_exp_warn(exp->line_no);
                }
                break;
        }
    }
}

void s_weed_de_morgan(term *t, int new_kind, bool not_opds)
{
    term *left, *right;

    t->kind = TERM_PARENTHESES;
    t->val.exp = t->val.not_term->val.exp;
    if (not_opds)
    {
        left = make_term_not(make_term_parentheses(t->val.exp->val.op.left));
        right = make_term_not(make_term_parentheses(t->val.exp->val.op.right));
        t->val.exp->val.op.left = make_exp_term(left);
        t->val.exp->val.op.right = make_exp_term(right);
    }
    t->val.exp->kind = new_kind;
    s_weed_term(t);
}


/*
 * const_x OP const_y = const_z
 */
void fold_constant(expression *exp, int result)
{
    if (exp->val.op.left->val.term->kind == TERM_INT &&
        exp->val.op.right->val.term->kind == TERM_INT)
    {
        exp->kind = EXP_TERM;
        exp->val.term->kind = TERM_INT;
        exp->val.term->val.integer = result;
        print_exp_warn(exp->line_no);
    }
}

/*
 * const_x op const_y = true/false
 */
void fold_constant_op(expression *exp, bool condition)
{
    if (exp->val.op.left->val.term->kind == TERM_INT &&
        exp->val.op.right->val.term->kind == TERM_INT)
    {
        if (condition)
        {
            exp->kind = EXP_TERM;
            exp->val.term->kind = TERM_TRUE;
        }
        else
        {
            exp->kind = EXP_TERM;
            exp->val.term->kind = TERM_FALSE;
        }
        print_exp_warn(exp->line_no);
    }
}

void s_weed_switch(switch_stm *s_stm)
{
    if (s_stm == NULL)
    {
        return;
    }
    s_weed_switch_case(s_stm->switch_case);
    s_weed_switch(s_stm->switch_list);
}

void s_weed_switch_case(switch_case *s_case)
{
    s_weed_exp(s_case->exp);
    s_weed_statement(&s_case->stm);
}

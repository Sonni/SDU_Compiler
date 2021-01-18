#include <string.h>

#include "first_weed.h"
#include "../../utils/vector.h"
#include "../../utils/string_def.h"
#include "../parser/pretty.h"
vector *v;

statement_list *create_sl(statement *stm, statement_list *sl)
{
    return make_sl_statement_list(stm, sl);
}

void first_weed(body *b)
{
    v = vector_init();

    f_weed_body(b);
}

void f_weed_function(function *f)
{
    if (strcmp(f->head->id, f->tail->id) != 0)
    {
        print_error(MISMATCH_HEAD_TAIL_ID, f->head->line_no, 1);
    }
    else
    {
        f_weed_head(f->head);
        f_weed_body(f->body);
        statement_list *sl = f->body->sl;

        // Check if the function contains a return
        // (return in both if and else will also be accepted)
        bool has_return = false;
        while (sl != NULL)
        {
            if (sl->stm->has_return)
            {
                has_return = true;
            }
            sl = sl->sl;
        }
        if (!has_return)
        {
            print_error(NO_RETURN, f->head->line_no, 1);
        }
    }
}

void f_weed_body(body *b)
{
    f_weed_decl_list(b->dl);
    f_weed_statement_list(b->sl);
}

void f_weed_head(head *h)
{
    f_weed_par_decl_list(h->pdl);
    f_weed_type(h->type);
}

void f_weed_type(type *t)
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
            f_weed_type(t->val.type);
            break;
        case TYPE_RECORD:
            f_weed_var_decl_list(t->val.vdl);
            break;
        case TYPE_CLASS:
            f_weed_decl_list(t->val._class.dl);
            break;
    }
}

void f_weed_par_decl_list(par_decl_list *pdl)
{
    switch (pdl->kind)
    {
        case PDL_VDL:
            f_weed_var_decl_list(pdl->vdl);
            break;
        case PDL_EMPTY:
            break;
    }
}

void f_weed_var_decl_list(var_decl_list *vdl)
{
    switch (vdl->kind)
    {
        case VDL_VT_VDL:
            f_weed_var_type(vdl->vt);
            f_weed_var_decl_list(vdl->vdl);
            break;
        case VDL_VT:
            f_weed_var_type(vdl->vt);
            break;
    }
}

void f_weed_var_type(var_type *vt)
{
    f_weed_type(vt->type);
}

void f_weed_statement_list(statement_list *sl)
{
    if (sl == NULL)
    {
        return;
    }

    switch (sl->kind)
    {
        case SL_STM:
            f_weed_statement(sl->stm);
            break;
        case SL_STM_SL:
            f_weed_statement_list(sl->sl);
            f_weed_statement(sl->stm);
            break;
    }
}

void f_weed_statement(statement *stm)
{
    switch (stm->kind)
    {
        case STM_RETURN:
            f_weed_exp(stm->val.exp);
            stm->has_return = true;
            break;
        case STM_WRITE:
            f_weed_exp(stm->val.exp);
            break;
        case STM_ALLOCATE:
            f_weed_variable(stm->val.var);
            break;
        case STM_ALLOCATE_OF_LENGTH:
            f_weed_variable(stm->val.assignment.var);
            f_weed_exp(stm->val.assignment.exp);
            break;
        case STM_ASSIGNMENT:
            f_weed_variable(stm->val.assignment.var);
            f_weed_exp(stm->val.assignment.exp);
            break;
        case STM_ASSIGN_LIST:
            break;
        case STM_IF:
            f_weed_exp(stm->val.if_then.exp);
            f_weed_statement(stm->val.if_then.stm);
            break;
        case STM_ELSE:
            f_weed_exp(stm->val.if_then.exp);
            f_weed_statement(stm->val.if_else.stm1);
            f_weed_statement(stm->val.if_else.stm2);

            bool stm1_has_return, stm2_has_return;
            statement_list *sl;

            // Check if both if and else contains a return
            stm1_has_return = stm->val.if_else.stm1->has_return;
            if (stm->val.if_else.stm1->kind == STM_LIST) {
                sl = stm->val.if_else.stm1->val.sl;
                while (sl != NULL) {
                    if (sl->stm->has_return) {
                        stm1_has_return = true;
                    }
                    sl = sl->sl;
                }
            }

            stm2_has_return = stm->val.if_else.stm2->has_return;
            if (stm->val.if_else.stm2->kind == STM_LIST) {
                sl = stm->val.if_else.stm2->val.sl;
                while (sl != NULL) {
                    if (sl->stm->has_return) {
                        stm2_has_return = true;
                    }
                    sl = sl->sl;
                }
            }
            if (stm1_has_return && stm2_has_return)
            {
                stm->has_return = true;
            }


            break;
        case STM_WHILE:
            vector_push(v, stm);
            f_weed_exp(stm->val.while_do.exp);
            f_weed_statement(stm->val.while_do.stm);
            vector_pop(v);
            break;
        case STM_LIST:
            f_weed_statement_list(stm->val.sl);
            if (stm->val.sl->stm != NULL)
            {
                stm->has_return = stm->val.sl->stm->has_return;
            }
            break;
        case STM_BREAK:
            if (vector_size(v) == 0)
            {
                print_error(NO_BREAK_CONT_OUTSIDE_LOOP, stm->line_no, 1);
            }
            stm->loop = vector_get_element(v, vector_size(v) - 1);
            stm->has_skip = true;
            break;
        case STM_CONTINUE:
            if (vector_size(v) == 0)
            {
                print_error(NO_BREAK_CONT_OUTSIDE_LOOP, stm->line_no, 1);
            }
            stm->loop = vector_get_element(v, vector_size(v) - 1);
            if (stm->loop->kind == STM_SWITCH)
            {
                print_error("Continue is not allowed in a switch statement", stm->line_no, 1);
            }
            stm->has_skip = true;
            break;
        case STM_FREE:
            f_weed_variable(stm->val.var);
            break;
        case STM_IN:
            break;
        case STM_ASSIGN_VAL:
            f_weed_variable(stm->val.var);
            f_weed_exp(stm->val.exp);
            break;
        case STM_SWITCH:
            vector_push(v, stm);
            stm->loop = stm;
            f_weed_exp(stm->val.switch_stm.exp);
            f_weed_switch(stm->val.switch_stm.s_stm);
            vector_pop(v);
            break;
    }
}

void f_weed_decl_list(decl_list *dl)
{
    switch (dl->kind)
    {
        case DL_DECL_DL:
            f_weed_decl_list(dl->dl);
            f_weed_declaration(dl->decl);
            break;
        case DL_EMPTY:
            break;
    }
}

void f_weed_declaration(declaration *d)
{
    switch (d->kind)
    {
        case DECL_TYPE:
            f_weed_type(d->val.type.type);
            break;
        case DECL_FUNC:
            f_weed_function(d->val.func);
            break;
        case DECL_VDL:
            f_weed_var_decl_list(d->val.vdl);
            break;
        case DECL_CONST:
            f_weed_type(d->val.con.type);
            f_weed_exp(d->val.con.exp);
            break;
    }
}

void f_weed_variable(variable *var)
{
    switch (var->kind)
    {
        case VAR_ID:
            break;
        case VAR_EXP:
            f_weed_variable(var->val.var_exp.var);
            f_weed_exp_list(var->val.var_exp.exp);
            break;
        case VAR_RECORD:
            f_weed_variable(var->val.var_exp.var);
            break;
        case VAR_CLASS:
            break;
        case VAR_FUNC:
            break;
    }
}

void f_weed_term(term *t)
{
    switch (t->kind)
    {
        case TERM_VAR:
            f_weed_variable(t->val.var);
            break;
        case TERM_AL:
            f_weed_act_list(t->val.term_act_list.al);
            break;
        case TERM_PARENTHESES:
            f_weed_exp(t->val.exp);
            break;
        case TERM_NOT:
            f_weed_term(t->val.not_term);
            break;
        case TERM_ABS:
            f_weed_exp(t->val.exp);
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

void f_weed_act_list(act_list *al)
{
    switch (al->kind) {
        case AL_EXP_LIST:
            f_weed_exp_list(al->exp_list);
            break;
        case AL_EMPTY:
            break;
    }
}

void f_weed_exp_list(expression_list *exp_list)
{
    switch (exp_list->kind) {
        case EXP_LIST_EXP:
            f_weed_exp(exp_list->exp);
            break;
        case EXP_LIST_EXP_LIST:
            f_weed_exp_list(exp_list->exp_list);
            f_weed_exp(exp_list->exp);
            break;
    }
}



void f_weed_exp(expression *exp)
{
    if (exp->kind == EXP_TERM)
    {
        f_weed_term(exp->val.term);
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
                f_weed_exp(l);
                f_weed_exp(r);
                break;
            case EXP_DIV:
                f_weed_exp(l);
                f_weed_exp(r);
                break;
            case EXP_PLUS:
                f_weed_exp(l);
                f_weed_exp(r);
                break;
            case EXP_MINUS:
                f_weed_exp(l);
                f_weed_exp(r);
                break;
            case EXP_EQ:
                f_weed_exp(l);
                f_weed_exp(r);
                break;
            case EXP_NEQ:
                f_weed_exp(l);
                f_weed_exp(r);
                break;
            case EXP_GREATER:
                f_weed_exp(l);
                f_weed_exp(r);
                break;
            case EXP_LESS:
                f_weed_exp(l);
                f_weed_exp(r);
                break;
            case EXP_GEQ:
                f_weed_exp(l);
                f_weed_exp(r);
                break;
            case EXP_LEQ:
                f_weed_exp(l);
                f_weed_exp(r);
                break;
            case EXP_AND:
                f_weed_exp(l);
                f_weed_exp(r);
                break;
            case EXP_OR:
                f_weed_exp(l);
                f_weed_exp(r);
                break;
        }
    }
}

void f_weed_switch(switch_stm *s_stm)
{
    if (s_stm == NULL)
    {
        return;
    }
    f_weed_switch_case(s_stm->switch_case);
    f_weed_switch(s_stm->switch_list);
}

void f_weed_switch_case(switch_case *s_case)
{
    f_weed_exp(s_case->exp);
    f_weed_statement(s_case->stm);
}

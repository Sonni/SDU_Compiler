#include "inner_class_rename.h"
#include <stdio.h>
#include <string.h>

void inner_class_rename(body *b, char *old_name, char *new_name)
{
    inner_class_rename_body(b, old_name, new_name);
}

void inner_class_rename_body(body *b, char *old_name, char *new_name)
{
    inner_class_rename_decl_list(b->dl, old_name, new_name);
    inner_class_rename_statement_list(b->sl, old_name, new_name);
}

void inner_class_rename_function(function *f, char *old_name, char *new_name)
{
    inner_class_rename_head(f->head, old_name, new_name);
    inner_class_rename_body(f->body, old_name, new_name);
}

void inner_class_rename_head(head *h, char *old_name, char *new_name)
{
    inner_class_rename_par_decl_list(h->pdl, old_name, new_name);
    inner_class_rename_type(h->type, old_name, new_name);
}


void inner_class_rename_type(type *t, char *old_name, char *new_name)
{
    switch (t->kind)
    {
        case TYPE_ID:
            if (strcmp(t->id, old_name) == 0)
            {
                t->id = new_name;
            }
            break;
        case TYPE_INT:
            break;
        case TYPE_FLOAT:
            break;
        case TYPE_BOOL:
            break;
        case TYPE_STRING:
            break;
        case TYPE_ARRAY:
            inner_class_rename_type(t->val.type, old_name, new_name);
            break;
        case TYPE_RECORD:
            inner_class_rename_var_decl_list(t->val.vdl, old_name, new_name);
            break;
        case TYPE_CLASS:
            inner_class_rename_decl_list(t->val._class.dl, old_name, new_name);
            break;
    }
}

void inner_class_rename_par_decl_list(par_decl_list *pdl, char *old_name, char *new_name)
{
    switch (pdl->kind)
    {
        case PDL_VDL:
            inner_class_rename_var_decl_list(pdl->vdl, old_name, new_name);
            break;
        case PDL_EMPTY:
            break;
    }
}

void inner_class_rename_var_decl_list(var_decl_list *vdl, char *old_name, char *new_name)
{
    switch (vdl->kind)
    {
        case VDL_VT_VDL:
            inner_class_rename_var_type(vdl->vt, old_name, new_name);
            inner_class_rename_var_decl_list(vdl->vdl, old_name, new_name);
            break;
        case VDL_VT:
            inner_class_rename_var_type(vdl->vt, old_name, new_name);
            break;
    }
}

void inner_class_rename_var_type(var_type *vt, char *old_name, char *new_name)
{
    inner_class_rename_type(vt->type, old_name, new_name);
}

void inner_class_rename_statement_list(statement_list *sl, char *old_name, char *new_name)
{
    if (sl == NULL)
        return;

    inner_class_rename_statement_list(sl->sl, old_name, new_name);
    inner_class_rename_statement(sl->stm, old_name, new_name);
}

void inner_class_rename_statement(statement *stm, char *old_name, char *new_name)
{
    switch (stm->kind)
    {
        case STM_RETURN:
            inner_class_rename_exp(stm->val.exp, old_name, new_name);
            break;
        case STM_WRITE:
            inner_class_rename_exp(stm->val.exp, old_name, new_name);
            break;
        case STM_ALLOCATE:
            inner_class_rename_variable(stm->val.var, old_name, new_name);
            break;
        case STM_ALLOCATE_OF_LENGTH:
            inner_class_rename_variable(stm->val.assignment.var, old_name, new_name);
            inner_class_rename_exp(stm->val.assignment.exp, old_name, new_name);
            break;
        case STM_ASSIGNMENT:
            inner_class_rename_variable(stm->val.assignment.var, old_name, new_name);
            inner_class_rename_exp(stm->val.assignment.exp, old_name, new_name);
            break;
        case STM_ASSIGN_LIST:
            inner_class_rename_variable(stm->val.assign_list.var, old_name, new_name);
            inner_class_rename_exp_list(stm->val.assign_list.el, old_name, new_name);
            break;
        case STM_IF:
            inner_class_rename_exp(stm->val.if_then.exp, old_name, new_name);
            inner_class_rename_statement(stm->val.if_then.stm, old_name, new_name);
            break;
        case STM_ELSE:
            inner_class_rename_exp(stm->val.if_then.exp, old_name, new_name);
            inner_class_rename_statement(stm->val.if_else.stm1, old_name, new_name);
            inner_class_rename_statement(stm->val.if_else.stm2, old_name, new_name);
            break;
        case STM_WHILE:
            inner_class_rename_exp(stm->val.while_do.exp, old_name, new_name);
            inner_class_rename_statement(stm->val.while_do.stm, old_name, new_name);
            break;
        case STM_LIST:
            inner_class_rename_statement_list(stm->val.sl, old_name, new_name);
            break;
        case STM_BREAK:
            break;
        case STM_CONTINUE:
            break;
        case STM_IN:
            inner_class_rename_variable(stm->val.var, old_name, new_name);
            break;
        case STM_FREE:
            inner_class_rename_variable(stm->val.var, old_name, new_name);
            break;
        case STM_ASSIGN_VAL:
            inner_class_rename_variable(stm->val.assignment.var, old_name, new_name);
            inner_class_rename_exp(stm->val.assignment.exp, old_name, new_name);
            break;
        case STM_SWITCH:
            inner_class_rename_exp(stm->val.switch_stm.exp, old_name, new_name);
            inner_class_rename_switch(stm->val.switch_stm.s_stm, old_name, new_name);
            break;
    }
}

void inner_class_rename_decl_list(decl_list *dl, char *old_name, char *new_name)
{
    switch (dl->kind)
    {
        case DL_DECL_DL:
            inner_class_rename_decl_list(dl->dl, old_name, new_name);
            inner_class_rename_declaration(dl->decl, old_name, new_name);
            break;
        case DL_EMPTY:
            break;
    }
}

void inner_class_rename_declaration(declaration *d, char *old_name, char *new_name)
{
    switch (d->kind)
    {
        case DECL_TYPE:
            inner_class_rename_type(d->val.type.type, old_name, new_name);
            break;
        case DECL_FUNC:
            inner_class_rename_function(d->val.func, old_name, new_name);
            break;
        case DECL_VDL:
            inner_class_rename_var_decl_list(d->val.vdl, old_name, new_name);
            break;
        case DECL_CONST:
            inner_class_rename_type(d->val.con.type, old_name, new_name);
            inner_class_rename_exp(d->val.con.exp, old_name, new_name);
            break;
    }
}

void inner_class_rename_variable(variable *var, char *old_name, char *new_name)
{
    switch (var->kind)
    {
        case VAR_ID:
            break;
        case VAR_EXP:
            inner_class_rename_variable(var->val.var_exp.var, old_name, new_name);
            inner_class_rename_exp_list(var->val.var_exp.exp, old_name, new_name);
            break;
        case VAR_RECORD:
            inner_class_rename_variable(var->val.var_exp.var, old_name, new_name);
            break;
        case VAR_CLASS:
            inner_class_rename_variable(var->val.var_exp.var, old_name, new_name);
            break;
        case VAR_FUNC:
            inner_class_rename_variable(var->val.var_exp.var, old_name, new_name);
            inner_class_rename_act_list(var->val.var_exp.al, old_name, new_name);
            break;
    }
}

void inner_class_rename_term(term *t, char *old_name, char *new_name)
{
    switch (t->kind)
    {
        case TERM_VAR:
            inner_class_rename_variable(t->val.var, old_name, new_name);
            break;
        case TERM_AL:
            inner_class_rename_act_list(t->val.term_act_list.al, old_name, new_name);
            break;
        case TERM_PARENTHESES:
            inner_class_rename_exp(t->val.exp, old_name, new_name);
            break;
        case TERM_NOT:
            inner_class_rename_term(t->val.not_term, old_name, new_name);
            break;
        case TERM_ABS:
            inner_class_rename_exp(t->val.exp, old_name, new_name);
            break;
        case TERM_INT:
            break;
        case TERM_FLOAT:
            break;
        case TERM_TRUE:
            break;
        case TERM_FALSE:
            break;
        case TERM_NULL:
            break;
        case TERM_STRING:
            break;
    }
}

void inner_class_rename_act_list(act_list *al, char *old_name, char *new_name)
{
    switch (al->kind)
    {
        case AL_EXP_LIST:
            inner_class_rename_exp_list(al->exp_list, old_name, new_name);
            break;
        case AL_EMPTY:
            break;
    }
}

void inner_class_rename_exp_list(expression_list *exp_list, char *old_name, char *new_name)
{
    if (exp_list == NULL)
    {
        return;
    }
    inner_class_rename_exp(exp_list->exp, old_name, new_name);

    if (exp_list->exp_list != NULL)
    {
        inner_class_rename_exp_list(exp_list->exp_list, old_name, new_name);
    }
}

void inner_class_rename_exp(expression *exp, char *old_name, char *new_name)
{
    switch (exp->kind)
    {
        case EXP_TIMES:
            inner_class_rename_exp(exp->val.op.left, old_name, new_name);
            inner_class_rename_exp(exp->val.op.right, old_name, new_name);
            break;
        case EXP_DIV:
            inner_class_rename_exp(exp->val.op.left, old_name, new_name);
            inner_class_rename_exp(exp->val.op.right, old_name, new_name);
            break;
        case EXP_PLUS:
            inner_class_rename_exp(exp->val.op.left, old_name, new_name);
            inner_class_rename_exp(exp->val.op.right, old_name, new_name);
            break;
        case EXP_MINUS:
            inner_class_rename_exp(exp->val.op.left, old_name, new_name);
            inner_class_rename_exp(exp->val.op.right, old_name, new_name);
            break;
        case EXP_TERM:
            inner_class_rename_term(exp->val.term, old_name, new_name);
            break;
        case EXP_EQ:
            inner_class_rename_exp(exp->val.op.left, old_name, new_name);
            inner_class_rename_exp(exp->val.op.right, old_name, new_name);
            break;
        case EXP_NEQ:
            inner_class_rename_exp(exp->val.op.left, old_name, new_name);
            inner_class_rename_exp(exp->val.op.right, old_name, new_name);
            break;
        case EXP_GREATER:
            inner_class_rename_exp(exp->val.op.left, old_name, new_name);
            inner_class_rename_exp(exp->val.op.right, old_name, new_name);
            break;
        case EXP_LESS:
            inner_class_rename_exp(exp->val.op.left, old_name, new_name);
            inner_class_rename_exp(exp->val.op.right, old_name, new_name);
            break;
        case EXP_GEQ:
            inner_class_rename_exp(exp->val.op.left, old_name, new_name);
            inner_class_rename_exp(exp->val.op.right, old_name, new_name);
            break;
        case EXP_LEQ:
            inner_class_rename_exp(exp->val.op.left, old_name, new_name);
            inner_class_rename_exp(exp->val.op.right, old_name, new_name);
            break;
        case EXP_AND:
            inner_class_rename_exp(exp->val.op.left, old_name, new_name);
            inner_class_rename_exp(exp->val.op.right, old_name, new_name);
            break;
        case EXP_OR:
            inner_class_rename_exp(exp->val.op.left, old_name, new_name);
            inner_class_rename_exp(exp->val.op.right, old_name, new_name);
            break;
    }
}

void inner_class_rename_switch(switch_stm *s_stm, char *old_name, char *new_name)
{
    if (s_stm == NULL)
    {
        return;
    }
    inner_class_rename_switch_case(s_stm->switch_case, old_name, new_name);
    inner_class_rename_switch(s_stm->switch_list, old_name, new_name);

}

void inner_class_rename_switch_case(switch_case *s_case, char *old_name, char *new_name)
{
    inner_class_rename_exp(s_case->exp, old_name, new_name);
    inner_class_rename_statement(s_case->stm, old_name, new_name);
}

#include "ast_reset.h"

void reset(body *b)
{
    reset_body(b);
}

void reset_function(function *f)
{
    f->st = NULL;
    f->sym_type = NULL;
    reset_head(f->head);
    reset_body(f->body);
}

void reset_body(body *b)
{
    b->sym_type = NULL;
    b->st = NULL;
    reset_decl_list(b->dl);
    reset_statement_list(b->sl);
}


void reset_head(head *h)
{
    h->id = h->original_name;
    h->new_name = h->original_name;
    h->sym_type = NULL;
    h->st = NULL;
    reset_par_decl_list(h->pdl);
    reset_type(h->type);
}


void reset_type(type *t)
{
    t->sym_type = NULL;
    t->st = NULL;
    switch (t->kind)
    {
        case TYPE_ID:
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
            reset_type(t->val.type);
            break;
        case TYPE_RECORD:
            reset_var_decl_list(t->val.vdl);
            break;
        case TYPE_CLASS:
            reset_decl_list(t->val._class.dl);
            break;
    }
}

void reset_par_decl_list(par_decl_list *pdl)
{
    pdl->sym_type = NULL;
    pdl->st = NULL;
    switch (pdl->kind)
    {
        case PDL_VDL:
            reset_var_decl_list(pdl->vdl);
            break;
        case PDL_EMPTY:
            break;
    }
}

void reset_var_decl_list(var_decl_list *vdl)
{
    vdl->sym_type = NULL;
    vdl->st = NULL;
    switch (vdl->kind)
    {
        case VDL_VT_VDL:
            reset_var_type(vdl->vt);
            reset_var_decl_list(vdl->vdl);
            break;
        case VDL_VT:
            reset_var_type(vdl->vt);
            break;
    }
}

void reset_var_type(var_type *vt)
{
    vt->sym_type = NULL;
    vt->st = NULL;
    reset_type(vt->type);
}

void reset_statement_list(statement_list *sl)
{
    sl->st = NULL;
    sl->sym_type = NULL;
    switch (sl->kind)
    {
        case SL_STM:
            reset_statement(sl->stm);
            break;
        case SL_STM_SL:
            reset_statement(sl->stm);
            reset_statement_list(sl->sl);
            break;
    }
}

void reset_statement(statement *stm)
{
    stm->sym_type = NULL;
    stm->st = NULL;
    switch (stm->kind)
    {
        case STM_RETURN:
            reset_exp(stm->val.exp);
            break;
        case STM_WRITE:
            reset_exp(stm->val.exp);
            break;
        case STM_ALLOCATE:
            reset_variable(stm->val.var);
            break;
        case STM_ALLOCATE_OF_LENGTH:
            reset_variable(stm->val.assignment.var);
            reset_exp(stm->val.assignment.exp);
            break;
        case STM_ASSIGNMENT:
            reset_variable(stm->val.assignment.var);
            reset_exp(stm->val.assignment.exp);
            break;
        case STM_ASSIGN_LIST:
            reset_variable(stm->val.assign_list.var);
            reset_exp_list(stm->val.assign_list.el);
            break;
        case STM_IF:
            reset_exp(stm->val.if_then.exp);
            reset_statement(stm->val.if_then.stm);
            break;
        case STM_ELSE:
            reset_exp(stm->val.if_then.exp);
            reset_statement(stm->val.if_else.stm1);
            reset_statement(stm->val.if_else.stm2);
            break;
        case STM_WHILE:
            reset_exp(stm->val.while_do.exp);
            reset_statement(stm->val.while_do.stm);
            break;
        case STM_LIST:
            reset_statement_list(stm->val.sl);
            break;
        case STM_BREAK:
        case STM_CONTINUE:
            break;
        case STM_IN:
            reset_variable(stm->val.var);
            break;
        case STM_FREE:
            reset_variable(stm->val.var);
            break;
        case STM_ASSIGN_VAL:
            reset_variable(stm->val.assignment.var);
            reset_exp(stm->val.assignment.exp);
            break;
        case STM_SWITCH:
            reset_exp(stm->val.switch_stm.exp);
            reset_switch(stm->val.switch_stm.s_stm);
            break;
    }
}

void reset_decl_list(decl_list *dl)
{
    dl->sym_type = NULL;
    dl->st = NULL;
    switch (dl->kind)
    {
        case DL_DECL_DL:
            reset_decl_list(dl->dl);
            reset_declaration(dl->decl);
            break;
        case DL_EMPTY:
            break;
    }
}

void reset_declaration(declaration *d)
{
    d->sym_type = NULL;
    d->st = NULL;

    switch (d->kind)
    {
        case DECL_TYPE:
            reset_type(d->val.type.type);
            break;
        case DECL_FUNC:
            reset_function(d->val.func);
            break;
        case DECL_VDL:
            reset_var_decl_list(d->val.vdl);
            break;
        case DECL_CONST:
            reset_type(d->val.con.type);
            reset_exp(d->val.con.exp);
            break;
    }
}

void reset_variable(variable *var)
{
    var->sym_type = NULL;
    var->st = NULL;
    switch (var->kind)
    {
        case VAR_ID:
            break;
        case VAR_EXP:
            reset_variable(var->val.var_exp.var);
            reset_exp_list(var->val.var_exp.exp);
            break;
        case VAR_RECORD:
            reset_variable(var->val.var_exp.var);
            break;
        case VAR_CLASS:
            reset_variable(var->val.var_exp.var);
            break;
        case VAR_FUNC:
            var->val.var_exp.id = var->val.var_exp.original_name;
            reset_variable(var->val.var_exp.var);
            reset_act_list(var->val.var_exp.al);
            break;
    }
}

void reset_term(term *t)
{
    t->sym_type = NULL;
    t->st = NULL;
    switch (t->kind)
    {
        case TERM_VAR:
            reset_variable(t->val.var);
            break;
        case TERM_AL:
            t->val.term_act_list.id = t->val.term_act_list.original_name;
            reset_act_list(t->val.term_act_list.al);
            break;
        case TERM_PARENTHESES:
            reset_exp(t->val.exp);
            break;
        case TERM_NOT:
            reset_term(t->val.not_term);
            break;
        case TERM_ABS:
            reset_exp(t->val.exp);
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

void reset_act_list(act_list *al)
{
    al->sym_type = NULL;
    al->st = NULL;

    switch (al->kind)
    {
        case AL_EXP_LIST:
            reset_exp_list(al->exp_list);
            break;
        case AL_EMPTY:
            break;
    }

}

void reset_exp_list(expression_list *exp_list)
{
    exp_list->sym_type = NULL;
    exp_list->st = NULL;

    if (exp_list == NULL)
    {
        return;
    }
    reset_exp(exp_list->exp);

    if (exp_list->exp_list != NULL)
    {
        reset_exp_list(exp_list->exp_list);
    }

}

void reset_exp(expression *exp)
{
    exp->sym_type = NULL;
    exp->st = NULL;
    switch (exp->kind)
    {
        case EXP_TERM:
            reset_term(exp->val.term);
            break;
        case EXP_TIMES:
        case EXP_DIV:
        case EXP_PLUS:
        case EXP_MINUS:
        case EXP_EQ:
        case EXP_NEQ:
        case EXP_GREATER:
        case EXP_LESS:
        case EXP_GEQ:
        case EXP_LEQ:
        case EXP_AND:
        case EXP_OR:
            reset_exp(exp->val.op.left);
            reset_exp(exp->val.op.right);
            break;
    }
}

void reset_switch(switch_stm *s_stm)
{
    if (s_stm == NULL)
    {
        return;
    }
    s_stm->sym_type = NULL;
    s_stm->st = NULL;
    reset_switch_case(s_stm->switch_case);
    reset_switch(s_stm->switch_list);
}

void reset_switch_case(switch_case *s_case)
{
    s_case->sym_type = NULL;
    s_case->st = NULL;
    reset_exp(s_case->exp);
    reset_statement(s_case->stm);
}

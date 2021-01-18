#include <stdio.h>
#include <stdlib.h>
#include "new_names.h"
#include "../../../utils/memory/memory.h"
#include "../../../utils/string_util.h"

int name_id;
bool create_new_names;

symbol *clone(symbol *s, char *name)
{
    char id[500];
    sprintf(id, "n_%i_%s", name_id, name);

    symbol *res = NEW(symbol);
    res->st = s->st;
    res->sym_type = s->sym_type;
    res->value = s->value;
    res->offset = s->offset;
    res->string = s->string;
    res->access = s->access;
    res->know_value = s->know_value;
    res->new_name = cpy_str(id);
    res->name = res->new_name;
    res->is_const = s->is_const;
    s->new_name = res->new_name;

    name_id++;

    return res;
}

void new_name(body *b)
{
    create_new_names = false;
    new_name_body(b);

    create_new_names = true;
    new_name_body(b);
}

void new_name_body(body *b)
{
    new_name_decl_list(b->dl);
    if (create_new_names)
    new_name_statement_list(b->sl);
}

void new_name_function(function *f)
{
    new_name_head(f->head);
    new_name_body(f->body);
}

void new_name_head(head *h)
{
    new_name_par_decl_list(h->pdl);
    new_name_type(h->type);
}

void new_name_type(type *t)
{
    symbol *s = NULL;
    switch (t->kind)
    {
        case TYPE_ID:
            if (create_new_names)
            {
                s = get_symbol(t->st, t->id);
                t->id = s->new_name;
            }
            break;
        case TYPE_INT:
        case TYPE_FLOAT:
        case TYPE_STRING:
            break;
        case TYPE_BOOL:
            break;
        case TYPE_ARRAY:
            new_name_type(t->val.type);
            break;
        case TYPE_RECORD:
            new_name_var_decl_list(t->val.vdl);
            break;
        case TYPE_CLASS:
            break;
    }
}

void new_name_par_decl_list(par_decl_list *pdl)
{
    switch (pdl->kind)
    {
        case PDL_VDL:
            new_name_var_decl_list(pdl->vdl);
            break;
        case PDL_EMPTY:
            break;
    }
}

void new_name_var_decl_list(var_decl_list *vdl)
{
    switch (vdl->kind)
    {
        case VDL_VT_VDL:
            new_name_var_type(vdl->vt);
            new_name_var_decl_list(vdl->vdl);
            break;
        case VDL_VT:
            new_name_var_type(vdl->vt);
            break;
    }
}

void new_name_var_type(var_type *vt)
{
    if (!create_new_names)
    {
        symbol *s = get_symbol(vt->st, vt->id);

        s = clone(s, vt->id);

        symbol *s2 = put_symbol(vt->st, s->name, s->sym_type,
                   s->value);
        s2->offset = s->offset;
        s2->new_name = s->name;

        vt->id = s->new_name;
    }
    new_name_type(vt->type);
}

void new_name_statement_list(statement_list *sl)
{
    if (sl == NULL)
        return;

    new_name_statement_list(sl->sl);
    new_name_statement(sl->stm);
}

void new_name_statement(statement *stm)
{
    switch (stm->kind)
    {
        case STM_RETURN:
            new_name_exp(stm->val.exp);
            break;
        case STM_WRITE:
            new_name_exp(stm->val.exp);
            break;
        case STM_ALLOCATE:
            new_name_variable(stm->val.var);
            break;
        case STM_ALLOCATE_OF_LENGTH:
            new_name_variable(stm->val.assignment.var);
            new_name_exp(stm->val.assignment.exp);
            break;
        case STM_ASSIGNMENT:
            new_name_variable(stm->val.assignment.var);
            new_name_exp(stm->val.assignment.exp);
            break;
        case STM_ASSIGN_LIST:
            break;
        case STM_IF:
            new_name_exp(stm->val.if_then.exp);
            new_name_statement(stm->val.if_then.stm);
            break;
        case STM_ELSE:
            new_name_exp(stm->val.if_else.exp);
            new_name_statement(stm->val.if_else.stm1);
            new_name_statement(stm->val.if_else.stm2);
            break;
        case STM_WHILE:
            new_name_exp(stm->val.while_do.exp);
            new_name_statement(stm->val.while_do.stm);
            break;
        case STM_LIST:
            new_name_statement_list(stm->val.sl);
            break;
        case STM_BREAK:
            break;
        case STM_CONTINUE:
            break;
        case STM_IN:
            new_name_variable(stm->val.var);
            break;
        case STM_FREE:
            new_name_variable(stm->val.var);
            break;
        case STM_ASSIGN_VAL:
            new_name_variable(stm->val.assignment.var);
            new_name_exp(stm->val.assignment.exp);
            break;
        case STM_SWITCH:
            new_name_exp(stm->val.switch_stm.exp);
            new_name_switch(stm->val.switch_stm.s_stm);
            break;
    }
}

void new_name_decl_list(decl_list *dl)
{
    switch (dl->kind)
    {
        case DL_DECL_DL:
            new_name_decl_list(dl->dl);
            new_name_declaration(dl->decl);
            break;
        case DL_EMPTY:
            break;
    }
}

void new_name_declaration(declaration *d)
{
    symbol *s;

    switch (d->kind)
    {
        case DECL_TYPE:
            if (!create_new_names)
            {
                s = get_symbol(d->st, d->val.type.id);

                s = clone(s, d->val.type.id);
                symbol *s2 = put_symbol(d->st, s->name, s->sym_type,
                           s->value);
                s2->offset = s->offset;
                s2->new_name = s->name;
                d->val.type.id = s->name;
            }
            new_name_type(d->val.type.type);
            break;
        case DECL_FUNC:
            new_name_function(d->val.func);
            break;
        case DECL_VDL:
            new_name_var_decl_list(d->val.vdl);
            break;
        case DECL_CONST:
            new_name_type(d->val.con.type);
            new_name_exp(d->val.con.exp);
            if (!create_new_names)
            {
                s = get_symbol(d->st, d->val.con.id);
                s = clone(s, d->val.con.id);
                symbol *s2 = put_symbol(d->st, s->name, s->sym_type,
                                        s->value);
                s2->offset = s->offset;
                s2->new_name = s->name;
                d->val.con.id = s->name;
                s2->string = s->name;

            }
            break;
    }
}

void new_name_variable(variable *var)
{
    symbol *s;
    switch (var->kind)
    {
        case VAR_ID:
            if (create_new_names)
            {
                s = get_symbol(var->st, var->val.var_exp.id);
                var->val.var_exp.id = s->new_name;
            }
            break;
        case VAR_EXP:
            new_name_variable(var->val.var_exp.var);
            new_name_exp_list(var->val.var_exp.exp);
            break;
        case VAR_RECORD:
            new_name_variable(var->val.var_exp.var);
            break;
        case VAR_CLASS:
            new_name_variable(var->val.var_exp.var);
            break;
        case VAR_FUNC:
            new_name_variable(var->val.var_exp.var);
            new_name_act_list(var->val.var_exp.al);
            break;
    }
}

void new_name_term(term *t)
{
    switch (t->kind)
    {
        case TERM_VAR:
            new_name_variable(t->val.var);
            break;
        case TERM_AL:
            new_name_act_list(t->val.term_act_list.al);
            break;
        case TERM_PARENTHESES:
            new_name_exp(t->val.exp);
            break;
        case TERM_NOT:
            new_name_term(t->val.not_term);
            break;
        case TERM_ABS:
            new_name_exp(t->val.exp);
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

void new_name_act_list(act_list *al)
{
    switch (al->kind)
    {
        case AL_EXP_LIST:
            new_name_exp_list(al->exp_list);
            break;
        case AL_EMPTY:
            break;
    }
}

void new_name_exp_list(expression_list *exp_list)
{
    switch (exp_list->kind)
    {
        case EXP_LIST_EXP:
            new_name_exp(exp_list->exp);
            break;
        case EXP_LIST_EXP_LIST:
            new_name_exp(exp_list->exp);
            new_name_exp_list(exp_list->exp_list);
            break;
    }
}


void new_name_exp(expression *exp)
{
    switch (exp->kind)
    {
        case EXP_TIMES:
            new_name_exp(exp->val.op.left);
            new_name_exp(exp->val.op.right);
            break;
        case EXP_DIV:
            new_name_exp(exp->val.op.left);
            new_name_exp(exp->val.op.right);
            break;
        case EXP_PLUS:
            new_name_exp(exp->val.op.left);
            new_name_exp(exp->val.op.right);
            break;
        case EXP_MINUS:
            new_name_exp(exp->val.op.left);
            new_name_exp(exp->val.op.right);
            break;
        case EXP_TERM:
            new_name_term(exp->val.term);
            break;
        case EXP_EQ:
            new_name_exp(exp->val.op.left);
            new_name_exp(exp->val.op.right);
            break;
        case EXP_NEQ:
            new_name_exp(exp->val.op.left);
            new_name_exp(exp->val.op.right);
            break;
        case EXP_GREATER:
            new_name_exp(exp->val.op.left);
            new_name_exp(exp->val.op.right);
            break;
        case EXP_LESS:
            new_name_exp(exp->val.op.left);
            new_name_exp(exp->val.op.right);
            break;
        case EXP_GEQ:
            new_name_exp(exp->val.op.left);
            new_name_exp(exp->val.op.right);
            break;
        case EXP_LEQ:
            new_name_exp(exp->val.op.left);
            new_name_exp(exp->val.op.right);
            break;
        case EXP_AND:
            new_name_exp(exp->val.op.left);
            new_name_exp(exp->val.op.right);
            break;
        case EXP_OR:
            new_name_exp(exp->val.op.left);
            new_name_exp(exp->val.op.right);
            break;
    }
}

void new_name_switch(switch_stm *s_stm)
{
    if (s_stm == NULL)
    {
        return;
    }
    new_name_switch_case(s_stm->switch_case);
    new_name_switch(s_stm->switch_list);
}

void new_name_switch_case(switch_case *s_case)
{
    new_name_exp(s_case->exp);
    new_name_statement(s_case->stm);
}

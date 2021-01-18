#include "free_ast.h"
#include <stdlib.h>


void free_ast(body *b)
{
    free_body(b);
    free(b);
}

void free_body(body *b)
{
    free_decl_list(b->dl);
    free(b->dl);
    free_statement_list(b->sl);
    free(b->sl);
    free_class_decl_list(b->cdl);
    free(b->cdl);
}

void free_function(function *f)
{
    free_head(f->head);
    free(f->head);
    free_body(f->body);
    free(f->body);
    free_tail(f->tail);
    free(f->tail);
}

void free_head(head *h)
{
    free_par_decl_list(h->pdl);
    free(h->pdl);
    free_type(h->type);
    free(h->type);
    free(h->id);
}

void free_tail(tail *t)
{
   free(t->id);
}

void free_type(type *t)
{
    switch (t->kind)
    {
        case TYPE_ID:
            free(t->id);
            break;
        case TYPE_INT:
        case TYPE_FLOAT:
        case TYPE_STRING:
            break;
        case TYPE_BOOL:
            break;
        case TYPE_ARRAY:
            free(t->id);
            break;
        case TYPE_RECORD:
            free_var_decl_list(t->val.vdl);
            free(t->val.vdl);
            free(t->id);
            break;
        case TYPE_CLASS:
            break;
    }
}

void free_par_decl_list(par_decl_list *pdl)
{
    switch (pdl->kind)
    {
        case PDL_VDL:
            free_var_decl_list(pdl->vdl);
            free(pdl->vdl);
            break;
        case PDL_EMPTY:
            break;
    }
}

void free_var_decl_list(var_decl_list *vdl)
{
    switch (vdl->kind)
    {
        case VDL_VT_VDL:
            free_var_type(vdl->vt);
            free(vdl->vt);
            free_var_decl_list(vdl->vdl);
            free(vdl->vdl);
            break;
        case VDL_VT:
            free_var_type(vdl->vt);
            free(vdl->vt);
            break;
    }
}

void free_var_type(var_type *vt)
{
    free_type(vt->type);
    free(vt->type);
    free(vt->id);
}

void free_statement_list(statement_list *sl)
{
    if (sl == NULL)
    {
        return;
    }
    free_statement_list(sl->sl);
    free_statement(sl->stm);

    free(sl->sl);
    free(sl->stm);
}

void free_statement(statement *stm)
{
    switch (stm->kind)
    {
        case STM_RETURN:
            free_exp(stm->val.exp);
            free(stm->val.exp);
            break;
        case STM_WRITE:
            free_exp(stm->val.exp);
            free(stm->val.exp);
            break;
        case STM_ALLOCATE:
            free_variable(stm->val.var);
            free(stm->val.var);
            break;
        case STM_ALLOCATE_OF_LENGTH:
            free_variable(stm->val.assignment.var);
            free_exp(stm->val.assignment.exp);

            free(stm->val.assignment.var);
            free(stm->val.assignment.exp);
            break;
        case STM_ASSIGNMENT:
            free_variable(stm->val.assignment.var);
            free_exp(stm->val.assignment.exp);

            free(stm->val.assignment.var);
            free(stm->val.assignment.exp);
            break;
        case STM_ASSIGN_LIST:
            break;
        case STM_IF:
            free_exp(stm->val.if_then.exp);
            free_statement(stm->val.if_then.stm);

            free(stm->val.if_then.exp);
            free(stm->val.if_then.stm);
            break;
        case STM_ELSE:
            free_exp(stm->val.if_else.exp);
            free_statement(stm->val.if_else.stm1);
            free_statement(stm->val.if_else.stm2);

            free(stm->val.if_else.exp);
            free(stm->val.if_else.stm1);
            free(stm->val.if_else.stm2);
            break;
        case STM_WHILE:
            free_exp(stm->val.while_do.exp);
            free_statement(stm->val.while_do.stm);

            free(stm->val.while_do.exp);
            free(stm->val.while_do.stm);
            break;
        case STM_LIST:
            free_statement_list(stm->val.sl);
            free(stm->val.sl);
            break;
        case STM_BREAK:
        case STM_IN:
        case STM_CONTINUE:
        case STM_FREE:
            break;
        case STM_ASSIGN_VAL:
            free_variable(stm->val.assignment.var);
            free_exp(stm->val.assignment.exp);
            break;
        case STM_SWITCH:
            break;
    }
}

void free_decl_list(decl_list *dl)
{
    switch (dl->kind)
    {
        case DL_DECL_DL:
            free_decl_list(dl->dl);
            free_declaration(dl->decl);

            free(dl->dl);
            free(dl->decl);
            break;
        case DL_EMPTY:
            break;
    }
}

void free_declaration(declaration *d)
{
    switch (d->kind)
    {
        case DECL_TYPE:
            free_type(d->val.type.type);
            free(d->val.type.type);
            break;
        case DECL_FUNC:
            free_function(d->val.func);
            free(d->val.func);
            break;
        case DECL_VDL:
            free_var_decl_list(d->val.vdl);
            free(d->val.vdl);
            break;
        case DECL_CONST:
            break;
    }
}

void free_variable(variable *var)
{
    switch (var->kind)
    {
        case VAR_ID:
            break;
        case VAR_EXP:
            free_variable(var->val.var_exp.var);
            free_exp_list(var->val.var_exp.exp);

            free(var->val.var_exp.var);
            free(var->val.var_exp.exp);
            break;
        case VAR_RECORD:
            free_variable(var->val.var_exp.var);
            free(var->val.var_exp.var);
            break;
        case VAR_CLASS:
            free_variable(var->val.var_exp.var);
            free(var->val.var_exp.var);
            break;
        case VAR_FUNC:
            break;
    }
}

void free_term(term *t)
{
    switch (t->kind)
    {
        case TERM_VAR:
            free_variable(t->val.var);

            break;
        case TERM_AL:
            free_act_list(t->val.term_act_list.al);
            free(t->val.term_act_list.al);
            break;
        case TERM_PARENTHESES:
            free_exp(t->val.exp);
            free(t->val.exp);
            break;
        case TERM_NOT:
            free_term(t->val.not_term);
            free(t->val.not_term);
            break;
        case TERM_ABS:
            free_exp(t->val.exp);
            free(t->val.exp);
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

void free_act_list(act_list *al)
{
    switch (al->kind)
    {
        case AL_EXP_LIST:
            free_exp_list(al->exp_list);
            free(al->exp_list);
            break;
        case AL_EMPTY:
            break;
    }
}

void free_exp_list(expression_list *exp_list)
{
    switch (exp_list->kind)
    {
        case EXP_LIST_EXP:
            free_exp(exp_list->exp);
            free(exp_list->exp);
            break;
        case EXP_LIST_EXP_LIST:
            free_exp_list(exp_list->exp_list);
            free_exp(exp_list->exp);

            free(exp_list->exp_list);
            free(exp_list->exp);
            break;
    }
}


void free_exp(expression *exp)
{
    switch (exp->kind)
    {
        case EXP_TERM:
            free_term(exp->val.term);
            free(exp->val.term);
            break;
        case EXP_EQ:
        case EXP_NEQ:
        case EXP_GREATER:
        case EXP_LESS:
        case EXP_GEQ:
        case EXP_LEQ:
        case EXP_AND:
        case EXP_OR:
        case EXP_TIMES:
        case EXP_DIV:
        case EXP_PLUS:
        case EXP_MINUS:
            free_exp(exp->val.op.left);
            free_exp(exp->val.op.right);
            free(exp->val.op.left);
            free(exp->val.op.right);
            break;
    }

}

void free_class_decl_list(class_decl_list *cdl)
{
    switch (cdl->kind)
    {
        case CDL_DECL:
            free_class_decl(cdl->class_decl);
            free(cdl->class_decl);
            break;
        case CDL_LIST:
            free_class_decl(cdl->class_decl);
            free_class_decl_list(cdl->cdl);
            free(cdl->class_decl);
            free(cdl->cdl);
            break;
    }
}

void free_class_decl(class_decl *cdl)
{
    switch (cdl->kind)
    {
        case CLASS:
            free_decl_list(cdl->dl);
            free(cdl->dl);
            break;
        case CLASS_EMPTY:
            break;
    }
}


#include "obj_op_replace.h"
#include <stdio.h>

void obj_op_replace(body *b)
{
    obj_op_replace_body(b);
}

void obj_op_replace_body(body *b)
{
    obj_op_replace_decl_list(b->dl);
    obj_op_replace_statement_list(b->sl);
}

void obj_op_replace_function(function *f)
{
    obj_op_replace_head(f->head);
    obj_op_replace_body(f->body);
}

void obj_op_replace_head(head *h)
{
    obj_op_replace_par_decl_list(h->pdl);
    obj_op_replace_type(h->type);
}


void obj_op_replace_type(type *t)
{
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
            obj_op_replace_type(t->val.type);
            break;
        case TYPE_RECORD:
            obj_op_replace_var_decl_list(t->val.vdl);
            break;
        case TYPE_CLASS:
            obj_op_replace_decl_list(t->val._class.dl);
            break;
    }
}

void obj_op_replace_par_decl_list(par_decl_list *pdl)
{
    switch (pdl->kind)
    {
        case PDL_VDL:
            obj_op_replace_var_decl_list(pdl->vdl);
            break;
        case PDL_EMPTY:
            break;
    }
}

void obj_op_replace_var_decl_list(var_decl_list *vdl)
{
    switch (vdl->kind)
    {
        case VDL_VT_VDL:
            obj_op_replace_var_type(vdl->vt);
            obj_op_replace_var_decl_list(vdl->vdl);
            break;
        case VDL_VT:
            obj_op_replace_var_type(vdl->vt);
            break;
    }
}

void obj_op_replace_var_type(var_type *vt)
{
    obj_op_replace_type(vt->type);
}

void obj_op_replace_statement_list(statement_list *sl)
{
    if (sl == NULL)
        return;

    obj_op_replace_statement_list(sl->sl);
    obj_op_replace_statement(sl->stm);
}

void obj_op_replace_statement(statement *stm)
{
    switch (stm->kind)
    {
        case STM_RETURN:
            obj_op_replace_exp(stm->val.exp);
            break;
        case STM_WRITE:
            obj_op_replace_exp(stm->val.exp);
            if (stm->val.exp->sym_type->kind == SYMBOL_CLASS)
            {

                act_list *al = make_act_list_empty();
                variable *var = make_var_func(stm->val.exp->val.term->val.var, "__print", al);

                term *t = make_term_var(var);
                stm->val.exp->val.term = t;

                stm->val.exp->kind = EXP_TERM;
                stm->val.exp->val.term->sym_type = make_symbol_type(SYMBOL_INT);
                stm->val.exp->sym_type = make_symbol_type(SYMBOL_INT);
            }
            break;
        case STM_ALLOCATE:
            obj_op_replace_variable(stm->val.var);
            break;
        case STM_ALLOCATE_OF_LENGTH:
            obj_op_replace_variable(stm->val.assignment.var);
            obj_op_replace_exp(stm->val.assignment.exp);
            break;
        case STM_ASSIGNMENT:
            obj_op_replace_variable(stm->val.assignment.var);
            obj_op_replace_exp(stm->val.assignment.exp);
            break;
        case STM_ASSIGN_LIST:
            obj_op_replace_variable(stm->val.assign_list.var);
            obj_op_replace_exp_list(stm->val.assign_list.el);
            break;
        case STM_IF:
            obj_op_replace_exp(stm->val.if_then.exp);
            obj_op_replace_statement(stm->val.if_then.stm);
            break;
        case STM_ELSE:
            obj_op_replace_exp(stm->val.if_then.exp);
            obj_op_replace_statement(stm->val.if_else.stm1);
            obj_op_replace_statement(stm->val.if_else.stm2);
            break;
        case STM_WHILE:
            obj_op_replace_exp(stm->val.while_do.exp);
            obj_op_replace_statement(stm->val.while_do.stm);
            break;
        case STM_LIST:
            obj_op_replace_statement_list(stm->val.sl);
            break;
        case STM_BREAK:
            break;
        case STM_CONTINUE:
            break;
        case STM_IN:
            obj_op_replace_variable(stm->val.var);
            break;
        case STM_FREE:
            obj_op_replace_variable(stm->val.var);
            break;
        case STM_ASSIGN_VAL:
            obj_op_replace_variable(stm->val.assignment.var);
            obj_op_replace_exp(stm->val.assignment.exp);
            break;
        case STM_SWITCH:
            obj_op_replace_exp(stm->val.switch_stm.exp);
            obj_op_replace_switch(stm->val.switch_stm.s_stm);
            break;
    }
}

void obj_op_replace_decl_list(decl_list *dl)
{
    switch (dl->kind)
    {
        case DL_DECL_DL:
            obj_op_replace_decl_list(dl->dl);
            obj_op_replace_declaration(dl->decl);
            break;
        case DL_EMPTY:
            break;
    }
}

void obj_op_replace_declaration(declaration *d)
{
    switch (d->kind)
    {
        case DECL_TYPE:
            obj_op_replace_type(d->val.type.type);
            break;
        case DECL_FUNC:
            obj_op_replace_function(d->val.func);
            break;
        case DECL_VDL:
            obj_op_replace_var_decl_list(d->val.vdl);
            break;
        case DECL_CONST:
            obj_op_replace_type(d->val.con.type);
            obj_op_replace_exp(d->val.con.exp);
            break;
    }
}

void obj_op_replace_variable(variable *var)
{
    switch (var->kind)
    {
        case VAR_ID:
            break;
        case VAR_EXP:
            obj_op_replace_variable(var->val.var_exp.var);
            obj_op_replace_exp_list(var->val.var_exp.exp);
            break;
        case VAR_RECORD:
            obj_op_replace_variable(var->val.var_exp.var);
            break;
        case VAR_CLASS:
            obj_op_replace_variable(var->val.var_exp.var);
            break;
        case VAR_FUNC:
            obj_op_replace_variable(var->val.var_exp.var);
            obj_op_replace_act_list(var->val.var_exp.al);
            break;
    }
}

void obj_op_replace_term(term *t)
{
    switch (t->kind)
    {
        case TERM_VAR:
            obj_op_replace_variable(t->val.var);
            break;
        case TERM_AL:
            obj_op_replace_act_list(t->val.term_act_list.al);
            break;
        case TERM_PARENTHESES:
            obj_op_replace_exp(t->val.exp);
            break;
        case TERM_NOT:
            obj_op_replace_term(t->val.not_term);
            break;
        case TERM_ABS:
            obj_op_replace_exp(t->val.exp);
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

void obj_op_replace_act_list(act_list *al)
{
    switch (al->kind)
    {
        case AL_EXP_LIST:
            obj_op_replace_exp_list(al->exp_list);
            break;
        case AL_EMPTY:
            break;
    }
}

void obj_op_replace_exp_list(expression_list *exp_list)
{
    if (exp_list == NULL)
    {
        return;
    }
    obj_op_replace_exp(exp_list->exp);

    if (exp_list->exp_list != NULL)
    {
        obj_op_replace_exp_list(exp_list->exp_list);
    }
}

void gen_func_call(expression *exp, char *func_name, int exp_sym_type)
{
    expression *l = exp->val.op.left, *r = exp->val.op.right;

    if (l->val.term->sym_type->kind == SYMBOL_NULL || r->val.term->sym_type->kind == SYMBOL_NULL)
    {
        return;
    }
    if (l->kind == EXP_TERM && l->val.term->sym_type->kind == SYMBOL_CLASS)
    {

        expression_list *exp_list = make_exp_list_exp(r);
        act_list *al = make_act_list(exp_list);
        variable *var;
        var = make_var_func(l->val.term->val.var, func_name, al);

        term *t = make_term_var(var);
        exp->val.term = t;

        exp->kind = EXP_TERM;
        exp->val.term->sym_type = make_symbol_type(exp_sym_type);
        exp->sym_type = make_symbol_type(exp_sym_type);
        //exp->sym_type->kind = exp_sym_type;
    }
}

void obj_op_replace_exp(expression *exp)
{
    switch (exp->kind)
    {
        case EXP_TIMES:
            obj_op_replace_exp(exp->val.op.left);
            obj_op_replace_exp(exp->val.op.right);
            gen_func_call(exp, "__times", SYMBOL_INT);
            break;
        case EXP_DIV:
            obj_op_replace_exp(exp->val.op.left);
            obj_op_replace_exp(exp->val.op.right);
            gen_func_call(exp, "__div", SYMBOL_INT);
            break;
        case EXP_PLUS:
            obj_op_replace_exp(exp->val.op.left);
            obj_op_replace_exp(exp->val.op.right);
            gen_func_call(exp, "__plus", SYMBOL_INT);
            break;
        case EXP_MINUS:
            obj_op_replace_exp(exp->val.op.left);
            obj_op_replace_exp(exp->val.op.right);
            gen_func_call(exp, "__minus", SYMBOL_INT);
            break;
        case EXP_TERM:
            obj_op_replace_term(exp->val.term);
            break;
        case EXP_EQ:
            obj_op_replace_exp(exp->val.op.left);
            obj_op_replace_exp(exp->val.op.right);
            gen_func_call(exp, "__equal", SYMBOL_BOOL);
            break;
        case EXP_NEQ:
            obj_op_replace_exp(exp->val.op.left);
            obj_op_replace_exp(exp->val.op.right);
            gen_func_call(exp, "__nequal", SYMBOL_BOOL);
            break;
        case EXP_GREATER:
            obj_op_replace_exp(exp->val.op.left);
            obj_op_replace_exp(exp->val.op.right);
            gen_func_call(exp, "__greater", SYMBOL_BOOL);
            break;
        case EXP_LESS:
            obj_op_replace_exp(exp->val.op.left);
            obj_op_replace_exp(exp->val.op.right);
            gen_func_call(exp, "__less", SYMBOL_BOOL);
            break;
        case EXP_GEQ:
            obj_op_replace_exp(exp->val.op.left);
            obj_op_replace_exp(exp->val.op.right);
            gen_func_call(exp, "__greaterEqual", SYMBOL_BOOL);
            break;
        case EXP_LEQ:
            obj_op_replace_exp(exp->val.op.left);
            obj_op_replace_exp(exp->val.op.right);
            gen_func_call(exp, "__lessEqual", SYMBOL_BOOL);
            break;
        case EXP_AND:
            obj_op_replace_exp(exp->val.op.left);
            obj_op_replace_exp(exp->val.op.right);
            break;
        case EXP_OR:
            obj_op_replace_exp(exp->val.op.left);
            obj_op_replace_exp(exp->val.op.right);
            break;
    }
}

void obj_op_replace_switch(switch_stm *s_stm)
{
    if (s_stm == NULL)
    {
        return;
    }
    obj_op_replace_switch_case(s_stm->switch_case);
    obj_op_replace_switch(s_stm->switch_list);

}

void obj_op_replace_switch_case(switch_case *s_case)
{
    obj_op_replace_exp(s_case->exp);
    obj_op_replace_statement(s_case->stm);
}

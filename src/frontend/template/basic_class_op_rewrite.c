#include "basic_class_op_rewrite.h"
#include <stdio.h>
#include "../../utils/string_util.h"
#include "../typecheck/type_calc.h"
#include "../symbol/symbol.h"

char *class_op_calc_new_name_func(char *old_name, par_decl_list *pdl)
{
    char *res = str_concat(old_name, "");
    var_decl_list *vdl = pdl->vdl;
    symbol *s;
    char *class_name;
    while (vdl != NULL)
    {
        switch (vdl->vt->type->sym_type->kind)
        {
            case SYMBOL_ID:
                res = str_concat(res, "_id");
                break;
            case SYMBOL_INT:
                res = str_concat(res, "_int");
                break;
            case SYMBOL_FLOAT:
                res = str_concat(res, "_float");
                break;
            case SYMBOL_BOOL:
                res = str_concat(res, "_bool");
                break;
            case SYMBOL_ARRAY:
                res = str_concat(res, "_array");
                break;
            case SYMBOL_RECORD:
                res = str_concat(res, "_record");
                break;
            case SYMBOL_FUNC:
                res = str_concat(res, "_func");
                break;
            case SYMBOL_NULL:
                res = str_concat(res, "_null");
                break;
            case SYMBOL_UNKNOWN:
                res = str_concat(res, "_unknown");
                break;
            case SYMBOL_CLASS:
                class_name = vdl->vt->type->sym_type->val._class.name;

                s = get_symbol(vdl->vt->type->st, vdl->vt->type->sym_type->val._class.name);
                while (s->sym_type->val.type_p->kind != TYPE_CLASS)
                {
                    class_name = s->sym_type->val.type_p->id;
                    s = get_symbol(vdl->vt->type->st, s->sym_type->val.type_p->id);
                }
                res = str_concat(res, "_");
                res = str_concat(res, class_name);
                break;
            case SYMBOL_STRING:
                res = str_concat(res, "_string");
                break;
            case SYMBOL_TEMPLATED:
                res = str_concat(res, "_template");
                break;
        }
        vdl = vdl->vdl;
    }

    return res;
}

void basic_class_op_rewrite(body *b)
{
    basic_class_op_rewrite_body(b);
}

void basic_class_op_rewrite_body(body *b)
{
    basic_class_op_rewrite_decl_list(b->dl);
    basic_class_op_rewrite_statement_list(b->sl);
}

void basic_class_op_rewrite_function(function *f)
{
    basic_class_op_rewrite_head(f->head);
    basic_class_op_rewrite_body(f->body);
}

void basic_class_op_rewrite_head(head *h)
{
    basic_class_op_rewrite_par_decl_list(h->pdl);
    basic_class_op_rewrite_type(h->type);

    if (h->id[0] == '_' && h->id[1] == '_')
    {
        h->id = class_op_calc_new_name_func(h->id, h->pdl);
        h->original_name = h->id;
    }
}


void basic_class_op_rewrite_type(type *t)
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
            basic_class_op_rewrite_type(t->val.type);
            break;
        case TYPE_RECORD:
            basic_class_op_rewrite_var_decl_list(t->val.vdl);
            break;
        case TYPE_CLASS:
            basic_class_op_rewrite_decl_list(t->val._class.dl);
            break;
    }
}

void basic_class_op_rewrite_par_decl_list(par_decl_list *pdl)
{
    switch (pdl->kind)
    {
        case PDL_VDL:
            basic_class_op_rewrite_var_decl_list(pdl->vdl);
            break;
        case PDL_EMPTY:
            break;
    }
}

void basic_class_op_rewrite_var_decl_list(var_decl_list *vdl)
{
    switch (vdl->kind)
    {
        case VDL_VT_VDL:
            basic_class_op_rewrite_var_type(vdl->vt);
            basic_class_op_rewrite_var_decl_list(vdl->vdl);
            break;
        case VDL_VT:
            basic_class_op_rewrite_var_type(vdl->vt);
            break;
    }
}

void basic_class_op_rewrite_var_type(var_type *vt)
{
    basic_class_op_rewrite_type(vt->type);
}

void basic_class_op_rewrite_statement_list(statement_list *sl)
{
    if (sl == NULL)
        return;

    basic_class_op_rewrite_statement_list(sl->sl);
    basic_class_op_rewrite_statement(sl->stm);
}

void basic_class_op_rewrite_statement(statement *stm)
{
    switch (stm->kind)
    {
        case STM_RETURN:
            basic_class_op_rewrite_exp(stm->val.exp);
            break;
        case STM_WRITE:
            basic_class_op_rewrite_exp(stm->val.exp);
            break;
        case STM_ALLOCATE:
            basic_class_op_rewrite_variable(stm->val.var);
            break;
        case STM_ALLOCATE_OF_LENGTH:
            basic_class_op_rewrite_variable(stm->val.assignment.var);
            basic_class_op_rewrite_exp(stm->val.assignment.exp);
            break;
        case STM_ASSIGNMENT:
            basic_class_op_rewrite_variable(stm->val.assignment.var);
            basic_class_op_rewrite_exp(stm->val.assignment.exp);
            break;
        case STM_ASSIGN_LIST:
            basic_class_op_rewrite_variable(stm->val.assign_list.var);
            basic_class_op_rewrite_exp_list(stm->val.assign_list.el);
            break;
        case STM_IF:
            basic_class_op_rewrite_exp(stm->val.if_then.exp);
            basic_class_op_rewrite_statement(stm->val.if_then.stm);
            break;
        case STM_ELSE:
            basic_class_op_rewrite_exp(stm->val.if_then.exp);
            basic_class_op_rewrite_statement(stm->val.if_else.stm1);
            basic_class_op_rewrite_statement(stm->val.if_else.stm2);
            break;
        case STM_WHILE:
            basic_class_op_rewrite_exp(stm->val.while_do.exp);
            basic_class_op_rewrite_statement(stm->val.while_do.stm);
            break;
        case STM_LIST:
            basic_class_op_rewrite_statement_list(stm->val.sl);
            break;
        case STM_BREAK:
            break;
        case STM_CONTINUE:
            break;
        case STM_IN:
            basic_class_op_rewrite_variable(stm->val.var);
            break;
        case STM_FREE:
            basic_class_op_rewrite_variable(stm->val.var);
            break;
        case STM_ASSIGN_VAL:
            basic_class_op_rewrite_variable(stm->val.assignment.var);
            basic_class_op_rewrite_exp(stm->val.assignment.exp);
            break;
        case STM_SWITCH:
            basic_class_op_rewrite_exp(stm->val.switch_stm.exp);
            basic_class_op_rewrite_switch(stm->val.switch_stm.s_stm);
            break;
    }
}

void basic_class_op_rewrite_decl_list(decl_list *dl)
{
    switch (dl->kind)
    {
        case DL_DECL_DL:
            basic_class_op_rewrite_decl_list(dl->dl);
            basic_class_op_rewrite_declaration(dl->decl);
            break;
        case DL_EMPTY:
            break;
    }
}

void basic_class_op_rewrite_declaration(declaration *d)
{
    switch (d->kind)
    {
        case DECL_TYPE:
            basic_class_op_rewrite_type(d->val.type.type);
            break;
        case DECL_FUNC:
            basic_class_op_rewrite_function(d->val.func);
            break;
        case DECL_VDL:
            basic_class_op_rewrite_var_decl_list(d->val.vdl);
            break;
        case DECL_CONST:
            basic_class_op_rewrite_type(d->val.con.type);
            basic_class_op_rewrite_exp(d->val.con.exp);
            break;
    }
}

char *class_op_calc_new_name_call(char *old_name, act_list *al)
{
    char *res = str_concat(old_name, "");
    expression_list *el = al->exp_list;
    symbol *s;
    char *class_name;
    while (el != NULL)
    {
        switch (el->exp->sym_type->kind)
        {
            case SYMBOL_ID:
                res = str_concat(res, "_id");
                break;
            case SYMBOL_INT:
                res = str_concat(res, "_int");
                break;
            case SYMBOL_FLOAT:
                res = str_concat(res, "_float");
                break;
            case SYMBOL_BOOL:
                res = str_concat(res, "_bool");
                break;
            case SYMBOL_ARRAY:
                res = str_concat(res, "_array");
                break;
            case SYMBOL_RECORD:
                res = str_concat(res, "_record");
                break;
            case SYMBOL_FUNC:
                res = str_concat(res, "_func");
                break;
            case SYMBOL_NULL:
                res = str_concat(res, "_null");
                break;
            case SYMBOL_UNKNOWN:
                res = str_concat(res, "_unknown");
                break;
            case SYMBOL_CLASS:
                class_name = el->exp->sym_type->val._class.name;

                s = get_symbol(el->exp->st, el->exp->sym_type->val._class.name);
                while (s->sym_type->val.type_p->kind != TYPE_CLASS)
                {
                    class_name = s->sym_type->val.type_p->id;
                    s = get_symbol(el->exp->st, s->sym_type->val.type_p->id);
                }
                res = str_concat(res, "_");
                res = str_concat(res, class_name);
                break;
            case SYMBOL_STRING:
                res = str_concat(res, "_string");
                break;
            case SYMBOL_TEMPLATED:
                res = str_concat(res, "_template");
                break;
        }
        el = el->exp_list;
    }

    return res;
}

void basic_class_op_rewrite_variable(variable *var)
{
    symbol *s;
    switch (var->kind)
    {
        case VAR_ID:
            break;
        case VAR_EXP:
            basic_class_op_rewrite_variable(var->val.var_exp.var);
            basic_class_op_rewrite_exp_list(var->val.var_exp.exp);
            break;
        case VAR_RECORD:
            basic_class_op_rewrite_variable(var->val.var_exp.var);
            break;
        case VAR_CLASS:
            basic_class_op_rewrite_variable(var->val.var_exp.var);
            break;
        case VAR_FUNC:
            basic_class_op_rewrite_variable(var->val.var_exp.var);
            basic_class_op_rewrite_act_list(var->val.var_exp.al);

            if (var->val.var_exp.id[0] == '_' && var->val.var_exp.id[1] == '_')
            {
                s = get_symbol(var->st, var->val.var_exp.id);
                var->val.var_exp.id = class_op_calc_new_name_call(var->val.var_exp.id, var->val.var_exp.al);
                var->val.var_exp.original_name = var->val.var_exp.id;
                //printf("%p\n", s);
                //var->val.var_exp.func->head->id = var->val.var_exp.id;
            }
            break;
    }
}

void basic_class_op_rewrite_term(term *t)
{
    switch (t->kind)
    {
        case TERM_VAR:
            basic_class_op_rewrite_variable(t->val.var);
            break;
        case TERM_AL:
            basic_class_op_rewrite_act_list(t->val.term_act_list.al);
            break;
        case TERM_PARENTHESES:
            basic_class_op_rewrite_exp(t->val.exp);
            break;
        case TERM_NOT:
            basic_class_op_rewrite_term(t->val.not_term);
            break;
        case TERM_ABS:
            basic_class_op_rewrite_exp(t->val.exp);
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

void basic_class_op_rewrite_act_list(act_list *al)
{
    switch (al->kind)
    {
        case AL_EXP_LIST:
            basic_class_op_rewrite_exp_list(al->exp_list);
            break;
        case AL_EMPTY:
            break;
    }
}

void basic_class_op_rewrite_exp_list(expression_list *exp_list)
{
    if (exp_list == NULL)
    {
        return;
    }
    basic_class_op_rewrite_exp(exp_list->exp);

    if (exp_list->exp_list != NULL)
    {
        basic_class_op_rewrite_exp_list(exp_list->exp_list);
    }
}


void basic_class_op_rewrite_exp(expression *exp)
{
    switch (exp->kind)
    {
        case EXP_TIMES:
            basic_class_op_rewrite_exp(exp->val.op.left);
            basic_class_op_rewrite_exp(exp->val.op.right);
            break;
        case EXP_DIV:
            basic_class_op_rewrite_exp(exp->val.op.left);
            basic_class_op_rewrite_exp(exp->val.op.right);
            break;
        case EXP_PLUS:
            basic_class_op_rewrite_exp(exp->val.op.left);
            basic_class_op_rewrite_exp(exp->val.op.right);
            break;
        case EXP_MINUS:
            basic_class_op_rewrite_exp(exp->val.op.left);
            basic_class_op_rewrite_exp(exp->val.op.right);
            break;
        case EXP_TERM:
            basic_class_op_rewrite_term(exp->val.term);
            break;
        case EXP_EQ:
            basic_class_op_rewrite_exp(exp->val.op.left);
            basic_class_op_rewrite_exp(exp->val.op.right);
            break;
        case EXP_NEQ:
            basic_class_op_rewrite_exp(exp->val.op.left);
            basic_class_op_rewrite_exp(exp->val.op.right);
            break;
        case EXP_GREATER:
            basic_class_op_rewrite_exp(exp->val.op.left);
            basic_class_op_rewrite_exp(exp->val.op.right);
            break;
        case EXP_LESS:
            basic_class_op_rewrite_exp(exp->val.op.left);
            basic_class_op_rewrite_exp(exp->val.op.right);
            break;
        case EXP_GEQ:
            basic_class_op_rewrite_exp(exp->val.op.left);
            basic_class_op_rewrite_exp(exp->val.op.right);
            break;
        case EXP_LEQ:
            basic_class_op_rewrite_exp(exp->val.op.left);
            basic_class_op_rewrite_exp(exp->val.op.right);
            break;
        case EXP_AND:
            basic_class_op_rewrite_exp(exp->val.op.left);
            basic_class_op_rewrite_exp(exp->val.op.right);
            break;
        case EXP_OR:
            basic_class_op_rewrite_exp(exp->val.op.left);
            basic_class_op_rewrite_exp(exp->val.op.right);
            break;
    }
}

void basic_class_op_rewrite_switch(switch_stm *s_stm)
{
    if (s_stm == NULL)
    {
        return;
    }
    basic_class_op_rewrite_switch_case(s_stm->switch_case);
    basic_class_op_rewrite_switch(s_stm->switch_list);

}

void basic_class_op_rewrite_switch_case(switch_case *s_case)
{
    basic_class_op_rewrite_exp(s_case->exp);
    basic_class_op_rewrite_statement(s_case->stm);
}
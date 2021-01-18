#include <stdlib.h>
#include <stdio.h>
#include "func_inliner.h"
#include "../../../utils/vector.h"
#include "../../../utils/string_util.h"
#include "../../../utils/ast_copy.h"
#include "new_names.h"
#include "../../../frontend/parser/pretty.h"


extern char *suffix;
extern function *called_func;
statement_list *cur_sl;
body *func_body;

vector *funcs;
bool has_changed;
bool collect_garbage;
int suffix_id;

void inliner_inline(body *b)
{
    funcs = vector_init();
    collect_garbage = false;
    suffix_id = 0;

    //new_name(b);

    do {
        has_changed = false;
        inliner_collect_program(b);
        inliner_inline_body(b);
    } while (has_changed);


    collect_garbage = true;
    inliner_collect_body(b);
    inliner_rm_body(b);
}

void inliner_inline_body(body *b)
{
    inliner_inline_decl_list(b->dl);
    func_body = b;
    inliner_inline_statement_list(b->sl);
}

void inliner_inline_function(function *f)
{
    vector_push(funcs, f);
    inliner_inline_body(f->body);
    vector_pop(funcs);
}


void inliner_inline_statement_list(statement_list *sl)
{

    if (sl == NULL)
        return;

    inliner_inline_statement_list(sl->sl);
    cur_sl = sl;
    inliner_inline_statement(sl->stm);
    return;

    switch (sl->kind)
    {
        case SL_STM:
            cur_sl = sl;
            inliner_inline_statement(sl->stm);
            break;
        case SL_STM_SL:
            inliner_inline_statement_list(sl->sl);
            cur_sl = sl;
            inliner_inline_statement(sl->stm);
            break;
    }
}

void inliner_inline_statement(statement *stm)
{
    switch (stm->kind)
    {
        case STM_RETURN:
            inliner_inline_exp(stm->val.exp);
            break;
        case STM_WRITE:
            inliner_inline_exp(stm->val.exp);
            break;
        case STM_ALLOCATE_OF_LENGTH:
            inliner_inline_exp(stm->val.assignment.exp);
            break;
        case STM_ASSIGNMENT:
            inliner_inline_exp(stm->val.assignment.exp);
            break;
        case STM_ASSIGN_LIST:
            break;
        case STM_IF:
            inliner_inline_exp(stm->val.if_then.exp);
            inliner_inline_statement(stm->val.if_then.stm);
            break;
        case STM_ELSE:
            inliner_inline_exp(stm->val.if_else.exp);
            inliner_inline_statement(stm->val.if_else.stm1);
            inliner_inline_statement(stm->val.if_else.stm2);
            break;
        case STM_WHILE:
            inliner_inline_exp(stm->val.while_do.exp);
            inliner_inline_statement(stm->val.while_do.stm);
            break;
        case STM_LIST:
            inliner_inline_statement_list(stm->val.sl);
            break;
        case STM_ALLOCATE:
        case STM_BREAK:
        case STM_CONTINUE:
            break;
        case STM_IN:
        case STM_FREE:
            break;
        case STM_ASSIGN_VAL:
            inliner_inline_exp(stm->val.assignment.exp);
            break;
        case STM_SWITCH:
            inliner_inline_exp(stm->val.switch_stm.exp);
            inliner_inline_switch(stm->val.switch_stm.s_stm);
            break;
    }
}

void inliner_inline_decl_list(decl_list *dl)
{
    switch (dl->kind)
    {
        case DL_DECL_DL:
            inliner_inline_declaration(dl->decl);
            inliner_inline_decl_list(dl->dl);
            break;
        case DL_EMPTY:
            break;
    }
}

void inliner_inline_declaration(declaration *d)
{
    switch (d->kind)
    {
        case DECL_FUNC:
            inliner_inline_function(d->val.func);
            break;
        case DECL_CONST:
            inliner_inline_exp(d->val.con.exp);
            break;
        case DECL_TYPE:
        case DECL_VDL:
            break;
    }
}

void inliner_inline_variable(variable *var)
{
    switch (var->kind)
    {
        case VAR_ID:
            break;
        case VAR_EXP:
            inliner_inline_variable(var->val.var_exp.var);
            inliner_inline_exp_list(var->val.var_exp.exp);
            break;
        case VAR_RECORD:
            inliner_inline_variable(var->val.var_exp.var);
            break;
        case VAR_CLASS:
            inliner_inline_variable(var->val.var_exp.var);
            break;
        case VAR_FUNC:
            inliner_inline_variable(var->val.var_exp.var);
            break;
    }
}

void inliner_inline_term(term *t)
{
    function *called_function;
    symbol *s;
    inline_hint *ilh;
    char *var_return;

    statement *tmp_stm;
    statement_list *new_sl, *last, *param_sl, *param_sl_last, *tmp_sl;

    declaration *tmp_decl, *ret_decl;
    decl_list *new_dl, *new_param_dl, *this_dl, *last_dl, *ret_dl;
    var_decl_list *new_param_vdl, *param_vdl, *ret_vdl;

    type *ret_type;
    var_type *ret_vt;

    variable *tmp_var, *tmp_ret_var;
    expression_list *exp_list;
    char *func_id;
    switch (t->kind)
    {
        case TERM_AL:
            func_id = t->val.term_act_list.id;

            s = get_symbol(t->st, func_id);
            called_function = s->sym_type->val.func.func;
            ilh = called_function->il_hint;

            if (ilh->kind == IL_NOT && !strong_inline)
            {
                return;
            }
            if (ilh->calls_func)
            {
                return;
            }
            if (ilh->num_stm * ilh->num_called > MAX_STM_TIMES_CALLS &&
                !strong_inline && ilh->kind != IL_IL)
            {
                return;
            }
            suffix_id++;
            suffix = malloc(sizeof(char) * 10);
            sprintf(suffix, "_%i", suffix_id);

            merge_symbols(t->st, called_function->st, suffix);

            var_return = str_concat("ret", suffix);
            put_symbol(t->st, var_return, called_function->sym_type, 0);

            called_func = called_function;

            new_sl = ast_copy_statement_list(called_function->body->sl, t->st);

            last = new_sl;
            while (last->sl != NULL)
                last = last->sl;
            last->kind = SL_STM;


            new_dl = ast_copy_decl_list(called_function->body->dl, t->st);
            new_param_dl = make_dl_empty();
            new_param_dl->st = t->st;
            if (called_function->head->pdl->kind == PDL_VDL)
            {
                new_param_vdl =
                        ast_copy_var_decl_list(called_function->head->pdl->vdl,
                                               t->st);
                tmp_decl = make_declaration_vdl(make_access_modifier(AM_PUBLIC)
                        , new_param_vdl);
                tmp_decl->st = t->st;
                new_param_dl = make_dl_declaration(tmp_decl, new_param_dl);
                new_param_dl->st = t->st;
            }


            this_dl = func_body->dl;
            last_dl = this_dl;


            if (last_dl->kind == DL_DECL_DL)
            {
                while (last_dl->dl->kind == DL_DECL_DL)
                {
                    last_dl = last_dl->dl;
                }
                last_dl->dl = new_dl;
            }
            else
            {
                func_body->dl = new_dl;
                this_dl = func_body->dl;
            }

            last_dl = this_dl;

            if (last_dl->kind == DL_DECL_DL)
            {
                while (last_dl->dl->kind == DL_DECL_DL)
                {
                    last_dl = last_dl->dl;
                }
                last_dl->dl = new_param_dl;
            }
            else
            {
                func_body->dl = new_param_dl;
            }


            ret_type = ast_copy_type(called_function->head->type, t->st);
            ret_type->st = t->st;
            ret_type->sym_type = called_function->sym_type;
            ret_vt = make_var_type(str_concat("ret", suffix), ret_type);
            ret_vt->st = t->st;
            ret_vdl = make_vdl_vt(ret_vt);
            ret_vdl->st = t->st;
            ret_decl = make_declaration_vdl(make_access_modifier(AM_PUBLIC),
                    ret_vdl);
            ret_decl->st = t->st;
            ret_dl = make_dl_declaration(ret_decl, func_body->dl);
            ret_dl->st = t->st;
            func_body->dl = ret_dl;

            param_sl = NULL;
            param_sl_last = NULL;
            if (t->val.term_act_list.al->kind == AL_EXP_LIST)
            {
                exp_list = t->val.term_act_list.al->exp_list;
                param_vdl = called_function->head->pdl->vdl;

                vector *exp_v = vector_init();
                vector *vdl_v = vector_init();
                int size = 0;
                while (exp_list != NULL)
                {

                    vector_push(exp_v, exp_list->exp);
                    vector_push(vdl_v, param_vdl->vt);

                    exp_list = exp_list->exp_list;
                    param_vdl = param_vdl->vdl;
                    size++;
                }
                int i = 1;
                while (i <= size)
                {
                    expression *exp =
                            vector_get_element(exp_v, vector_size(exp_v) - i);
                    var_type *vt =
                            vector_get_element(vdl_v, vector_size(vdl_v) - i);

                    tmp_var = make_var_id(str_concat(vt->id, suffix));
                    tmp_var->sym_type = exp->sym_type;
                    tmp_var->st = t->st;
                    tmp_stm = make_statement_assignment(tmp_var, exp);
                    tmp_stm->st = t->st;


                    if (param_sl == NULL)
                    {
                        param_sl = make_sl_statement(tmp_stm);
                        param_sl_last = param_sl;
                    }
                    else
                    {
                        tmp_sl = make_sl_statement(tmp_stm);
                        param_sl_last->sl = tmp_sl;
                        param_sl_last = param_sl_last->sl;
                    }
                    i++;
                }
            }


            new_sl->stm->is_stm_end = true;
            new_sl->stm->return_label =
                    str_concat(str_concat(called_func->head->id, "_inline"),
                               suffix);

            last = new_sl;
            while (last->sl != NULL)
            {
                last = last->sl;
            }
            last->sl = param_sl;

            last = new_sl;
            while (last->sl != NULL)
            {
                last = last->sl;
            }
            last->sl = cur_sl->sl;
            cur_sl->sl = new_sl;



            t->kind = TERM_VAR;
            tmp_ret_var = make_var_id(str_concat("ret", suffix));
            tmp_ret_var->sym_type = called_function->sym_type;
            tmp_ret_var->st = t->st;
            t->val.var = tmp_ret_var;
            has_changed = true;

            break;
        case TERM_PARENTHESES:
            inliner_inline_exp(t->val.exp);
            break;
        case TERM_NOT:
            inliner_inline_term(t->val.not_term);
            break;
        case TERM_ABS:
            inliner_inline_exp(t->val.exp);
            break;
        case TERM_VAR:
        case TERM_INT:
        case TERM_FLOAT:
        case TERM_STRING:
        case TERM_TRUE:
        case TERM_FALSE:
        case TERM_NULL:
            break;
    }
}
void inliner_inline_exp_list(expression_list *exp_list)
{
    switch (exp_list->kind)
    {
        case EXP_LIST_EXP:
            inliner_inline_exp(exp_list->exp);
            break;
        case EXP_LIST_EXP_LIST:
            inliner_inline_exp(exp_list->exp);
            inliner_inline_exp_list(exp_list->exp_list);
            break;
    }
}

void inliner_inline_exp(expression *exp)
{
    switch (exp->kind)
    {
        case EXP_TERM:
            inliner_inline_term(exp->val.term);
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
            inliner_inline_exp(exp->val.op.left);
            inliner_inline_exp(exp->val.op.right);
            break;
        case EXP_AND:
        case EXP_OR:
            inliner_inline_exp(exp->val.op.left);
            // Right might not get evaluated
            break;
    }
}

void inliner_inline_switch(switch_stm *s_stm)
{
    if (s_stm == NULL)
    {
        return;
    }
    inliner_inline_switch_case(s_stm->switch_case);
    inliner_inline_switch(s_stm->switch_list);
}

void inliner_inline_switch_case(switch_case *s_case)
{
    inliner_inline_exp(s_case->exp);
    inliner_inline_statement(s_case->stm);
}

/*
 * Collect
 */
void inliner_collect_program(body *b)
{
    inliner_collect_decl_list(b->dl);
}

void inliner_collect_body(body *b)
{
    inliner_collect_decl_list(b->dl);
    inliner_collect_statement_list(b->sl);
}

void inliner_collect_function(function *f)
{
    // Some functions might have been inlined
    f->il_hint->calls_func = false;

    vector_push(funcs, f);
    inliner_collect_body(f->body);
    vector_pop(funcs);
}

void inliner_collect_statement_list(statement_list *sl)
{
    if (sl == NULL)
    {
        return;
    }

    inliner_collect_statement_list(sl->sl);
    inliner_collect_statement(sl->stm);
    return;
}

void inliner_collect_statement(statement *stm)
{
    switch (stm->kind)
    {
        case STM_RETURN:
            inliner_collect_exp(stm->val.exp);
            break;
        case STM_WRITE:
            inliner_collect_exp(stm->val.exp);
            break;
        case STM_ALLOCATE:
            inliner_collect_variable(stm->val.var);
            break;
        case STM_ALLOCATE_OF_LENGTH:
            inliner_collect_variable(stm->val.assignment.var);
            inliner_collect_exp(stm->val.assignment.exp);
            break;
        case STM_ASSIGNMENT:
            inliner_collect_variable(stm->val.assignment.var);
            inliner_collect_exp(stm->val.assignment.exp);
            break;
        case STM_ASSIGN_LIST:
            break;
        case STM_IF:
            inliner_collect_exp(stm->val.if_then.exp);
            inliner_collect_statement(stm->val.if_then.stm);
            break;
        case STM_ELSE:
            inliner_collect_exp(stm->val.if_else.exp);
            inliner_collect_statement(stm->val.if_else.stm1);
            inliner_collect_statement(stm->val.if_else.stm2);
            break;
        case STM_WHILE:
            inliner_collect_exp(stm->val.while_do.exp);
            inliner_collect_statement(stm->val.while_do.stm);
            break;
        case STM_LIST:
            inliner_collect_statement_list(stm->val.sl);
            break;
        case STM_BREAK:
            break;
        case STM_CONTINUE:
            break;
        case STM_IN:
            inliner_collect_variable(stm->val.var);
            break;
        case STM_FREE:
            inliner_collect_variable(stm->val.var);
            break;
        case STM_ASSIGN_VAL:
            inliner_collect_variable(stm->val.assignment.var);
            inliner_collect_exp(stm->val.assignment.exp);
            break;
        case STM_SWITCH:
            inliner_collect_exp(stm->val.switch_stm.exp);
            inliner_collect_switch(stm->val.switch_stm.s_stm);
            break;
    }
    // Count the number of stms in functions
    if (!collect_garbage)
    {
        function *called_f = vector_get_element(funcs, vector_size(funcs) - 1);
        called_f->il_hint->num_stm++;
    }
}

void inliner_collect_decl_list(decl_list *dl)
{
    switch (dl->kind)
    {
        case DL_DECL_DL:
            inliner_collect_decl_list(dl->dl);
            inliner_collect_declaration(dl->decl);
            break;
        case DL_EMPTY:
            break;
    }
}


void inliner_collect_declaration(declaration *d)
{
    switch (d->kind)
    {
        case DECL_FUNC:
            inliner_collect_function(d->val.func);
            break;
        case DECL_CONST:
            inliner_collect_exp(d->val.con.exp);
            break;
        case DECL_TYPE:
        case DECL_VDL:
            break;
    }
}

void inliner_collect_variable(variable *var)
{
    function *func;
    switch (var->kind)
    {
        case VAR_ID:
            break;
        case VAR_EXP:
            inliner_collect_variable(var->val.var_exp.var);
            inliner_collect_exp_list(var->val.var_exp.exp);
            break;
        case VAR_RECORD:
            inliner_collect_variable(var->val.var_exp.var);
            break;
        case VAR_CLASS:
            inliner_collect_variable(var->val.var_exp.var);
            break;
        case VAR_FUNC:
            inliner_collect_variable(var->val.var_exp.var);

            if (!collect_garbage)
            {
                func = vector_get_element(funcs, vector_size(funcs) - 1);
                func->il_hint->calls_func = true;
            }
            break;
    }
}

void inliner_collect_term(term *t)
{
    function *func;
    symbol *s;
    switch (t->kind)
    {
        case TERM_VAR:
            inliner_collect_variable(t->val.var);
            break;
        case TERM_AL:
            if (collect_garbage)
            {
                s = get_symbol(t->st, t->val.term_act_list.id);
                func = s->sym_type->val.func.func;
                func->il_hint->should_remove = true;
            }
            else
            {
                func = vector_get_element(funcs, vector_size(funcs) - 1);
                func->il_hint->calls_func = true;
            }
            break;
        case TERM_PARENTHESES:
            inliner_collect_exp(t->val.exp);
            break;
        case TERM_NOT:
            inliner_collect_term(t->val.not_term);
            break;
        case TERM_ABS:
            inliner_collect_exp(t->val.exp);
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

void inliner_collect_exp(expression *exp)
{
    switch (exp->kind)
    {
        case EXP_TERM:
            inliner_collect_term(exp->val.term);
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
            inliner_collect_exp(exp->val.op.left);
            inliner_collect_exp(exp->val.op.right);
            break;
    }
}

void inliner_collect_exp_list(expression_list *exp_list)
{
    if (exp_list == NULL)
    {
        return;
    }
    inliner_collect_exp(exp_list->exp);
    inliner_collect_exp_list(exp_list->exp_list);
}

void inliner_collect_switch(switch_stm *s_stm)
{
    if (s_stm == NULL)
    {
        return;
    }
    inliner_collect_switch_case(s_stm->switch_case);
    inliner_collect_switch(s_stm->switch_list);
}

void inliner_collect_switch_case(switch_case *s_case)
{
    inliner_collect_exp(s_case->exp);
    inliner_collect_statement(s_case->stm);
}

void inliner_rm_body(body *b)
{
    b->dl = inliner_rm_decl_list(b->dl);
}

decl_list *inliner_rm_decl_list(decl_list *dl)
{
    if (dl->dl != NULL)
    {
        dl->dl = inliner_rm_decl_list(dl->dl);
    }

    if (dl->kind == DL_EMPTY)
    {
        return dl;
    }

    dl->decl = inliner_rm_declaration(dl->decl);
    if (dl->decl == NULL && dl->kind == DL_DECL_DL)
    {
        return dl->dl;
    }

    return dl;
}

declaration *inliner_rm_declaration(declaration *d)
{
    if (d->kind == DECL_FUNC)
    {
        if (d->val.func->il_hint->should_remove)
        {
            inliner_rm_body(d->val.func->body);
            return d;
        }
        else
        {
            return NULL;
        }
    }
    return d;
}

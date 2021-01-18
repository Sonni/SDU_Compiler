#include <stdlib.h>
#include "ast_copy.h"
#include "string_util.h"
#include <string.h>
#include "../frontend/parser/pretty.h"
#include <stdio.h>

function *ast_copy_function(function *f, symbol_table *new_st)
{
    function *new_func;

        new_func = make_function(make_template(f->template->tl), make_inline_hint(f->il_hint->kind),
                                           ast_copy_head(f->head, new_st),
                                           ast_copy_body(f->body, new_st),
                                           make_tail(str_concat(f->tail->id,
                                                                suffix)));


    new_func->line_no = f->line_no;
    new_func->sym_type = f->sym_type;
    new_func->st = new_st;
    return new_func;
}

function *ast_copy_function2(function *f, symbol_table *new_st)
{
    function *new_func;
    if (f->template->kind == TEMP)
    {

        new_func = make_function(make_template(f->template->tl), make_inline_hint(f->il_hint->kind),
                                 ast_copy_head(f->head, new_st),
                                 ast_copy_body(f->body, new_st),
                                 make_tail(str_concat(f->tail->id,
                                                      suffix)));
    }
    else
    {


        new_func = make_function(make_template_empty(), make_inline_hint(IL_EMPTY),
                                 ast_copy_head(f->head, new_st),
                                 ast_copy_body(f->body, new_st),
                                 make_tail(str_concat(f->tail->id,
                                                      suffix)));
    }

    new_func->line_no = f->line_no;
    new_func->sym_type = f->sym_type;
    new_func->st = new_st;
    return new_func;
}

body *ast_copy_body(body *b, symbol_table *new_st)
{

    body *new_b = make_body(ast_copy_decl_list(b->dl, new_st),
                            ast_copy_statement_list(b->sl, new_st));
    new_b->sym_type = b->sym_type;
    new_b->st = new_st;
    new_b->line_no = b->line_no;
    return new_b;
}

head *ast_copy_head(head *h, symbol_table *new_st)
{
    head *new_head = make_head(str_concat(h->id, suffix),
                               ast_copy_par_decl_list(h->pdl, new_st),
                               ast_copy_type(h->type, new_st));

    new_head->st = new_st;
    new_head->sym_type = h->sym_type;
    new_head->line_no = h->line_no;


    return new_head;
}

par_decl_list *ast_copy_par_decl_list(par_decl_list *pdl, symbol_table *new_st)
{
    par_decl_list *new_pdl;
    switch (pdl->kind)
    {
        case PDL_VDL:
            new_pdl = make_pdl_vdl(ast_copy_vdl(pdl->vdl, new_st));
            break;
        case PDL_EMPTY:
            new_pdl = make_pdl_empty();
            break;
    }

    new_pdl->sym_type = pdl->sym_type;
    new_pdl->st = new_st;
    new_pdl->line_no = pdl->line_no;

    return new_pdl;
}

var_decl_list *ast_copy_vdl(var_decl_list *vdl, symbol_table *new_st)
{
    var_decl_list *new_vdl = NULL;
    switch (vdl->kind)
    {
        case VDL_VT_VDL:

            new_vdl = make_vdl_vt_list(ast_copy_var_type(vdl->vt, new_st), ast_copy_vdl(vdl->vdl, new_st));
            break;
        case VDL_VT:
            new_vdl = make_vdl_vt(ast_copy_var_type(vdl->vt, new_st));
            break;
    }
    new_vdl->st = new_st;
    new_vdl->sym_type = vdl->sym_type;
    new_vdl->line_no = vdl->line_no;
    return new_vdl;
}



type *ast_copy_type(type *t, symbol_table *new_st)
{
    type *new_type = NULL;
    char *parent_id;
    switch (t->kind)
    {
        case TYPE_ID:
            if (strcmp(suffix, "") == 0)
            {
                new_type = make_type_id(t->id, t->ta);
            }
            else
            {
                if (get_symbol_depth(t->st, t->id) == 0)
                {
                    new_type = make_type_id(str_concat(t->id, suffix), t->ta);
                }
                else
                {
                    new_type = make_type_id(t->id, t->ta);
                }
            }

            break;
        case TYPE_INT:
            new_type = make_type_int();
            break;
        case TYPE_FLOAT:
            new_type = make_type_float();
            break;
        case TYPE_BOOL:
            new_type = make_type_bool();
            break;
        case TYPE_ARRAY:
            new_type = make_type_array(ast_copy_type(t->val.type, new_st));
            break;
        case TYPE_RECORD:
            new_type = make_type_record(ast_copy_var_decl_list(t->val.vdl,
                                                               new_st));
            break;
        case TYPE_CLASS:
            if (t->val._class.pl->kind == PL_PARENT)
            {
                parent_id = str_concat(suffix, t->val._class.pl->id);
                new_type = make_type_class(make_template_empty(), make_parent_list(parent_id), ast_copy_decl_list(t->val._class.dl, new_st));
            }
            else
            {
                new_type = make_type_class(make_template_empty(), make_parent_list_empty(), ast_copy_decl_list(t->val._class.dl, new_st));
            }
            break;
        case TYPE_STRING:
            new_type = make_type_string();
            break;
    }
    new_type->st = new_st;
    new_type->sym_type = t->sym_type;
    new_type->real_type = t->real_type;
    new_type->line_no = t->line_no;
    return new_type;
}

var_decl_list *ast_copy_var_decl_list(var_decl_list *vdl, symbol_table *new_st)
{
    var_decl_list *new_vdl;
    if (vdl->kind == VDL_VT_VDL)
    {
        new_vdl = make_vdl_vt_list(ast_copy_var_type(vdl->vt, new_st),
                                   ast_copy_var_decl_list(vdl->vdl, new_st));
    }
    else
    {
        new_vdl = make_vdl_vt(ast_copy_var_type(vdl->vt, new_st));
    }
    new_vdl->st = new_st;
    new_vdl->sym_type = vdl->sym_type;
    new_vdl->line_no = vdl->line_no;
    return new_vdl;
}

var_type *ast_copy_var_type(var_type *vt, symbol_table *new_st)
{
    var_type *new_vt = make_var_type(str_concat(vt->id, suffix),
                                     ast_copy_type(vt->type, new_st));
    new_vt->st = new_st;
    new_vt->sym_type = vt->sym_type;
    new_vt->line_no = vt->line_no;
    return new_vt;
}

decl_list *ast_copy_decl_list(decl_list *dl, symbol_table *new_st)
{
    decl_list *new_dl;

    if (dl->kind == DL_DECL_DL)
    {
        if ((dl->decl->kind == DECL_FUNC && strcmp(suffix, "") != 0) && dl->decl->val.func->template->kind != TEMP_EMPTY)
        {
            new_dl = ast_copy_decl_list(dl->dl, new_st);
        }
        else
        {
            new_dl = make_dl_declaration(
                    ast_copy_declaration(dl->decl, new_st),
                    ast_copy_decl_list(dl->dl, new_st));

        }
    }
    else
    {
        new_dl = make_dl_empty();
    }

    new_dl->st = new_st;
    new_dl->sym_type = dl->sym_type;
    new_dl->line_no = dl->line_no;
    return new_dl;
}

declaration *ast_copy_declaration(declaration *d, symbol_table *new_st)
{
    declaration *new_decl = NULL;
    switch (d->kind)
    {
        case DECL_TYPE:

            new_decl = make_declaration_type(d->am,
                                             str_concat(d->val.type.id,
                                                        suffix),
                                             ast_copy_type(d->val.type.type,
                                                           new_st));

            break;
        case DECL_FUNC:
            //new_decl = d;

            if (strcmp(suffix, "") == 0)
            {
                new_decl = make_declaration_function(d->am, ast_copy_function2(d->val.func, new_st));
            }

            break;
        case DECL_VDL:
            new_decl = make_declaration_vdl(d->am,
                                            ast_copy_var_decl_list(d->val.vdl,
                                                                   new_st));
            break;
        case DECL_CONST:
            new_decl = make_decl_const(d->am, d->val.con.type, d->val.con.id,
                                       d->val.con.exp);
            break;
    }
    new_decl->st = new_st;
    new_decl->sym_type = d->sym_type;
    new_decl->line_no = d->line_no;
    return new_decl;
}

statement_list *ast_copy_statement_list(statement_list *sl,
                                        symbol_table *new_st)
{
   /* if (sl == NULL)
    {
        return NULL;
    }

    statement_list *tmp_list = ast_copy_statement_list(sl->sl, new_st);
    statement_list *new_sl =
            make_sl_statement_list(ast_copy_statement(sl->stm, new_st),
                                   tmp_list);
    new_sl->st = new_st;
    new_sl->sym_type = sl->sym_type;*/

    statement_list *tmp_list;
    statement_list *new_sl;


    switch (sl->kind)
    {
        case SL_STM:
             new_sl =
                    make_sl_statement(ast_copy_statement(sl->stm, new_st));
            break;
        case SL_STM_SL:
            tmp_list = ast_copy_statement_list(sl->sl, new_st);
            new_sl =
                    make_sl_statement_list(ast_copy_statement(sl->stm, new_st),
                                           tmp_list);
            break;
    }

    return new_sl;
}

statement *ast_copy_statement(statement *stm, symbol_table *new_st)
{
    statement *new_stm = NULL;
    //variable *tmp_var;
    switch (stm->kind)
    {
        case STM_RETURN:

           /* tmp_var = make_var_id(str_concat("ret", suffix));
            tmp_var->st = new_st;

            tmp_var->sym_type = called_func->sym_type;

            new_stm = make_statement_assignment(tmp_var,
                                                ast_copy_exp(stm->val.exp,
                                                             new_st));

            new_stm->is_return = true;
            new_stm->return_label =
                    str_concat(str_concat(called_func->head->id, "_inline"),
                               suffix);
                               */

           new_stm = make_statement_return(ast_copy_exp(stm->val.exp, new_st));

            break;
        case STM_WRITE:
            new_stm = make_statement_write(ast_copy_exp(stm->val.exp, new_st));
            break;
        case STM_ALLOCATE:
            new_stm = make_statement_allocate(ast_copy_variable(stm->val.var,
                                                                new_st));
            break;
        case STM_ALLOCATE_OF_LENGTH:
            new_stm = make_statement_allocate_length(
                    ast_copy_variable(stm->val.assignment.var, new_st),
                    ast_copy_exp(stm->val.assignment.exp, new_st));
            break;
        case STM_ASSIGNMENT:

            new_stm = make_statement_assignment(
                    ast_copy_variable(stm->val.assignment.var, new_st),
                    ast_copy_exp(stm->val.assignment.exp, new_st));

            break;
        case STM_ASSIGN_LIST:
            break;
        case STM_IF:
            new_stm = make_statement_if(
                    ast_copy_exp(stm->val.if_then.exp, new_st),
                    ast_copy_statement(stm->val.if_then.stm, new_st));
            break;
        case STM_ELSE:
            new_stm = make_statement_else(
                    ast_copy_exp(stm->val.if_else.exp, new_st),
                    ast_copy_statement(stm->val.if_else.stm1, new_st),
                    ast_copy_statement(stm->val.if_else.stm2, new_st));
            break;
        case STM_WHILE:
            new_stm = make_statement_while(
                    ast_copy_exp(stm->val.while_do.exp, new_st),
                    ast_copy_statement(stm->val.while_do.stm, new_st),
                    stm->val.while_do.kind);
            break;
        case STM_LIST:
            new_stm = make_statement_sl(ast_copy_statement_list(stm->val.sl,
                                                                new_st));
            break;
        case STM_BREAK:
            new_stm = make_statement_keyword(STM_BREAK);
            break;
        case STM_CONTINUE:
            new_stm = make_statement_keyword(STM_CONTINUE);
            break;
        case STM_IN:
            new_stm = make_statement_in(stm->val.var);
            break;
        case STM_FREE:
            new_stm = make_statement_free(stm->val.var);
            break;
        case STM_ASSIGN_VAL:
            new_stm = make_statement_assign_val(
                    ast_copy_variable(stm->val.assignment.var, new_st),
                    ast_copy_exp(stm->val.assignment.exp, new_st));
            break;
        case STM_SWITCH:
            new_stm = make_statement_switch(
                    ast_copy_exp(stm->val.switch_stm.exp, new_st),
                    ast_copy_switch(stm->val.switch_stm.s_stm, new_st));
            break;
    }
    new_stm->st = new_st;
    new_stm->func = stm->func;
    new_stm->has_skip = stm->has_skip;
    new_stm->has_return = false;
    new_stm->loop = stm->loop;
    new_stm->line_no = stm->line_no;
    return new_stm;
}

variable *ast_copy_variable(variable *var, symbol_table *new_st)
{
    variable *new_var = NULL;

    switch (var->kind)
    {
        case VAR_ID:
            if (strcmp(suffix, "") == 0)
            {
                new_var = make_var_id(var->val.var_exp.id);
            }
            else
            {
                if (get_symbol_depth(var->st, var->val.var_exp.id) == 0)
                {
                    new_var = make_var_id(str_concat(var->val.var_exp.id, suffix));
                }
                else
                {
                    new_var = make_var_id(var->val.var_exp.id);
                }
            }

            break;
        case VAR_EXP:
            new_var = make_var_exp(ast_copy_variable(var->val.var_exp.var,
                                                     new_st),
                                   ast_copy_exp_list(var->val.var_exp.exp,
                                                     new_st));
            break;
        case VAR_RECORD:
            new_var = make_var_dot(ast_copy_variable(var->val.var_exp.var,
                                                     new_st),
                                   var->val.var_exp.id);
            break;
        case VAR_CLASS:

            new_var = make_var_arrow(ast_copy_variable(var->val.var_exp.var,
                                                       new_st),
                                     var->val.var_exp.id);
            break;
        case VAR_FUNC:
            new_var = make_var_func(ast_copy_variable(var->val.var_exp.var,
                                                      new_st),
                                    var->val.var_exp.id, ast_copy_act_list(var->val.var_exp.al, new_st));
            break;
    }

    new_var->st = new_st;
    new_var->sym_type = var->sym_type;
    new_var->line_no = var->line_no;
    return new_var;
}

expression *ast_copy_exp(expression *exp, symbol_table *new_st)
{
    expression *new_exp;
    if (exp->kind == EXP_TERM)
    {
        new_exp = make_exp_term(ast_copy_term(exp->val.term, new_st));
    }
    else
    {
        new_exp = make_exp(exp->kind, ast_copy_exp(exp->val.op.left, new_st),
                           ast_copy_exp(exp->val.op.right, new_st));
    }

    new_exp->st = new_st;
    new_exp->sym_type = exp->sym_type;
    new_exp->line_no = exp->line_no;
    return new_exp;
}

term *ast_copy_term(term *t, symbol_table *new_st)
{
    term *new_t = NULL;
    switch (t->kind)
    {
        case TERM_VAR:

            new_t = make_term_var(ast_copy_variable(t->val.var, new_st));
            break;
        case TERM_AL:
            new_t = make_term_al(str_concat(t->val.term_act_list.id, suffix),
                                 t->val.term_act_list.temp_args,
                                 ast_copy_act_list(t->val.term_act_list.al,
                                                   new_st));
            break;
        case TERM_PARENTHESES:
            new_t = make_term_parentheses(ast_copy_exp(t->val.exp, new_st));
            break;
        case TERM_NOT:
            new_t = make_term_not(ast_copy_term(t->val.not_term, new_st));
            break;
        case TERM_ABS:
            new_t = make_term_absolute(ast_copy_exp(t->val.exp, new_st));
            break;
        case TERM_INT:
            new_t = make_term_int(t->val.integer);
            break;
        case TERM_FLOAT:
            new_t = make_term_float(t->val.floating_point);
            break;
        case TERM_TRUE:
            new_t = make_term(TERM_TRUE);
            break;
        case TERM_FALSE:
            new_t = make_term(TERM_NULL);
            break;
        case TERM_NULL:
            new_t = make_term(TERM_NULL);
            break;
        case TERM_STRING:
            new_t = make_term_string(t->val.string);
            break;
    }
    new_t->st = new_st;
    new_t->sym_type = t->sym_type;
    new_t->line_no = t->line_no;
    return new_t;
}

expression_list *ast_copy_exp_list(expression_list *el, symbol_table *new_st)
{
    if (el == NULL)
    {
        return NULL;
    }
    expression_list *new_sl = make_exp_list_list(ast_copy_exp(el->exp, new_st),
                                                 ast_copy_exp_list(
                                                         el->exp_list,
                                                         new_st));

    new_sl->st = new_st;
    new_sl->sym_type = el->sym_type;
    new_sl->line_no = el->line_no;

    return new_sl;
}

act_list *ast_copy_act_list(act_list *al, symbol_table *new_st)
{
    act_list *new_al;
    switch (al->kind)
    {
        case AL_EXP_LIST:
            new_al = make_act_list(ast_copy_exp_list(al->exp_list, new_st));
            break;
        case AL_EMPTY:
            new_al = make_act_list_empty();
            break;
    }
    new_al->st = new_st;
    new_al->sym_type = al->sym_type;
    new_al->line_no = al->line_no;
    return new_al;
}

switch_stm *ast_copy_switch(switch_stm *s_stm, symbol_table *new_st)
{
    if (s_stm == NULL)
    {
        return NULL;
    }
    switch_stm *new_s_stm = make_switch_stm_list(
            ast_copy_switch_case(s_stm->switch_case, new_st),
            ast_copy_switch(s_stm->switch_list, new_st));

    new_s_stm->st = new_st;
    new_s_stm->sym_type = s_stm->sym_type;
    new_s_stm->line_no = s_stm->line_no;
    return new_s_stm;
}

switch_case *ast_copy_switch_case(switch_case *s_case, symbol_table *new_st)
{
    switch_case *new_s_case = make_switch_case(
            ast_copy_exp(s_case->exp, new_st),
            ast_copy_statement(s_case->stm, new_st));
    new_s_case->st = new_st;
    new_s_case->sym_type = s_case->sym_type;
    new_s_case->line_no = s_case->line_no;
    return new_s_case;
}

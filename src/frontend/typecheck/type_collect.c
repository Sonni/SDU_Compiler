#include <stdio.h>
#include "../../utils/memory/memory.h"
#include "../../utils/vector.h"
#include "type_collect.h"
#include "../../utils/string_def.h"
#include "../../utils/string_util.h"
#include <stdlib.h>
#include "../parser/pretty.h"


vector *func_p;
vector *loop_p;
symbol_table *func_st;
int string_counter;
bool in_class;
int num_class_func;
vector *class_name;

bool in_templated_func;


char *get_types_exp_list(act_list *al)
{
    char *res = malloc(sizeof(char) * 100);
    res = str_concat("", "");

    expression_list *exp_list;
    if (al->kind == AL_EXP_LIST)
    {
        exp_list = al->exp_list;
        while (exp_list != NULL)
        {
            switch (exp_list->exp->sym_type->kind)
            {
                case SYMBOL_ID:
                    res = str_concat(res, "_A");
                    break;
                case SYMBOL_INT:
                    res = str_concat(res, ".int");
                    break;
                case SYMBOL_FLOAT:
                    res = str_concat(res, ".float");
                    break;
                case SYMBOL_BOOL:
                    res = str_concat(res, ".bool");
                    break;
                case SYMBOL_ARRAY:
                    res = str_concat(res, ".arr");
                    break;
                case SYMBOL_RECORD:
                    res = str_concat(res, ".rec");
                    break;
                case SYMBOL_FUNC:
                    res = str_concat(res, ".func");
                    break;
                case SYMBOL_NULL:
                    res = str_concat(res, ".null");
                    break;
                case SYMBOL_UNKNOWN:
                    res = str_concat(res, ".unknown");
                    break;
                case SYMBOL_CLASS:
                    res = str_concat(res, ".class");
                    break;
                case SYMBOL_STRING:
                    res = str_concat(res, ".string");
                    break;
                case SYMBOL_TEMPLATED:
                    res = str_concat(res, ".template");
                    break;

            }

            exp_list = exp_list->exp_list;
        }
    }
    return cpy_str(res);
}

char *get_types(par_decl_list *pdl)
{
    var_decl_list *vdl;
    char *res = malloc(sizeof(char) * 500);
    res = str_concat("", "");
    if (pdl->kind != PDL_EMPTY)
    {
        vdl = pdl->vdl;
        while (vdl != NULL)
        {
            switch (vdl->vt->type->kind)
            {
                case TYPE_ID:
                    // if we have a type var, it will also be named as #, this is a problem
                    res = str_concat(res, "_#");
                    break;
                case TYPE_INT:
                    res = str_concat(res, ".int");
                    break;
                case TYPE_FLOAT:
                    res = str_concat(res, ".float");
                    break;
                case TYPE_BOOL:
                    res = str_concat(res, ".bool");
                    break;
                case TYPE_ARRAY:
                    res = str_concat(res, ".arr");
                    break;
                case TYPE_RECORD:
                    res = str_concat(res, ".rec");
                    break;
                case TYPE_STRING:
                    res = str_concat(res, ".string");
                    break;
                case TYPE_CLASS:
                    res = str_concat(res, ".class");
                    break;
            }
            vdl = vdl->vdl;
        }
    }

    return cpy_str(res);
}

void collect(body *b)
{
    in_templated_func = false;
    func_decl_list = vector_init();
    temp_class_type = vector_init();
    temp_class_decl = vector_init();
    num_class_func = 0;
    string_counter = 0;
    in_class = false;
    func_st = init_symbol_table();
    func_p = vector_init();
    loop_p = vector_init();
    collect_body(b, init_symbol_table());
    class_name = vector_init();
    in_templated_func = false;
}

void collect_function(function *f, symbol_table *st)
{

    if (f->template->kind != TEMP_EMPTY)
    {
        in_templated_func = true;
    }
    symbol_table *next_st = scope_symbol_table(st);
    f->st = next_st;

    collect_template(f->template);
    collect_head(f->head, next_st, st);


    vector_push(func_p, f);
    collect_body(f->body, next_st);

    vector_pop(func_p);

}

void collect_body(body *b, symbol_table *st)
{

    b->st = st;
    collect_decl_list(b->dl, st);
    collect_statement_list(b->sl, st);

}


void collect_head(head *h, symbol_table *st, symbol_table *p)
{
    h->st = p;
    h->num_args = collect_par_decl_list(h->pdl, st);
    collect_type(h->type, p);
}


void collect_type(type *t, symbol_table *st)
{
    symbol_type *sym_t;
    t->st = st;
    int num_args = 0;
    switch (t->kind)
    {
        case TYPE_ID:
            collect_temp_args(t->ta, st);

            if (t->ta->kind != TEMP_ARG_EMPTY)
            {
                vector_push(temp_class_type, t);
            }
            break;
        case TYPE_INT:
            t->sym_type = make_symbol_type(SYMBOL_INT);
            break;
        case TYPE_FLOAT:
            t->sym_type = make_symbol_type(SYMBOL_FLOAT);
            break;
        case TYPE_BOOL:
            t->sym_type = make_symbol_type(SYMBOL_BOOL);
            break;
        case TYPE_STRING:
            t->sym_type = make_symbol_type(SYMBOL_STRING);

            break;
        case TYPE_ARRAY:
            sym_t = NEW(symbol_type);
            sym_t->kind = SYMBOL_ARRAY;
            t->sym_type = sym_t;
            collect_type(t->val.type, st);
            break;
        case TYPE_RECORD:
            sym_t = NEW(symbol_type);
            sym_t->kind = SYMBOL_RECORD;
            t->sym_type = sym_t;
            collect_var_decl_list(t->val.vdl, scope_symbol_table(st),
                                  &num_args);
            break;
        case TYPE_CLASS:
            sym_t = NEW(symbol_type);
            sym_t->kind = SYMBOL_CLASS;

            collect_template(t->val._class.temp);

            symbol_table *sym_table = scope_symbol_table(st);
            //symbol_table *sym_table = init_symbol_table();
            sym_t->val._class.st = sym_table;
            //sym_t->val.type_p = t;
            t->sym_type = sym_t;
            t->st = st;
            in_class = true;
            collect_decl_list(t->val._class.dl, sym_table);
            in_class = false;
            break;
    }
}

int collect_par_decl_list(par_decl_list *pdl, symbol_table *st)
{
    pdl->st = st;
    int num_args = 0;
    switch (pdl->kind)
    {
        case PDL_VDL:
            collect_var_decl_list(pdl->vdl, st, &num_args);
            break;
        case PDL_EMPTY:
            break;
    }
    return num_args;
}

void collect_var_decl_list(var_decl_list *vdl, symbol_table *st, int *num_args)
{
    vdl->st = st;
    (*num_args)++;
    switch (vdl->kind)
    {
        case VDL_VT_VDL:
            collect_var_type(vdl->vt, st, (*num_args));
            collect_var_decl_list(vdl->vdl, st, num_args);
            break;
        case VDL_VT:
            collect_var_type(vdl->vt, st, (*num_args));
            break;
    }
}

void collect_var_type(var_type *vt, symbol_table *st, int offset)
{
    vt->st = st;
    symbol *s;


    if (vt->type->kind == TYPE_CLASS)
    {
        symbol_type *sym_t = make_symbol_type(SYMBOL_CLASS);
        sym_t->access = vt->access;
        sym_t->val.type_p = vt->type;
        s = put_symbol(st, vt->id, sym_t, -1);
        if (s == NULL)
        {
            print_error(VAR_REDEF, vt->line_no, 1);
        }
        else
        {
            s->access = vt->access;
        }

    }
    else
    {
        symbol_type *sym_t = make_symbol_type(SYMBOL_UNKNOWN);
        sym_t->access = vt->access;
        s = put_symbol(st, vt->id, sym_t, -1);
        if (s == NULL)
        {
            print_error(VAR_REDEF, vt->line_no, 1);
        }
        else
        {
            s->access = vt->access;
        }
    }

    if (gb_collect)
    {
        s->offset = offset + 2;
    }
    else
    {
        s->offset = offset;
    }

    collect_type(vt->type, st);

}

void collect_statement_list(statement_list *sl, symbol_table *st)
{
    sl->st = st;
    switch (sl->kind)
    {
        case SL_STM:
            collect_statement(sl->stm, st);
            break;
        case SL_STM_SL:
            collect_statement(sl->stm, st);
            collect_statement_list(sl->sl, st);
            break;
    }
}

void collect_statement(statement *stm, symbol_table *st)
{
    stm->st = st;
    switch (stm->kind)
    {
        case STM_RETURN:
            collect_exp(stm->val.exp, st, stm);

            int scope_level = vector_size(func_p);
            if (scope_level == 0)
            {
                print_error(RETURN_NO_FUNC, stm->line_no, 1);
            }
            stm->func = vector_get_element(func_p, scope_level-1);

            break;
        case STM_WRITE:
            collect_exp(stm->val.exp, st, stm);
            break;
        case STM_ALLOCATE:
            collect_variable(stm->val.var, st, stm);
            break;
        case STM_ALLOCATE_OF_LENGTH:
            collect_variable(stm->val.assignment.var, st, stm);
            collect_exp(stm->val.assignment.exp, st, stm);
            break;
        case STM_ASSIGNMENT:
            collect_variable(stm->val.assignment.var, st, stm);
            collect_exp(stm->val.assignment.exp, st, stm);
            break;
        case STM_ASSIGN_LIST:
            collect_variable(stm->val.assign_list.var, st, stm);
            collect_exp_list(stm->val.assign_list.el, st, stm);
            break;
        case STM_IF:
            collect_exp(stm->val.if_then.exp, st, stm);
            collect_statement(stm->val.if_then.stm, st);
            break;
        case STM_ELSE:
            collect_exp(stm->val.if_then.exp, st, stm);
            collect_statement(stm->val.if_else.stm1, st);
            collect_statement(stm->val.if_else.stm2, st);
            break;
        case STM_WHILE:
            collect_exp(stm->val.while_do.exp, st, stm);
            collect_statement(stm->val.while_do.stm, st);
            break;
        case STM_LIST:
            collect_statement_list(stm->val.sl, st);
            break;
        case STM_BREAK:
        case STM_CONTINUE:
            break;
        case STM_IN:
            collect_variable(stm->val.var, st, stm);
            break;
        case STM_FREE:
            collect_variable(stm->val.var, st, stm);
            break;
        case STM_ASSIGN_VAL:
            collect_variable(stm->val.assignment.var, st, stm);
            collect_exp(stm->val.assignment.exp, st, stm);
            break;
        case STM_SWITCH:
            collect_exp(stm->val.switch_stm.exp, st, stm);
            collect_switch(stm->val.switch_stm.s_stm, st, stm);
            break;
    }
    stm->func = vector_get_element(func_p, vector_size(func_p)-1);

}

void collect_decl_list(decl_list *dl, symbol_table *st)
{
    dl->st = st;
    switch (dl->kind)
    {
        case DL_DECL_DL:
            if (vector_size(func_p) > 0 && dl->decl->kind == DECL_FUNC && dl->decl->val.func->template->kind == TEMP)
            {
                return;
            }
            if (dl->decl->kind == DECL_FUNC && dl->decl->val.func->template->kind == TEMP)
            { // The decl is a templated function, save it for later so it can be removed
                vector_push(func_decl_list, dl);
            }
            collect_decl_list(dl->dl, st);
            collect_declaration(dl->decl, st);
            break;
        case DL_EMPTY:
            break;
    }
}

void collect_declaration(declaration *d, symbol_table *st)
{
    d->st = st;
    symbol_type *sym_t = NEW(symbol_type);
    symbol *s;
    int num_args = 0;
    switch (d->kind)
    {
        case DECL_TYPE:
            sym_t->kind = SYMBOL_ID;
            sym_t->access = d->am->kind;
            s = put_symbol(st, d->val.type.id, sym_t, 1);
            if (s == NULL)
            {
                print_error(VAR_REDEF, d->line_no, 1);
            }
            else
            {
                s->access = d->am->kind;
            }

            if (d->val.type.type->kind == TYPE_CLASS && d->val.type.type->val._class.temp->kind == TEMP)
            {
                vector_push(temp_class_decl, d);
            }

            collect_type(d->val.type.type, st);

            sym_t->val.type_p = d->val.type.type;
            break;
        case DECL_FUNC:
            d->val.func->access = d->am->kind;
            collect_function(d->val.func, st);
            break;
        case DECL_VDL:
            collect_var_decl_list(d->val.vdl, st, &num_args);
            break;
        case DECL_CONST:
            sym_t->access = d->am->kind;
            if ((s = put_symbol(st, d->val.type.id, sym_t, 1)) == NULL)
            {
                print_error(VAR_REDEF, d->line_no, 1);
            }
            else
            {
                s->access = d->am->kind;
                s->value = -999; // This is used in check phase, -999=const var
                s->string = d->val.con.id;
                s->is_const = true;
                s->sym_type->exp = d->val.con.exp;
            }

            string_counter++;
            collect_type(d->val.con.type, st);
            sym_t->kind = d->val.con.type->sym_type->kind;

            collect_exp(d->val.con.exp, st, NULL);
            break;
    }
}

void collect_variable(variable *var, symbol_table *st, statement *stm)
{
    var->st = st;
    switch (var->kind)
    {
        case VAR_ID:
            break;
        case VAR_EXP:
            collect_variable(var->val.var_exp.var, st, stm);
            collect_exp_list(var->val.var_exp.exp, st, stm);
            break;
        case VAR_RECORD:
            collect_variable(var->val.var_exp.var, st, stm);
            break;
        case VAR_CLASS:
            collect_variable(var->val.var_exp.var, st, stm);
            break;
        case VAR_FUNC:
            collect_variable(var->val.var_exp.var, st, stm);
            collect_act_list(var->val.var_exp.al, st, stm);
            break;
    }
}

void collect_term(term *t, symbol_table *st, statement *stm)
{
    t->st = st;
    switch (t->kind)
    {
        case TERM_VAR:
            collect_variable(t->val.var, st, stm);
            break;
        case TERM_AL:
            if (t->val.term_act_list.temp_args->kind != TEMP_ARG_EMPTY)
            {
                collect_temp_args(t->val.term_act_list.temp_args, st);
                t->val.term_act_list.stm = stm;
            }

            collect_act_list(t->val.term_act_list.al, st, stm);

            break;
        case TERM_PARENTHESES:
            collect_exp(t->val.exp, st, stm);
            break;
        case TERM_NOT:
            collect_term(t->val.not_term, st, stm);
            break;
        case TERM_ABS:
            collect_exp(t->val.exp, st, stm);
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

void collect_act_list(act_list *al, symbol_table *st, statement *stm)
{
    al->st = st;

    switch (al->kind)
    {
        case AL_EXP_LIST:
            collect_exp_list(al->exp_list, st, stm);
            break;
        case AL_EMPTY:
            break;
    }

}

void collect_exp_list(expression_list *exp_list, symbol_table *st, statement *stm)
{
    exp_list->st = st;

    if (exp_list == NULL)
    {
        return;
    }
    collect_exp(exp_list->exp, st, stm);

    if (exp_list->exp_list != NULL)
    {
        collect_exp_list(exp_list->exp_list, st, stm);
    }
}

void collect_exp(expression *exp, symbol_table *st, statement *stm)
{
    exp->st = st;
    switch (exp->kind)
    {
        case EXP_TERM:
            collect_term(exp->val.term, st, stm);
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
            collect_exp(exp->val.op.left, st, stm);
            collect_exp(exp->val.op.right, st, stm);
            break;
    }
}

void collect_switch(switch_stm *s_stm, symbol_table *st, statement *stm)
{
    if (s_stm == NULL)
    {
        return;
    }
    s_stm->st = st;
    collect_switch_case(s_stm->switch_case, st, stm);
    collect_switch(s_stm->switch_list, st, stm);
}

void collect_switch_case(switch_case *s_case, symbol_table *st, statement *stm)
{
    s_case->st = st;
    collect_exp(s_case->exp, st, stm);
    collect_statement(s_case->stm, st);
}

void collect_template(template *temp)
{
if (temp->kind == TEMP)
{
temp->num_para = 0;
if (temp->tl->kind == TL_TDL)
{
temp_decl_list *tdl = temp->tl->tdl;
while (tdl->kind == TDL_ID_TDL)
{
temp->num_para++;
tdl = tdl->tdl;
}
temp->num_para++;
}
}

}

void collect_temp_args(temp_args *temp_args, symbol_table *st)
{
    temp_args->st = st;
    if (temp_args->kind == TEMP_ARG)
    {
        temp_args->num_args = 0;
        if (temp_args->tal->kind == TAL_TADL)
        {
            temp_args_decl_list *tadl = temp_args->tal->tadl;
            while (tadl->kind == TADL_TYPE_TADL)
            {
                temp_args->num_args++;
                tadl = tadl->tadl;
            }
            temp_args->num_args++;
        }
    }
    switch (temp_args->kind)
    {
        case TEMP:
            collect_tal(temp_args->tal, st);
            break;
        case TEMP_EMPTY:
            break;
    }
}

void collect_tal(temp_args_list *tal, symbol_table *st)
{
    tal->st = st;
    switch (tal->kind)
    {
        case TL_TDL:
            collect_tadl(tal->tadl, st);
            break;
        case TL_EMPTY:
            break;
    }
}

void collect_tadl(temp_args_decl_list *tadl, symbol_table *st)
{
    tadl->st = st;
    switch (tadl->kind)
    {
        case TDL_ID_TDL:
            collect_type(tadl->t, st);
            collect_tadl(tadl->tadl, st);
            break;
        case TDL_ID:
            collect_type(tadl->t, st);
            break;
    }
}

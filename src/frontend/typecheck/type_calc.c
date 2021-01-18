#include <stdio.h>
#include "type_calc.h"
#include "../symbol/symbol.h"
#include "../../utils/string_def.h"
#include "../../utils/string_def.h"
#include "../../utils/string_util.h"
#include <stdlib.h>
#include "../../utils/memory/memory.h"
#include "type_collect.h"

vector *func_p;
vector *loop_p;
symbol_table *func_st;
bool in_class;
int num_class_func;
bool in_func;
extern bool print_errors;


char *get_typess(par_decl_list *pdl)
{
    var_decl_list *vdl;
    char *res = malloc(sizeof(char) * 500);
    res = str_concat("", "");
    if (pdl->kind != PDL_EMPTY)
    {
        vdl = pdl->vdl;

        while (vdl != NULL)
        {
            type *real_type = find_type(vdl->vt->type);
            switch (real_type->kind)
            {
                case TYPE_ID:
                    res = str_concat(res, "_A");
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


/*
 * This function is used to find the type of variable assigned to
 * another variable of a certain type, example:
 * type a : int;
 * var b = a;
 */
type *find_type(type *t)
{
    type *new_t = t;

    if (t->has_been_visited)
    {
        print_error(TYPE_RECUR_DEF, t->line_no, 1);
        return NULL;
    }
    new_t->has_been_visited = true;
    if (t->kind == TYPE_ID)
    {
        symbol *s = get_symbol(t->st, t->id);

        if (s != NULL && s->sym_type->kind == SYMBOL_ID)
        {
            new_t = find_type(s->sym_type->val.type_p);
        }
        else
        {
            new_t = NULL;
        }
    }

    t->has_been_visited = false;

    return new_t;
}

void calc(body *b)
{

    num_class_func = 0;
    in_class = false;
    in_func = false;
    calc_body(b, false);
}

void calc_function(function *f, bool second_pass)
{
    if (true)
    {
        calc_head(f->head, f->template->kind != TEMP_EMPTY, second_pass, f->was_templated);


        char *func_id = str_concat(f->head->id, "");
        if (f->template->kind == TEMP_EMPTY && !second_pass && !f->was_templated)
        {
            if (func_id[0] != '_' || func_id[1] != '_')
            {
                func_id = str_concat(func_id, get_typess(f->head->pdl));
            }
        }

        char *new_name = func_id;
        if (in_class)
        {
            char func_num[100];
            sprintf(func_num, "_%i_", num_class_func);
            new_name = str_concat(func_num, func_id);
            num_class_func++;
        }
        symbol *s = get_symbol(func_st, func_id);
        if (s != NULL)
        {
            s->sym_type->val.func.func = f;
            s->access = f->access;
        }


        f->sym_type = f->head->sym_type;
        s = get_symbol(f->st, func_id);
        if (s == NULL || s->sym_type->kind != SYMBOL_FUNC)
        {
            print_error(FUNC_DEF_FAIL, f->line_no, 1);
        }
        else
        {
            s->sym_type->val.func.func = f;
            s->access = f->access;
        }

        f->head->id = func_id;
        f->head->new_name = new_name;
        if (f->template->kind != TEMP_EMPTY)
            return;

        calc_body(f->body, second_pass);
        f->sym_type = f->head->sym_type;
    }
}

void calc_body(body *b, bool second_pass)
{
calc_decl_list(b->dl, second_pass);
calc_statement_list(b->sl);
}

void calc_head(head *h, bool is_templated, bool second_pass, bool was_templated)
{
    symbol_type *sym_t = NEW(symbol_type);
    sym_t->kind = SYMBOL_FUNC;

    char *func_id = str_concat(h->id, "");
    if (!is_templated && !second_pass && !was_templated)
    {
        if (func_id[0] != '_' && func_id[1] != '_')
        {
            func_id = str_concat(h->id, get_typess(h->pdl));
        }
    }

    if (in_class)
    {
        char func_num[100];
        sprintf(func_num, "%i_", num_class_func);
    }

    symbol *s= put_symbol(h->st, func_id, sym_t, 1);

    if (s == NULL)
    {
    if (!second_pass && print_errors && !is_templated)
{
        print_error(FUNC_REDEF, h->line_no, 1);

}
    }
    else
    {
    s->is_func = true;
    }

    h->sym_type = sym_t;

    if (is_templated) return;

    calc_par_decl_list(h->pdl);
    calc_type(h->type);
    h->sym_type->val.func.return_type = h->type;
}

void calc_type(type *t)
{
    symbol *s;
    switch (t->kind)
    {
        case TYPE_ID:
            s = get_symbol(t->st, t->id);


            if (s == NULL || (s->sym_type->kind != SYMBOL_ID && s->sym_type->kind != SYMBOL_CLASS))
            {
                if (print_errors)
                {
                    print_error(TYPE_NOT_IN_ST, t->line_no, 1);
                }
                t->sym_type = make_symbol_type(SYMBOL_UNKNOWN);
                return;
            }
            else
            {


                type *real_type = find_type(s->sym_type->val.type_p);

                if (real_type == NULL)
                {

                    print_error(TYPE_NOT_FOUND, t->line_no, 1);
                    return;

                }
                else
                {
                    t->sym_type = real_type->sym_type;
                    // s->sym_type = t->sym_type; // WATCH OUT
                    if (t->sym_type->kind == SYMBOL_CLASS)
                    {
                        //t->kind = TYPE_CLASS;
                    }

                }
            }
            break;
        case TYPE_INT:
        case TYPE_FLOAT:
        case TYPE_BOOL:
        case TYPE_STRING:
            break;
        case TYPE_ARRAY:
            calc_type(t->val.type);
            t->sym_type->val.type_p = t->val.type;
            break;
        case TYPE_RECORD:
            calc_var_decl_list(t->val.vdl);
            t->sym_type->val.vdl = t->val.vdl;
            break;
        case TYPE_CLASS:
            in_class = true;
            calc_decl_list(t->val._class.dl, false); //TODO
            t->sym_type->val._class.dl = t->val._class.dl;
            t->sym_type->val._class.type_p = t;

            if (t->val._class.pl->kind == PL_PARENT)
            {
                s = get_symbol(t->st, t->val._class.pl->id);
                if (s == NULL)
                {
                    print_error("Parent not declared", t->line_no, 1);
                }
                t->val._class.dl->st->next =
                        s->sym_type->val.type_p->sym_type->val._class.st;

                //t->val._class.dl->st = merge_tables(t->val._class.dl->st);
                //dump_symbol_table(t->val._class.dl->st);


            }
            symbol_set_offset(t->val._class.dl->st);
            in_class = false;
            break;
    }
}

void calc_par_decl_list(par_decl_list *pdl)
{
    switch (pdl->kind)
    {
        case PDL_VDL:
            calc_var_decl_list(pdl->vdl);
            break;
        case PDL_EMPTY:
            break;
    }
}

void calc_var_decl_list(var_decl_list *vdl)
{
    switch (vdl->kind)
    {
        case VDL_VT_VDL:
            calc_var_type(vdl->vt);
            calc_var_decl_list(vdl->vdl);
            break;
        case VDL_VT:
            calc_var_type(vdl->vt);
            break;
    }
}

void calc_var_type(var_type *vt) {
    calc_type(vt->type);


    get_symbol(vt->st, vt->id)->sym_type = vt->type->sym_type;
    if (vt->type->kind == TYPE_ID)
    {
        symbol *s = get_symbol(vt->st, vt->type->id);

        if (vt->type->ta->kind == TEMP_ARG)
        {
            get_symbol(vt->st, vt->id)->is_templated = true;
        }
        else if (s != NULL && s->is_templated)
        {
            get_symbol(vt->st, vt->id)->is_templated = true;
        }

        if (get_symbol(vt->st, vt->id)->sym_type->kind == SYMBOL_CLASS)
        {
            vt->type->val._class.name = vt->type->id;
            get_symbol(vt->st, vt->id)->sym_type->val._class.name = vt->type->id;
        }
    }

}

void calc_statement_list(statement_list *sl)
{
    switch (sl->kind)
    {
        case SL_STM:
            calc_statement(sl->stm);
            break;
        case SL_STM_SL:
            calc_statement(sl->stm);
            calc_statement_list(sl->sl);
            break;
    }
}

void calc_statement(statement *stm)
{
    switch (stm->kind)
    {
        case STM_RETURN:
            calc_exp(stm->val.exp);
            break;
        case STM_WRITE:
            calc_exp(stm->val.exp);
            break;
        case STM_ALLOCATE:
            break;
        case STM_ALLOCATE_OF_LENGTH:
            calc_exp(stm->val.assignment.exp);
            break;
        case STM_ASSIGNMENT:
            calc_exp(stm->val.assignment.exp);
            break;
        case STM_ASSIGN_LIST:
            break;
        case STM_IF:
            calc_exp(stm->val.if_then.exp);
            break;
        case STM_ELSE:
            calc_exp(stm->val.if_then.exp);
            break;
        case STM_WHILE:
            calc_exp(stm->val.while_do.exp);
            break;
        case STM_LIST:
            calc_statement_list(stm->val.sl);
            break;
        case STM_BREAK:
        case STM_CONTINUE:
            break;
        case STM_IN:
            break;
        case STM_FREE:
            break;
        case STM_ASSIGN_VAL:
            calc_exp(stm->val.assignment.exp);
            break;
        case STM_SWITCH:
            calc_exp(stm->val.switch_stm.exp);
            break;
    }
    //stm->func = vector_get_element(func_p, vector_size(func_p)-1);

}

void calc_decl_list(decl_list *dl, bool second_pass)
{
    switch (dl->kind)
    {
        case DL_DECL_DL:
            calc_decl_list(dl->dl, second_pass);
            //if (dl->decl->kind == DECL_FUNC && dl->decl->val.func->template->kind != TEMP_EMPTY && second_pass)
            //  return;
            calc_declaration(dl->decl);
            break;
        case DL_EMPTY:
            break;
    }
}

void calc_declaration(declaration *d)
{
    switch (d->kind)
    {
        case DECL_TYPE:
            if (d->val.type.type->kind == TYPE_CLASS && d->val.type.type->val._class.temp->kind != TEMP_EMPTY)
            {
                return;
            }
            calc_type(d->val.type.type);

            if (d->val.type.type->kind == TYPE_ID)
            {
                if (d->val.type.type->ta->kind == TEMP_ARG)
                {
                    get_symbol(d->st, d->val.type.id)->is_templated = true;
                }
            }
            break;
        case DECL_FUNC:
            if (in_class && in_func)
            {
                print_error("No function declaration are allowed "
                            "inside of a function in a class", d->line_no, 1);
            }
            in_func = true;
            calc_function(d->val.func, false);
            in_func = false;

            break;
        case DECL_VDL:
            calc_var_decl_list(d->val.vdl);
            break;
        case DECL_CONST:
            calc_type(d->val.con.type);
            break;
    }
}

void calc_term(term *t)
{
    switch (t->kind)
    {
        case TERM_VAR:
            break;
        case TERM_AL:
            if (t->val.term_act_list.temp_args->kind != TEMP_ARG_EMPTY)
            {

                calc_temp_args(t->val.term_act_list.temp_args);
            }
            break;
        case TERM_PARENTHESES:
        case TERM_NOT:
        case TERM_ABS:
        case TERM_INT:
        case TERM_FLOAT:
        case TERM_TRUE:
        case TERM_FALSE:
        case TERM_NULL:
        case TERM_STRING:
            break;
    }
}

void calc_exp(expression *exp)
{
    switch (exp->kind)
    {
        case EXP_TERM:
            calc_term(exp->val.term);
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
            break;
    }
}

void calc_temp_args(temp_args *temp_args)
{
    switch (temp_args->kind)
    {
        case TEMP:
            calc_tal(temp_args->tal);
            break;
        case TEMP_EMPTY:
            break;
    }
}

void calc_tal(temp_args_list *tal)
{
    switch (tal->kind)
    {
        case TL_TDL:
            calc_tadl(tal->tadl);
            break;
        case TL_EMPTY:
            break;
    }
}

void calc_tadl(temp_args_decl_list *tadl)
{
    switch (tadl->kind)
    {
        case TDL_ID_TDL:
            calc_type(tadl->t);
            calc_tadl(tadl->tadl);
            break;
        case TDL_ID:
            calc_type(tadl->t);
            break;
    }
}

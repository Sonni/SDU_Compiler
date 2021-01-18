#include "gen_template.h"
#include "../symbol/symbol.h"
#include "../../utils/memory/memory.h"
#include "../../utils/ast_copy.h"
#include "../../utils/string_util.h"
#include "../typecheck/type_collect.h"
#include "../typecheck/type_calc.h"
#include "../typecheck/type_check.h"

#include "../parser/pretty.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "basic_class_op_rewrite.h"

extern vector *func_decl_list;
extern vector *temp_func_calls;
vector *generated_funcs;
extern char *suffix;

void delete_templates(body *b)
{
    decl_list *dl = b->dl;
    while (dl->kind != DL_EMPTY)
    {

        if ((dl->decl->kind == DECL_FUNC && dl->decl->val.func->template->kind != TEMP_EMPTY) ||
                (dl->decl->kind == DECL_TYPE && dl->decl->val.type.type->kind == TYPE_CLASS && dl->decl->val.type.type->val._class.temp->kind != TEMP_EMPTY))
        {
            dl = dl->dl;
        }
        else
        {
            break;
        }
    }

    b->dl = dl;
    decl_list *previous = dl;
    while (dl->kind != DL_EMPTY)
    {
        if ((dl->decl->kind == DECL_FUNC && dl->decl->val.func->template->kind != TEMP_EMPTY) ||
                (dl->decl->kind == DECL_TYPE && dl->decl->val.type.type->kind == TYPE_CLASS && dl->decl->val.type.type->val._class.temp->kind != TEMP_EMPTY))
        {
            previous->dl = dl->dl;
            dl = previous->dl;
            continue;
        }

        previous = dl;
        dl = dl->dl;
    }
}

void insert_new_func(function *func, decl_list *dl)
{
    dl->kind = DL_DECL_DL;
    dl->decl = make_declaration_function(make_access_modifier(AM_EMPTY), func);
    dl->dl = make_dl_empty();
}

bool is_match(term *t, decl_list *dl)
{
    function *temp_func = ast_copy_function(dl->decl->val.func, dl->st);
    char *func_call_name = gen_final_name(temp_func, t);
    gen_tl(dl->decl->val.func->template->tl, temp_func, t->val.term_act_list.temp_args->tal->tadl);

    collect_function(temp_func, temp_func->st);
    calc_function(temp_func, true);
    check_function(temp_func);

    char *func_name = class_op_calc_new_name_func(temp_func->head->id, temp_func->head->pdl);


    return strcmp(func_call_name, func_name) == 0;
}

vector *gen_template()
{
    generated_funcs = vector_init();
    suffix = malloc(sizeof(char) * 10);
    sprintf(suffix, "");


    for (int i = 0; i < (int) vector_size(temp_func_calls); i++)
    { // Check if func is templated, if so it does not exists in symbol table before the real function is generated
        term *t = vector_get_element(temp_func_calls, i);
        bool found_matching_gen_temp = false;
        for (int j = 0; j < (int) vector_size(generated_funcs); j++)
        {
            char *real_name = gen_final_name(vector_get_element(generated_funcs, j), t);
            function *func = vector_get_element(generated_funcs, j);
            if (strcmp(real_name, func->head->id) == 0)
            {
                found_matching_gen_temp = true;

                t->val.term_act_list.temp_id = real_name;
                t->val.term_act_list.id = real_name;
                t->val.term_act_list.temp_args->kind = TEMP_ARG_EMPTY;
                t->val.term_act_list.was_templated = true;
                t->val.term_act_list.func = func;


                break;
            }
        }
        if (found_matching_gen_temp)
        {
            continue;
        }

        bool found_match = false;

        for (int j = 0; j < (int) vector_size(func_decl_list); j++)
        { // Check if func is templated, if so it does not exists in symbol table before the real function is generated
            decl_list *dl = vector_get_element(func_decl_list, j);

            bool same_num_args = t->val.term_act_list.temp_args->num_args == dl->decl->val.func->template->num_para;


            if (strcmp(t->val.term_act_list.oldId, dl->decl->val.func->head->id) == 0 && same_num_args) {
                function *new_func = gen_new_function(dl, t);
                vector_push(generated_funcs, new_func);
                found_match = true;
                break;
            }
        }
        if (!found_match)
        {
            char *msg = str_concat("Found no match for call to templated function ", t->val.term_act_list.id);
            print_error(msg, t->line_no, 1);
            return NULL;
        }
    }



    if ((int) vector_size(func_decl_list) != 0)
    {
        decl_list *dl = vector_get_element(func_decl_list, vector_size(func_decl_list)-1);

        while (dl->kind != DL_EMPTY) {
            dl = dl->dl;
        }

        for (int i = 0; i < (int) vector_size(generated_funcs); i++)
        {
            function *f = vector_get_element(generated_funcs, i);
            insert_new_func(f, dl);
            dl = dl->dl;
        }
    }

    return generated_funcs;
}

function *gen_new_function(decl_list *dl, term *t)
{
    function *new_func = ast_copy_function(dl->decl->val.func, dl->st);

    new_func->template->kind = TEMP_EMPTY;
    template *old_tmp = new_func->template;
    new_func->template = dl->decl->val.func->template;

    if (dl->decl->val.func->template->kind != TEMP_EMPTY) {
       gen_tl(dl->decl->val.func->template->tl, new_func, t->val.term_act_list.temp_args->tal->tadl);
    }



    t->val.term_act_list.temp_id = gen_final_name(new_func, t);



    t->val.term_act_list.id = t->val.term_act_list.temp_id;
    t->val.term_act_list.original_name = t->val.term_act_list.temp_id;
    new_func->head->id = t->val.term_act_list.id;

    new_func->head->new_name = t->val.term_act_list.id;
    new_func->head->original_name = t->val.term_act_list.id;



    // Edit function call
    t->val.term_act_list.func = new_func;

    // Add the new function to symbol table
    symbol_type *sym_t = make_symbol_type(SYMBOL_FUNC);
    sym_t->val.func.func = new_func;
    sym_t->val.func.old_name = new_func->head->id;
    sym_t->val.func.return_type = new_func->head->type;
    put_symbol(dl->st, new_func->head->id, sym_t, 0);


    // Type check the new function
    /*collect_function(new_func, dl->st);
    calc_function(new_func, true);
    check_function(new_func);*/


    // Type check the new function call
    t->val.term_act_list.temp_args->kind = TEMP_ARG_EMPTY;
    t->val.term_act_list.was_templated = true;
    /*collect_statement(t->val.term_act_list.stm, t->val.term_act_list.stm->st);
    calc_statement(t->val.term_act_list.stm);
    check_statement(&t->val.term_act_list.stm);*/

    new_func->template = old_tmp;
    new_func->was_templated = true;


    return new_func;
}

void gen_function(function *func, char *type_name, type *t1)
{
    gen_head(func->head, type_name, t1);
    gen_body(func->body, type_name, t1);
}



void gen_head(head *h, char *type_name, type *type)
{

    gen_par_decl_list(h->pdl, type_name, type);

    gen_type(h->type, type_name, type);

}

void gen_body(body *b, char *type_name, type *t1)
{
    gen_decl_list(b->dl, type_name, t1);
}

void gen_decl_list(decl_list *dl, char *type_name, type *t1)
{
    switch (dl->kind)
    {
        case DL_DECL_DL:
            gen_decl_list(dl->dl, type_name, t1);
            gen_decl(dl->decl, type_name, t1);
            break;
        case DL_EMPTY:
            break;
    }
}

void gen_decl(declaration *d, char *type_name, type *t1)
{
    switch (d->kind)
    {
        case DECL_TYPE:
            gen_type(d->val.type.type, type_name, t1);
            break;
        case DECL_FUNC:
            if (d->val.func->template->kind == TEMP_EMPTY)
            {
                gen_function(d->val.func, type_name, t1);
            }
            break;
        case DECL_VDL:
            gen_var_decl_list(d->val.vdl, type_name, t1);
            break;
        case DECL_CONST:
            break;
    }
}

void gen_type(type *t1, char *type_name, type *t2)
{
    if (t1->kind == TYPE_ID && strcmp(type_name, t1->id) == 0)
    {
        t1->kind = t2->kind;
        t1->id = t2->id;
    }
}

void gen_par_decl_list(par_decl_list *pdl, char *type_name, type *type)
{
    switch (pdl->kind)
    {
        case PDL_VDL:
            gen_var_decl_list(pdl->vdl, type_name, type);
            break;
        case PDL_EMPTY:
            break;
    }
}

void gen_var_decl_list(var_decl_list *vdl, char *type_name, type *type)
{
    switch (vdl->kind)
    {
        case VDL_VT_VDL:
            if (vdl->vt->type->kind == 0 && strcmp(type_name, vdl->vt->type->id) == 0)
            {
                    symbol_type *sym_t = NEW(symbol_type);
                    sym_t->kind = type->sym_type->kind;
                    vdl->vt->type->sym_type = sym_t;

                    vdl->vt->type->id = type->id;
                if (type->kind == TYPE_CLASS)
                {
                }
                else
                {
                    vdl->vt->type->kind = type->kind;
                }
                vdl->vt->type->is_temp_arg = true;

            }


            gen_var_decl_list(vdl->vdl, type_name, type);
            break;
        case VDL_VT:

            if (vdl->vt->type->kind == 0 && strcmp(type_name, vdl->vt->type->id) == 0)
            {
                    symbol_type *sym_t = NEW(symbol_type);
                    sym_t->kind = type->sym_type->kind;
                    vdl->vt->type->sym_type = sym_t;
                    vdl->vt->type->id = type->id;
                if (type->kind == TYPE_CLASS)
                {
                    vdl->vt->type->id = type->id;

                }
                else
                {
                    vdl->vt->type->kind = type->kind;
                }
                vdl->vt->type->is_temp_arg = true;


            }

            break;
    }
}

void gen_tl(temp_list *tl, function *func, temp_args_decl_list *tadl)
{
    switch (tl->kind)
    {
        case TL_TDL:
            gen_tdl(tl->tdl, func, tadl);
            break;
        case TL_EMPTY:
            break;
    }
}

void gen_tdl(temp_decl_list *tdl, function *func, temp_args_decl_list *tadl)
{
    switch (tdl->kind)
    {
        case TDL_ID_TDL:
            gen_head(func->head, tdl->id, tadl->t);
            gen_body(func->body, tdl->id, tadl->t);
            // We assume that the size of tadl and tdl is the same (Implement this in the type checker or here)
            gen_tdl(tdl->tdl, func, tadl->tadl);
            break;
        case TDL_ID:
            gen_head(func->head, tdl->id, tadl->t);
            gen_body(func->body, tdl->id, tadl->t);
            break;
    }
}


char *gen_final_name(function *func, term *t)
{
    symbol *s;
    char *name;
    char *res = str_concat(t->val.term_act_list.temp_id, ".");

    if (func->head->pdl->kind != PDL_EMPTY)
    {
        var_decl_list *vdl = func->head->pdl->vdl;
        while (vdl != NULL)
        {
            if (!vdl->vt->type->is_temp_arg)
            {
                vdl = vdl->vdl;
                continue;
            }
            switch (vdl->vt->type->sym_type->kind)
            {
                case SYMBOL_ID:
                    s = get_symbol(t->st, vdl->vt->type->id);
                    if (s != NULL)
                    {
                        if (s->sym_type->kind == SYMBOL_CLASS)
                        {
                        }
                    }
                    // if we have a type var, it will also be named as #, this is a problem
                    res = str_replace(res, "#", ".ERROR");
                    break;
                case SYMBOL_INT:
                    res = str_replace(res, "#", ".int");
                    break;
                case SYMBOL_FLOAT:
                    res = str_replace(res, "#", ".float");
                    break;
                case SYMBOL_BOOL:
                    res = str_replace(res, "#", ".bool");
                    break;
                case SYMBOL_ARRAY:
                    res = str_replace(res, "#", ".arr");
                    break;
                case SYMBOL_RECORD:
                    res = str_replace(res, "#", ".rec");
                    break;
                case SYMBOL_STRING:
                    res = str_replace(res, "#", ".string");
                    break;
                case SYMBOL_CLASS:



                    name = str_concat(".", vdl->vt->type->id);
                    res = str_replace(res, "#", name);

                    break;
                case SYMBOL_FUNC:
                case SYMBOL_NULL:
                case SYMBOL_UNKNOWN:
                case SYMBOL_TEMPLATED:
                    break;
            }
            vdl = vdl->vdl;
        }
    }
    return res;
}

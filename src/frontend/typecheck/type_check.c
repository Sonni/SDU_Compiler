#include <stdio.h>
#include <stdlib.h>
#include "type_check.h"
#include "../../utils/string_def.h"
#include "../../utils/string_util.h"

#include "type_calc.h"
#include "type_collect.h"
#include <string.h>
#include "../parser/pretty.h"

extern vector *func_decl_list;


bool check_symbol_types(symbol_type *s1, symbol_type *s2)
{
    if (s1 == NULL || s2 == NULL)
    {
        return false;
    }
    if (s1->kind == s2->kind)
    {
        if (s1->kind == SYMBOL_INT || s2->kind == SYMBOL_BOOL)
        {
            return true;
        }
        else if (s1->kind == SYMBOL_STRING || s2->kind == SYMBOL_STRING)
        {
            return true;
        }
        else if (s1->kind == SYMBOL_CLASS || s2->kind == SYMBOL_CLASS)
        {
            return true;
        }
        else if (s1->kind == SYMBOL_FLOAT || s2->kind == SYMBOL_FLOAT)
        {
            return true;
        }
        else if (s1->kind == SYMBOL_ARRAY)
        {
            return check_symbol_types(s1->val.type_p->sym_type,
                                      s2->val.type_p->sym_type);
        }
        else if (s1->kind == SYMBOL_RECORD)
        { // TODO more elegant code
            if (s1->has_been_visited && s2->has_been_visited)
            {
                return true;
            }
            s1->has_been_visited = true;
            s2->has_been_visited = true;
            var_decl_list *vdl1 = s1->val.vdl;
            var_decl_list *vdl2 = s2->val.vdl;

            while (vdl1 != NULL && vdl2 != NULL)
            {
                symbol *sym1 = get_symbol(s1->val.type_p->st, vdl1->vt->id);
                symbol *sym2 = get_symbol(s2->val.type_p->st, vdl2->vt->id);
                if (sym1 == NULL || sym2 == NULL)
                {
                    s1->has_been_visited = false;
                    s2->has_been_visited = false;
                    return false;
                }
                else
                {
                    if (!check_symbol_types(sym1->sym_type,
                                            sym2->sym_type))
                    {
                        s1->has_been_visited = false;
                        s2->has_been_visited = false;
                        return false;
                    }
                }
                vdl1 = vdl1->vdl;
                vdl2 = vdl2->vdl;
            }

            if (vdl1 != NULL || vdl2 != NULL)
            {
                s1->has_been_visited = false;
                s2->has_been_visited = false;
                return false;
            }
            s1->has_been_visited = false;
            s2->has_been_visited = false;
            return true;
        }
        else if (s1->kind == SYMBOL_CLASS && s2->kind == SYMBOL_CLASS)
        {
            s1->has_been_visited = false;
            s2->has_been_visited = false;
            return true;
        }
    }
    if ((s1->kind == SYMBOL_BOOL && s2->kind == SYMBOL_INT) ||
        (s1->kind == SYMBOL_INT && s2->kind == SYMBOL_BOOL) ||
        (s1->kind == SYMBOL_FLOAT && s2->kind == SYMBOL_INT) ||
        (s1->kind == SYMBOL_INT && s2->kind == SYMBOL_FLOAT) ||
        (s1->kind == SYMBOL_FLOAT && s2->kind == SYMBOL_BOOL) ||
        (s1->kind == SYMBOL_BOOL && s2->kind == SYMBOL_FLOAT))
    {
        s1->has_been_visited = false;
        s2->has_been_visited = false;
        return true;
    }

    s1->has_been_visited = false;
    s2->has_been_visited = false;
    return false;
}

void check(body *b)
{
    temp_func_calls = vector_init();
    check_body(b);
}

void check_function(function *f)
{
    if (f->template->kind == TEMP_EMPTY) {
        check_head(f->head);

        check_body(f->body);

    }
}

void check_body(body *b)
{
    check_decl_list(b->dl);
    check_statement_list(b->sl);
}

void check_head(head *h)
{

    check_par_decl_list(h->pdl);

    check_type(h->type);

}

void check_type(type *t)
{
    int *offset;
    symbol_table *res;
    switch (t->kind)
    {
        case TYPE_ID:

            break;
        case TYPE_INT:
        case TYPE_FLOAT:
        case TYPE_BOOL:
        case TYPE_STRING:
            break;
        case TYPE_ARRAY:
            check_type(t->val.type);
            break;
        case TYPE_RECORD:
            check_var_decl_list(t->val.vdl);
            break;
        case TYPE_CLASS:
            check_decl_list(t->val._class.dl);

            res = init_symbol_table();
            offset = malloc(sizeof(int));
            if (gb_collect)
            {
                (*offset) = 3;
            }
            else
            {
                (*offset) = 1;
            }
            t->val._class.dl->st = merge_tables2(t->val._class.dl->st,
                                                 res, offset);

            break;
    }
}

void check_par_decl_list(par_decl_list *pdl)
{
    switch (pdl->kind)
    {
        case PDL_VDL:
            check_var_decl_list(pdl->vdl);
            break;
        case PDL_EMPTY:
            break;
    }
}

void check_var_decl_list(var_decl_list *vdl)
{
    switch (vdl->kind)
    {
        case VDL_VT_VDL:
            check_var_type(vdl->vt);
            check_var_decl_list(vdl->vdl);
            break;
        case VDL_VT:
            check_var_type(vdl->vt);
            break;
    }
}

void check_var_type(var_type *vt)
{
    check_type(vt->type);
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
    }
}

void check_statement_list(statement_list *sl)
{
    switch (sl->kind)
    {
        case SL_STM:
            check_statement(&sl->stm);
            break;
        case SL_STM_SL:
            check_statement(&sl->stm);
            check_statement_list(sl->sl);
            break;
    }
}

void check_statement(statement **stm_p)
{
    statement *stm = (*stm_p);
    symbol *s;
    int k;


    switch (stm->kind) {
        case STM_RETURN:
            check_exp(stm->val.exp);

            //check if expression depend on templated class
            if(stm->val.exp->sym_type != NULL &&
               stm->val.exp->sym_type->kind == SYMBOL_TEMPLATED)
            {
                return;
            }

            if (stm->val.exp->sym_type == NULL ||
                stm->func->sym_type->val.func.return_type->sym_type->kind !=
                stm->val.exp->sym_type->kind) {
                k = stm->func->sym_type->val.func.return_type->sym_type->kind;
                if (print_errors && (k != SYMBOL_RECORD ||
                                     stm->val.exp->sym_type->kind != SYMBOL_NULL) &&
                    (k != SYMBOL_ARRAY ||
                     stm->val.exp->sym_type->kind != SYMBOL_NULL)) {
                    print_error(WRONG_RETURN_TYPE, stm->line_no, 1);
                }
            }
            break;
        case STM_WRITE:
            check_exp(stm->val.exp);

            //check if expression depend on templated class
            if( stm->val.exp->sym_type != NULL &&
                stm->val.exp->sym_type->kind == SYMBOL_TEMPLATED)
            {
                return;
            }
            if (!print_errors) return;
            //Can write int, bools and (chars)
            if (stm->val.exp->sym_type != NULL) {
                int kind = stm->val.exp->sym_type->kind;
                if (kind != SYMBOL_INT && kind != SYMBOL_BOOL &&
                    kind != SYMBOL_STRING && kind != SYMBOL_FLOAT) {
                    if (kind == SYMBOL_ARRAY) {
                        k = stm->val.exp->sym_type->val.type_p->sym_type->kind;
                        if (k != SYMBOL_INT && k != SYMBOL_BOOL && k != SYMBOL_FLOAT) {
                            print_error(ONLY_INT_BOOL_WRITE, stm->line_no, 1);
                        }
                        break;
                    }
                    else {
                        print_error(ONLY_INT_BOOL_WRITE, stm->line_no, 1);
                    }
                }
            }
            break;
        case STM_ALLOCATE:
            check_variable(stm->val.var);

            if (stm->val.var->sym_type->kind != SYMBOL_RECORD &&
                stm->val.var->sym_type->kind != SYMBOL_CLASS) {
                print_error(ONLY_RECORD_ALLOC, stm->line_no, 1);
            }
            break;
        case STM_ALLOCATE_OF_LENGTH:
            check_variable(stm->val.assignment.var);
            check_exp(stm->val.assignment.exp);

            //check if expression depend on templated class
            if(stm->val.assignment.var->sym_type->kind == SYMBOL_TEMPLATED ||
               stm->val.assignment.exp->sym_type->kind == SYMBOL_TEMPLATED)
            {
                return;
            }

            if (stm->val.assignment.var->sym_type->kind != SYMBOL_ARRAY) {
                print_error(ONLY_ARR_ALLOC, stm->line_no, 1);
            }
            if (stm->val.assignment.exp->sym_type->kind != SYMBOL_INT) {
                print_error(MEM_MUST_BE_INT, stm->line_no, 1);
            }
            break;
        case STM_ASSIGNMENT:

            check_variable(stm->val.assignment.var);
            check_exp(stm->val.assignment.exp);



            //check if one of the operands depend on templated class
            if ((stm->val.assignment.var->sym_type != NULL && stm->val.assignment.var->sym_type->kind == SYMBOL_TEMPLATED) ||
                (stm->val.assignment.exp->sym_type != NULL && stm->val.assignment.exp->sym_type->kind == SYMBOL_TEMPLATED))
            {
                return;
            }


            if (stm->val.assignment.var->kind == VAR_ID)
            {
                s = get_symbol(stm->val.assignment.var->st,
                               stm->val.assignment.var->val.var_exp.id);
                if (s == NULL || s->value == -999) {
                    print_error("Const variables can not be reassigned a value"
                            , stm->line_no, 1);
                }
            }


            if (stm->val.assignment.exp->sym_type == NULL)
            { //Error should have been printed
                return;
            }

            //Only arrays and records can be set to null
            if (stm->val.assignment.exp->sym_type->kind == SYMBOL_NULL)
            {
                int kind = stm->val.assignment.var->sym_type->kind;
                if (kind == SYMBOL_INT || kind == SYMBOL_BOOL || kind == SYMBOL_FLOAT)
                {
                    print_error(ONLY_ARR_RECORD_NULL, stm->line_no, 1);
                }
            }
            else
            {
                if (stm->val.assignment.exp->sym_type->kind == SYMBOL_UNKNOWN)
                {

                    return;
                }


                if (!check_symbol_types(stm->val.assignment.var->sym_type,
                                        stm->val.assignment.exp->sym_type))
                {
                    if (stm->val.assignment.exp->sym_type->kind != SYMBOL_CLASS && print_errors) //TODO
                        print_error(VAR_EXP_NOT_SAME_TYPE, stm->line_no, 1);
                }
            }


            break;
        case STM_ASSIGN_LIST:
            check_variable(stm->val.assign_list.var);
            symbol_type *var_sym = stm->val.assign_list.var->sym_type;


            if (var_sym != NULL)
            {
                check_exp_list(stm->val.assign_list.el);
                if (var_sym->kind == SYMBOL_RECORD)
                {
                    expression_list *el = stm->val.assign_list.el;
                    var_decl_list *vdl = var_sym->val.vdl;

                    vector *exp_stack = vector_init();
                    vector *id_stack = vector_init();
                    int counter = 0;
                    while (el != NULL && vdl != NULL)
                    {
                        symbol_type *el_param = el->exp->sym_type;
                        symbol_type *pdl_param =
                                find_type(vdl->vt->type)->sym_type;
                        if (!check_symbol_types(el_param, pdl_param))
                        {
                            print_error("Types of record and "
                                        "assignment does not match",
                                        stm->line_no, 1);
                        }

                        vector_push(exp_stack, el->exp);
                        vector_push(id_stack, vdl->vt->id);
                        el = el->exp_list;
                        vdl = vdl->vdl;

                        counter++;

                    }
                    if (el != NULL || vdl != NULL)
                    {
                        print_error("All values of the record must be assigned"
                                , stm->line_no, 1);
                    }

                    el = stm->val.assign_list.el;
                    vdl = var_sym->val.vdl;

                    statement_list *sl_start =
                            make_sl_statement_list(NULL, NULL);
                    statement_list *sl = sl_start;
                    for (int i = 0; i < counter; i++)
                    {
                        char *id = vector_get_element(id_stack,
                                                      counter - i - 1);
                        expression *exp = vector_get_element(exp_stack,
                                                             counter - i - 1);
                        sl->stm = make_statement_assignment(
                                make_var_dot(stm->val.assign_list.var, id),
                                exp);

                        if (i != counter - 1)
                        {
                            sl->sl = make_sl_statement_list(NULL, NULL);
                            sl = sl->sl;
                        }

                        el = el->exp_list;
                        vdl = vdl->vdl;
                    }
                    sl->kind = SL_STM;

                    (*stm_p) = make_statement_sl(sl_start);

                }
                else if (var_sym->kind == SYMBOL_ARRAY)
                {
                    expression_list *el = stm->val.assign_list.el;

                    vector *exp_stack = vector_init();
                    s = get_symbol(stm->val.assign_list.var->st,
                                   stm->val.assign_list.var->val.var_exp.id);
                    int counter = 0;
                    while (el != NULL)
                    {
                        symbol_type *el_param = el->exp->sym_type;

                        if (el_param->kind !=
                            s->sym_type->val.type_p->sym_type->kind)
                        {
                            print_error("Types of record and assignment "
                                        "does not match",
                                        stm->line_no, 1);
                        }

                        vector_push(exp_stack, el->exp);
                        el = el->exp_list;

                        counter++;

                    }




                    statement_list *sl_start = make_sl_statement_list(NULL,
                                                                      NULL);
                    statement_list *sl = sl_start;
                    for (int i = 0; i < counter; i++)
                    {
                        expression *exp = vector_get_element(exp_stack,
                                                             counter - i - 1);
                        sl->stm = make_statement_assignment(
                                make_var_exp(stm->val.assign_list.var,
                                             make_exp_list_exp(make_exp_term(
                                                     make_term_int(counter - i - 1)))), exp);

                        if (i != counter - 1)
                        {
                            sl->sl = make_sl_statement_list(NULL, NULL);
                            sl = sl->sl;
                        }
                    }
                    sl->kind = SL_STM;

                    (*stm_p) = make_statement_sl(sl_start);
                }
            }
            else
            {
                // Err
            }
            break;
        case STM_IF:
            check_exp(stm->val.if_then.exp);
            check_statement(&stm->val.if_then.stm);

            //check if expression depend on templated class
            if(stm->val.if_then.exp->sym_type != NULL &&
               stm->val.if_then.exp->sym_type->kind == SYMBOL_TEMPLATED)
            {
                return;
            }


            if (stm->val.if_then.exp->sym_type == NULL ||
                stm->val.if_then.exp->sym_type->kind != SYMBOL_BOOL)
            {
                print_error(EXP_OF_STM_NOT_BOOL, stm->line_no, 1);
            }
            break;
        case STM_ELSE:
            check_exp(stm->val.if_then.exp);
            //check if expression depend on templated class
            if( stm->val.if_then.exp->sym_type != NULL &&
                stm->val.if_then.exp->sym_type->kind == SYMBOL_TEMPLATED)
            {
                return;
            }
            check_statement(&stm->val.if_else.stm1);
            check_statement(&stm->val.if_else.stm2);
            if (stm->val.if_then.exp->sym_type == NULL ||
                stm->val.if_then.exp->sym_type->kind != SYMBOL_BOOL)
            {
                print_error(EXP_OF_STM_NOT_BOOL, stm->line_no, 1);
            }
            break;
        case STM_WHILE:
            check_exp(stm->val.while_do.exp);
            //check if expression depend on templated class
            if(stm->val.while_do.exp->sym_type != NULL &&
               stm->val.if_then.exp->sym_type->kind == SYMBOL_TEMPLATED)
            {
                return;
            }
            check_statement(&stm->val.while_do.stm);
            if (stm->val.while_do.exp->sym_type == NULL ||
                stm->val.while_do.exp->sym_type->kind != SYMBOL_BOOL)
            {
                print_error(EXP_OF_STM_NOT_BOOL, stm->line_no, 1);
            }
            break;
        case STM_LIST:
            check_statement_list(stm->val.sl);
            break;
        case STM_BREAK:
        case STM_CONTINUE:
            break;
        case STM_IN:
            check_variable(stm->val.var);

            //check if var depends on a templated class
            if (stm->val.var->sym_type->kind == SYMBOL_TEMPLATED)
            {
                return;
            }
            if (stm->val.var->sym_type->kind != SYMBOL_INT &&
                stm->val.var->sym_type->kind != SYMBOL_BOOL)
            {
                print_error(IO_ONLY_INT_BOOL, stm->line_no, 1);
            }
            break;
        case STM_FREE:
            check_variable(stm->val.var);
            //check if var depends on a templated class
            if (stm->val.var->sym_type->kind == SYMBOL_TEMPLATED)
            {
                return;
            }
            if (stm->val.var->sym_type->kind != SYMBOL_RECORD &&
                stm->val.var->sym_type->kind != SYMBOL_ARRAY)
            {
                print_error("Only records and arrays can be freed",
                            stm->line_no, 1);
            }
            break;
        case STM_ASSIGN_VAL:
            check_variable(stm->val.assignment.var);
            check_exp(stm->val.assignment.exp);
            //check if var depends on a templated class
            if (stm->val.var->sym_type->kind == SYMBOL_TEMPLATED ||
                stm->val.assignment.exp->sym_type->kind == SYMBOL_TEMPLATED)
            {
                return;
            }
            break;
        case STM_SWITCH:
            check_exp(stm->val.switch_stm.exp);
            //check if expression depend on templated class
            if(stm->val.switch_stm.exp->sym_type->kind == SYMBOL_TEMPLATED)
            {
                return;
            }
            if (stm->val.switch_stm.exp->sym_type->kind != SYMBOL_INT)
            {
                print_error("Exp is not of type int", stm->line_no, 1);
            }
            check_switch(stm->val.switch_stm.s_stm);
            break;
    }
}

void check_decl_list(decl_list *dl)
{
    switch (dl->kind)
    {
        case DL_DECL_DL:
            check_decl_list(dl->dl);
            check_declaration(dl->decl);
            break;
        case DL_EMPTY:
            break;
    }
}

void check_declaration(declaration *d)
{
    switch (d->kind)
    {
        case DECL_TYPE:
            if (d->val.type.type->kind == TYPE_CLASS && d->val.type.type->val._class.temp->kind == TEMP_EMPTY) {
                check_type(d->val.type.type);
            }

            break;
        case DECL_FUNC:

            check_function(d->val.func);
            break;
        case DECL_VDL:
            check_var_decl_list(d->val.vdl);
            break;
        case DECL_CONST:
            check_type(d->val.con.type);
            check_exp(d->val.con.exp);

            if (!check_symbol_types(d->val.con.type->sym_type,
                                    d->val.con.exp->sym_type))
            {
                print_error("Type and expression does not have same type",
                            d->line_no, 1);
            }
            break;
    }
}

void check_variable(variable *var)
{
    symbol *s, *s2;
    expression_list *exp_list;
    symbol_table *st;
    variable *tmp_var;
    switch (var->kind)
    {
        case VAR_ID:
            s = get_symbol(var->st, var->val.var_exp.id);
            if (s == NULL)
            {
                char *msg = str_concat("Symbol \"", var->val.var_exp.id);
                msg = str_concat(msg, "\" was not found");
                print_error(msg, var->line_no, 1);
            }
            else
            {
                var->sym_type = s->sym_type;
                s->st = get_real_symbol_table(var->st, var->val.var_exp.id);
            }
            break;
        case VAR_EXP:
            check_variable(var->val.var_exp.var);
            check_exp_list(var->val.var_exp.exp);
            exp_list = var->val.var_exp.exp;
            while (exp_list != NULL)
            {
                if (exp_list->exp->sym_type == NULL ||
                    exp_list->exp->sym_type->kind != SYMBOL_INT)
                {
                    print_error(INDEX_NOT_INT, var->line_no, 1);
                }
                exp_list = exp_list->exp_list;
            }
            if (var->val.var_exp.var->sym_type == NULL ||
                var->val.var_exp.var->sym_type->kind != SYMBOL_ARRAY)
            {
                print_error(VAR_NOT_ARR, var->line_no, 1);
            }
            else
            {
                var->sym_type =
                        var->val.var_exp.var->sym_type->val.type_p->sym_type;
            }
            break;
        case VAR_RECORD:
            check_variable(var->val.var_exp.var);
            if (var->val.var_exp.var->sym_type == NULL ||
                (var->val.var_exp.var->sym_type->kind != SYMBOL_RECORD &&
                 var->val.var_exp.var->sym_type->kind != SYMBOL_CLASS))
            {
                print_error(VAR_NOT_RECORD, var->line_no, 1);
            }
            s = get_symbol(var->val.var_exp.var->sym_type->val.type_p->st,
                           var->val.var_exp.id);

            if (s != NULL)
            {
                var->sym_type = s->sym_type;
            }
            else
            {
                var->sym_type = make_symbol_type(SYMBOL_UNKNOWN);
            }
            break;
        case VAR_CLASS:
            check_variable(var->val.var_exp.var);

            tmp_var = var;
            st = var->st;

            if (tmp_var != NULL)
            {

                while (tmp_var->val.var_exp.var->val.var_exp.var != NULL)
                {
                    tmp_var = tmp_var->val.var_exp.var;
                }

                if (tmp_var->val.var_exp.var->sym_type == NULL)
                {   // Error -- error message should have been printed somewhere else.

                    return;
                }


                st = tmp_var->val.var_exp.var->sym_type->val._class.st;
            }


            s = get_symbol(var->st, var->val.var_exp.var->val.var_exp.id);
            if (s == NULL)
            {
                s = get_symbol(st, var->val.var_exp.var->val.var_exp.id);
                if (s == NULL)
                {
                    print_error("TODO 1", var->line_no, 1);
                    return;
                }
            }


            if (s->is_templated)
            {
                var->sym_type = make_symbol_type(SYMBOL_TEMPLATED);
                return;
            }

            if (var->val.var_exp.var->sym_type == NULL ||
                var->val.var_exp.var->sym_type->kind != SYMBOL_CLASS)
            {
                print_error("TODO 1", var->line_no, 1);
            }

            s = get_symbol(var->val.var_exp.var->sym_type->val._class.dl->st,
                           var->val.var_exp.id);


            if (s == NULL)
            {
                if (print_errors)
                {
                    char *error = str_concat("No variable named ", var->val.var_exp.id);
                    print_error(error, var->line_no, 1);

                }
                var->sym_type = make_symbol_type(SYMBOL_UNKNOWN);

            }
            else
            {
                if (s->access != AM_PUBLIC)
                {
                    print_error("Variable is not public", var->line_no, 1);
                }
                var->sym_type = s->sym_type;
            }
            break;
        case VAR_FUNC:
            check_variable(var->val.var_exp.var);
            check_act_list(var->val.var_exp.al);



            if (var->val.var_exp.var->val.var_exp.var != NULL) {
                st = var->val.var_exp.var->val.var_exp.var->sym_type->val._class.st;
            }
            else
            {
                st = var->st;
            }

            s = get_symbol(st, var->val.var_exp.var->val.var_exp.id);



            if (s == NULL)
            {

                /* if (s->access != AM_PUBLIC)
                 {
                     print_error("Function is not public", var->line_no, 1);
                 }*/
                print_error("TODO 2", var->line_no, 1);
                var->sym_type = make_symbol_type(SYMBOL_UNKNOWN);
            }
            else
            {

                if (s->is_templated)
                {
                    var->sym_type = make_symbol_type(SYMBOL_TEMPLATED);
                    return;
                }
                char *func_id = var->val.var_exp.id;
                if (func_id[0] != '_' && func_id[1] != '_')
                {
                    func_id =
                            str_concat(var->val.var_exp.id,
                                       get_types_exp_list(var->val.var_exp.al));
                }


                s2 = get_symbol(s->sym_type->val._class.dl->st, func_id);

                if (s2 == NULL)
                {
                    if (print_errors) {
                        char *error = str_concat("No function named \"", func_id);
                        error = str_concat(error, "\"");
                        print_error(error, var->line_no, 1);
                    }
                    var->sym_type = make_symbol_type(SYMBOL_UNKNOWN);

                }
                else
                {

                    if (s2->sym_type->kind != SYMBOL_FUNC)
                    {
                        char *error = str_concat("Symbol \"", func_id);
                        error = str_concat(error, "\" is not a function");
                        print_error(error, var->line_no, 1);
                        var->sym_type = make_symbol_type(SYMBOL_UNKNOWN);
                        break;
                    }
                    var->sym_type =
                            s2->sym_type->val.func.return_type->sym_type;
                    var->val.var_exp.func = s2->sym_type->val.func.func;
                }

            }


            break;
    }
}

char *get_type(type *t)
{
    char *res = str_concat("", "");
    symbol *s;
    switch (t->kind)
    {
        case TYPE_ID:
            s = get_symbol(t->st, t->id);
            if (s != NULL)
            {
                if (s->sym_type->kind == SYMBOL_CLASS)
                {
                    res = str_concat(res, ".");
                    res = str_concat(res, t->id);
                    break;
                }
            }
            res = str_concat(res, ".");
            res = str_concat(res, t->id);
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
            break;
    }
    return res;
}

char *gen_prefix(char *prefix, temp_args_decl_list *tadl)
{
    while (tadl->kind == TADL_TYPE_TADL)
    {
        prefix = str_concat(prefix, get_type(tadl->t));
        tadl = tadl->tadl;
    }
    prefix = str_concat(prefix, get_type(tadl->t));
    prefix = str_concat(prefix, ".");

    return prefix;
}

int check_term(term *t)
{
    symbol *s;
    switch (t->kind)
    {
        case TERM_VAR:
            check_variable(t->val.var);
            t->sym_type = t->val.var->sym_type;
            break;
        case TERM_AL:

            check_act_list(t->val.term_act_list.al);
            char *func_id;

            if (t->val.term_act_list.temp_args->kind == TEMP_ARG_EMPTY)
            {
                if (t->val.term_act_list.was_templated)
                {
                    func_id = str_concat(t->val.term_act_list.id, "");
                    //func_id = str_concat(t->val.term_act_list.id, get_types_exp_list(t->val.term_act_list.al));
                }
                else
                {
                    func_id =
                            str_concat(t->val.term_act_list.id,
                                       get_types_exp_list(t->val.term_act_list.al));
                }
            }
            else
            {

                func_id = t->val.term_act_list.id;

                if (t->val.term_act_list.temp_args->tal->kind != TAL_EMPTY)
                {

                    char *tmp = str_concat("", t->val.term_act_list.id);
                    s = get_symbol(t->st, tmp);
                    if (s == NULL)
                    {
                        if (print_errors)
                        {
                            print_error("Function call could not be matched any declared function", t->line_no, 1);
                        }
                        else
                        {
                            t->sym_type = make_symbol_type(SYMBOL_UNKNOWN);
                        }
                        return -1;
                    }
                    //char *start = str_concat("@", "");
                    char *start = str_concat("", "");
                    char *prefix = gen_prefix(start, t->val.term_act_list.temp_args->tal->tadl);
                    prefix = str_concat(prefix, t->val.term_act_list.id);
                    prefix = str_concat(prefix, get_types(s->sym_type->val.func.func->head->pdl));
                    prefix = str_concat("", prefix);
                    t->val.term_act_list.temp_id = prefix;

                }
                else
                {
                    t->val.term_act_list.temp_id = str_concat(".", func_id);
                }
            }



            char *old_func_name = t->val.term_act_list.id;

            t->val.term_act_list.id = func_id;
            s = get_symbol(t->st, func_id);


            if (t->val.term_act_list.temp_args->kind != TEMP_ARG_EMPTY)
            {
                s = NULL;
            }

            if (s == NULL || s->sym_type->kind != SYMBOL_FUNC)
            { // Func name is not in symbol table
                bool is_in_vector = false;
                for (int i = 0; i < (int) vector_size(func_decl_list); i++)
                { // Check if func is templated, if so it does not exists in symbol table before the real function is generated
                    decl_list *dl = vector_get_element(func_decl_list, i);
                    if (strcmp(old_func_name, dl->decl->val.func->head->id) == 0) {
                        is_in_vector = true;
                        t->val.term_act_list.oldId = old_func_name;
                        vector_push(temp_func_calls, t);
                        break;
                    }
                }

                if (!is_in_vector) {
                    if (print_errors)
                    {
                        print_error("Function call could not be matched any declared function", t->line_no, 1);
                    }
                    t->sym_type = make_symbol_type(SYMBOL_UNKNOWN);
                    return -1;
                }
            }
            else
            {

                t->val.term_act_list.func = s->sym_type->val.func.func;
                act_list *al = t->val.term_act_list.al;

                if (al->kind == AL_EMPTY)
                {

                    if (s->sym_type->val.func.func->head->pdl->kind ==
                        PDL_EMPTY)
                    {
                        t->sym_type =
                                s->sym_type->val.func.return_type->sym_type;
                        return 0;
                    }
                    else
                    {
                        if (print_errors)
                        {
                            print_error(FEW_ARG_FUNC_CALL, t->line_no, 1);
                        }
                        t->sym_type = make_symbol_type(SYMBOL_UNKNOWN);
                        return 1;
                    }
                }
                else
                {

                    if (s->sym_type->val.func.func->head->pdl->kind ==
                        PDL_EMPTY)
                    {
                        if (print_errors)
                        {
                            print_error(MANY_ARG_FUNC_CALL, t->line_no, 1);
                        }
                        else
                        {
                            t->sym_type = make_symbol_type(SYMBOL_UNKNOWN);
                        }
                        return 1;
                    }


                    expression_list *el = al->exp_list;
                    var_decl_list *vdl =
                            s->sym_type->val.func.func->head->pdl->vdl;


                    while (el != NULL && vdl != NULL)
                    {
                        symbol_type *el_param = el->exp->sym_type;
                        symbol_type *pdl_param =
                                find_type(vdl->vt->type)->sym_type;
                        if (!check_symbol_types(el_param, pdl_param))
                        {
                            print_error(FUNC_CALL_MISMATCH_TYPE,
                                        t->line_no, 1);
                        }
                        el = el->exp_list;
                        vdl = vdl->vdl;
                    }

                    if (el != NULL || vdl != NULL)
                    {
                        print_error(MISMATCH_ARG_NUM, t->line_no, 1);
                    }

                    t->sym_type = s->sym_type->val.func.return_type->sym_type;

                }

            }
            break;
        case TERM_PARENTHESES:
            check_exp(t->val.exp);
            t->sym_type = t->val.exp->sym_type;
            break;
        case TERM_NOT:
            check_term(t->val.not_term);
            if (t->val.not_term->sym_type->kind == SYMBOL_BOOL)
            {
                t->sym_type = t->val.not_term->sym_type;
            }
            else
            {
                print_error(ONLY_NOT_BOOL, t->line_no, 1);
            }
            break;
        case TERM_ABS:
            check_exp(t->val.exp);
            // Abs value of int or size of array
            if (t->val.exp->sym_type->kind == SYMBOL_ARRAY ||
                t->val.exp->sym_type->kind == SYMBOL_INT)
            {
                t->sym_type = make_symbol_type(SYMBOL_INT);
            }
            else
            {
                print_error(ONLY_PIPE_INT_ARR, t->line_no, 1);
            }
            break;
        case TERM_INT:
            t->sym_type = make_symbol_type(SYMBOL_INT);
            break;
        case TERM_FLOAT:
            t->sym_type = make_symbol_type(SYMBOL_FLOAT);
            break;
        case TERM_TRUE:
        case TERM_FALSE:
            t->sym_type = make_symbol_type(SYMBOL_BOOL);
            break;
        case TERM_NULL:
            t->sym_type = make_symbol_type(SYMBOL_NULL);
            break;
        case TERM_STRING:
            t->sym_type = make_symbol_type(SYMBOL_STRING);
            break;
    }
    return 0;
}

void check_act_list(act_list *al)
{
    switch (al->kind)
    {
        case AL_EXP_LIST:
            check_exp_list(al->exp_list);
            break;
        case AL_EMPTY:
            break;
    }
}

void check_exp_list(expression_list *exp_list)
{

    if (exp_list == NULL)
    {
        return;
    }
    check_exp(exp_list->exp);

    if (exp_list->exp_list != NULL)
    {
        check_exp_list(exp_list->exp_list);
    }
}

bool check_exp_is_templated(expression *exp)
{
    if (exp->sym_type->kind == SYMBOL_CLASS)
    {
        symbol *s = get_symbol(exp->st, exp->val.term->val.var->val.var_exp.id);
        if (s != NULL && s->is_templated)
        {
            return true;
        }
    }
    return false;
}

void check_exp(expression *exp) {
    exp->sym_type = make_symbol_type(SYMBOL_UNKNOWN);
    symbol_type *l, *r;


    switch (exp->kind)
    {
        case EXP_TERM:

            check_term(exp->val.term);
            exp->sym_type = exp->val.term->sym_type;
            break;
        case EXP_TIMES:
        case EXP_DIV:
        case EXP_PLUS:
        case EXP_MINUS:
            check_exp(exp->val.op.left);
            check_exp(exp->val.op.right);



            l = exp->val.op.left->sym_type;
            r = exp->val.op.right->sym_type;


            if ((l != NULL && l->kind == SYMBOL_TEMPLATED) ||
                (r != NULL && r->kind == SYMBOL_TEMPLATED))
            {
                exp->sym_type->kind = SYMBOL_TEMPLATED;
                return;
            }

            if (l != NULL && r != NULL)
            {
                if (check_exp_is_templated(exp->val.op.left)) { exp->sym_type->kind = SYMBOL_TEMPLATED; return; }
                if (check_exp_is_templated(exp->val.op.right)) { exp->sym_type->kind = SYMBOL_TEMPLATED; return; }

                if ((l->kind == SYMBOL_BOOL || l->kind == SYMBOL_INT) &&
                    (r->kind == SYMBOL_INT || r->kind == SYMBOL_BOOL))
                {
                    exp->sym_type = make_symbol_type(SYMBOL_INT);
                }
                else if ((l->kind == SYMBOL_FLOAT || l->kind == SYMBOL_INT) &&
                         (r->kind == SYMBOL_INT || r->kind == SYMBOL_FLOAT))
                {
                    exp->sym_type = make_symbol_type(SYMBOL_FLOAT);
                }
                else if ((l->kind == SYMBOL_FLOAT || l->kind == SYMBOL_BOOL) &&
                         (r->kind == SYMBOL_BOOL || r->kind == SYMBOL_FLOAT))
                {
                    exp->sym_type = make_symbol_type(SYMBOL_FLOAT);
                }
                else if (l->kind == SYMBOL_CLASS || l->kind == SYMBOL_CLASS)
                {
                    exp->sym_type = make_symbol_type(SYMBOL_CLASS);
                }
            }
            else
            {
                print_error(EXP_NOT_VALID, exp->line_no, 1);
            }
            break;
        case EXP_EQ:
        case EXP_NEQ:
            check_exp(exp->val.op.left);
            check_exp(exp->val.op.right);



            // Check for both ints, bools, arrays, records
            l = exp->val.op.left->sym_type;
            r = exp->val.op.right->sym_type;

            if ((l != NULL && l->kind == SYMBOL_TEMPLATED) ||
                (r != NULL && r->kind == SYMBOL_TEMPLATED))
            {
                exp->sym_type->kind = SYMBOL_TEMPLATED;
                return;
            }
            if (l != NULL && r != NULL)
            {
                if (check_exp_is_templated(exp->val.op.left)) { exp->sym_type->kind = SYMBOL_TEMPLATED; return; }
                if (check_exp_is_templated(exp->val.op.right)) { exp->sym_type->kind = SYMBOL_TEMPLATED; return; }

                if (l->kind == SYMBOL_INT && r->kind == SYMBOL_INT)
                {
                    exp->sym_type = make_symbol_type(SYMBOL_BOOL);
                }
                else if (l->kind == SYMBOL_BOOL && r->kind == SYMBOL_BOOL)
                {
                    exp->sym_type = make_symbol_type(SYMBOL_BOOL);
                }
                else if (l->kind == SYMBOL_FLOAT && r->kind == SYMBOL_FLOAT)
                {
                    exp->sym_type = make_symbol_type(SYMBOL_BOOL);
                }
                else if ((l->kind == SYMBOL_ARRAY && r->kind == SYMBOL_NULL) ||
                         (l->kind == SYMBOL_NULL && r->kind == SYMBOL_ARRAY) ||
                         (l->kind == SYMBOL_ARRAY && r->kind == SYMBOL_ARRAY))
                {
                    exp->sym_type = make_symbol_type(SYMBOL_BOOL);
                }
                else if ((l->kind == SYMBOL_RECORD && r->kind == SYMBOL_NULL)||
                         (l->kind == SYMBOL_NULL && r->kind == SYMBOL_RECORD) ||
                         (l->kind == SYMBOL_RECORD && r->kind == SYMBOL_RECORD))
                {
                    exp->sym_type = make_symbol_type(SYMBOL_BOOL);
                }
                else if (l->kind == SYMBOL_CLASS && r->kind == SYMBOL_CLASS)
                {
                    exp->sym_type = make_symbol_type(SYMBOL_BOOL);
                }
                else if (l->kind == SYMBOL_CLASS && r->kind == SYMBOL_NULL)
                {
                    exp->sym_type = make_symbol_type(SYMBOL_BOOL);
                }
                else if (l->kind == SYMBOL_NULL && r->kind == SYMBOL_CLASS)
                {
                    exp->sym_type = make_symbol_type(SYMBOL_BOOL);
                }
            }
            break;
        case EXP_GREATER:
        case EXP_LESS:
        case EXP_GEQ:
        case EXP_LEQ:
            check_exp(exp->val.op.left);
            check_exp(exp->val.op.right);

            if (exp->val.op.left->sym_type->kind == SYMBOL_TEMPLATED ||
                exp->val.op.right->sym_type->kind == SYMBOL_TEMPLATED)
            {
                exp->sym_type->kind = SYMBOL_TEMPLATED;
                return;
            }

            if (exp->val.op.left->sym_type != NULL && exp->val.op.right->sym_type != NULL)
            {
                if (check_exp_is_templated(exp->val.op.left)) { exp->sym_type->kind = SYMBOL_TEMPLATED; return; }
                if (check_exp_is_templated(exp->val.op.right)) { exp->sym_type->kind = SYMBOL_TEMPLATED; return; }
            }


            if (exp->val.op.left->sym_type != NULL &&
                exp->val.op.right->sym_type != NULL &&
                ((exp->val.op.left->sym_type->kind == SYMBOL_INT &&
                  exp->val.op.right->sym_type->kind == SYMBOL_INT) ||
                 (exp->val.op.left->sym_type->kind == SYMBOL_FLOAT &&
                  exp->val.op.right->sym_type->kind == SYMBOL_FLOAT)||
                 (exp->val.op.left->sym_type->kind == SYMBOL_CLASS &&
                  exp->val.op.right->sym_type->kind == SYMBOL_CLASS)))
            {
                exp->sym_type = make_symbol_type(SYMBOL_BOOL);
            }
            else
            {
                if (print_errors)
                {
                    print_error(EXPS_NOT_INT, exp->line_no, 1);
                }
                else
                {
                    exp->sym_type = make_symbol_type(SYMBOL_UNKNOWN);
                }
            }
            break;
        case EXP_AND:
        case EXP_OR:
            check_exp(exp->val.op.left);
            check_exp(exp->val.op.right);

            if (exp->val.op.left->sym_type != NULL && exp->val.op.right->sym_type != NULL)
            {
                if (check_exp_is_templated(exp->val.op.left)) { exp->sym_type->kind = SYMBOL_TEMPLATED; return; }
                if (check_exp_is_templated(exp->val.op.right)) { exp->sym_type->kind = SYMBOL_TEMPLATED; return; }
            }

            if (exp->val.op.left->sym_type != NULL &&
                exp->val.op.right->sym_type != NULL &&
                exp->val.op.left->sym_type->kind == SYMBOL_BOOL &&
                exp->val.op.right->sym_type->kind == SYMBOL_BOOL)
            {
                exp->sym_type = make_symbol_type(SYMBOL_BOOL);
            }
            else
            {
                print_error(EXP_NOT_BOOL, exp->line_no, 1);
            }
            break;
    }
}

void check_switch(switch_stm *s_stm)
{
    if (s_stm == NULL)
    {
        return;
    }
    check_switch_case(s_stm->switch_case);
    check_switch(s_stm->switch_list);
}

void check_switch_case(switch_case *s_case)
{
    check_exp(s_case->exp);
    if (s_case->exp->sym_type->kind != SYMBOL_INT)
    {
        print_error("Exp is not of type int", s_case->line_no, 1);
    }
    check_statement(&s_case->stm);
}

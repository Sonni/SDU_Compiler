#include "gen_class.h"
#include "../parser/pretty.h"
#include "../../utils/ast_copy.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../utils/string_util.h"
#include "inner_class_rename.h"

extern vector *temp_class_type;
extern vector *temp_class_decl;
vector *generated_classes;
extern char *suffix;

void insert_new_class(declaration *d, decl_list *dl)
{
    dl->kind = DL_DECL_DL;
    dl->decl = make_declaration_type(make_access_modifier(AM_EMPTY), d->val.type.id, d->val.type.type);
    dl->dl = make_dl_empty();
}

vector *gen_class(body *b)
{
    generated_classes = vector_init();
    suffix = malloc(sizeof(char) * 10);
    sprintf(suffix, "");
    for (int i = 0; i < (int) vector_size(temp_class_type); i++)
    {
        type *t = vector_get_element(temp_class_type, i);

        // Check for an already generated matching class
        bool found_matching_class = false;

        for (int j = 0; j < (int) vector_size(generated_classes); j++)
        {
            declaration *d = vector_get_element(generated_classes, j);
            char *new_name = gen_final_class_name(t->id, t);
            if (strcmp(new_name, d->val.type.id) == 0)
            {
                t->id = new_name;
                t->ta->kind = TEMP_ARG_EMPTY;

                found_matching_class = true;
                break;
            }
        }
        if (found_matching_class)
        {
            continue;
        }

        bool found_match = false;

        // No already generated class did match, try to generate a new
        for (int j = 0; j < (int) vector_size(temp_class_decl); j++)
        {
            declaration *d = vector_get_element(temp_class_decl, j);
            bool same_num_args = d->val.type.type->val._class.temp->num_para == t->ta->num_args;

            if (strcmp(t->id, d->val.type.id) == 0 && same_num_args)
            {
                declaration *new_class = gen_new_class(d, t);
                vector_push(generated_classes, new_class);
                found_match = true;

                break;
            }
        }
        if (!found_match)
        {
            print_error("Found no matching class", t->line_no, 1);
            return NULL;
        }
    }


    if ((int) vector_size(temp_class_decl) != 0)
    {

        decl_list *dl = b->dl;

        while (dl->kind != DL_EMPTY) {
            dl = dl->dl;
        }


        for (int i = 0; i < (int) vector_size(generated_classes); i++)
        {
            declaration *d = vector_get_element(generated_classes, i);
            insert_new_class(d, dl);
            dl = dl->dl;
        }
    }

    return generated_classes;
}

declaration *gen_new_class(declaration *d, type *t)
{
    declaration *new_class = ast_copy_declaration(d, d->st);

    gen_class_tl(d->val.type.type->val._class.temp->tl, new_class, t->ta->tal->tadl);
    new_class->val.type.type->val._class.temp->kind = TEMP_EMPTY;

    get_symbol(t->st, t->id)->is_templated = false;

    char *new_name = gen_final_class_name(d->val.type.original_name, t);
    t->id = new_name;
    new_class->val.type.id = new_name;

    symbol_type *sym_t = make_symbol_type(SYMBOL_CLASS);
    put_symbol(t->st, new_name, sym_t, 0);

    t->ta->kind = TEMP_ARG_EMPTY;




    inner_class_rename_declaration(new_class, d->val.type.id, new_name);

    return new_class;
}

void gen_class_function(function *f, char *type_name, type *t1)
{
    gen_class_head(f->head, type_name, t1);
    gen_class_body(f->body, type_name, t1);
}

void gen_class_body(body *b, char *type_name, type *t1)
{
    gen_class_decl_list(b->dl, type_name, t1);
    //gen_class_statement_list(b->sl);
}


void gen_class_head(head *h, char *type_name, type *t1)
{
    gen_class_par_decl_list(h->pdl, type_name, t1);
    gen_class_type(h->type, type_name, t1);
}


void gen_class_type(type *t, char *type_name, type *t1)
{
    if (t->kind == 0 && strcmp(t->id, type_name) == 0)
    {
        t->kind = t1->kind;
        t->id = t1->id;
    }

    if (t->kind == TYPE_CLASS) {
        gen_class_decl_list(t->val._class.dl, type_name, t1);
    }
    else if (t->kind == TYPE_ARRAY)
    {
        gen_class_type(t->val.type, type_name, t1);
    }
    else if (t->kind == TYPE_RECORD)
    {

        gen_class_var_decl_list(t->val.vdl, type_name, t1);
    }
}

void gen_class_par_decl_list(par_decl_list *pdl, char *type_name, type *t1)
{
    switch (pdl->kind)
    {
        case PDL_VDL:
            gen_class_var_decl_list(pdl->vdl, type_name, t1);
            break;
        case PDL_EMPTY:
            break;
    }
}

void gen_class_var_decl_list(var_decl_list *vdl, char *type_name, type *t1)
{
    switch (vdl->kind)
    {
        case VDL_VT_VDL:
            gen_class_var_type(vdl->vt, type_name, t1);
            gen_class_var_decl_list(vdl->vdl, type_name, t1);
            break;
        case VDL_VT:
            gen_class_var_type(vdl->vt, type_name, t1);
            break;
    }
}

void gen_class_var_type(var_type *vt, char *type_name, type *t1)
{
    gen_class_type(vt->type, type_name, t1);
}

void gen_class_statement_list(statement_list *sl)
{
    switch (sl->kind)
    {
        case SL_STM:
            gen_class_statement(sl->stm);
            break;
        case SL_STM_SL:
            gen_class_statement(sl->stm);
            gen_class_statement_list(sl->sl);
            break;
    }
}

void gen_class_statement(statement *stm)
{
    switch (stm->kind)
    {
        case STM_RETURN:
            gen_class_exp(stm->val.exp);
            break;
        case STM_WRITE:
            gen_class_exp(stm->val.exp);
            break;
        case STM_ALLOCATE:
            gen_class_variable(stm->val.var);
            break;
        case STM_ALLOCATE_OF_LENGTH:
            gen_class_variable(stm->val.assignment.var);
            gen_class_exp(stm->val.assignment.exp);
            break;
        case STM_ASSIGNMENT:
            gen_class_variable(stm->val.assignment.var);
            gen_class_exp(stm->val.assignment.exp);
            break;
        case STM_ASSIGN_LIST:
            gen_class_variable(stm->val.assign_list.var);
            gen_class_exp_list(stm->val.assign_list.el);
            break;
        case STM_IF:
            gen_class_exp(stm->val.if_then.exp);
            gen_class_statement(stm->val.if_then.stm);
            break;
        case STM_ELSE:
            gen_class_exp(stm->val.if_then.exp);
            gen_class_statement(stm->val.if_else.stm1);
            gen_class_statement(stm->val.if_else.stm2);
            break;
        case STM_WHILE:
            gen_class_exp(stm->val.while_do.exp);
            gen_class_statement(stm->val.while_do.stm);
            break;
        case STM_LIST:
            gen_class_statement_list(stm->val.sl);
            break;
        case STM_BREAK:
        case STM_CONTINUE:
            break;
        case STM_IN:
            gen_class_variable(stm->val.var);
            break;
        case STM_FREE:
            gen_class_variable(stm->val.var);
            break;
        case STM_ASSIGN_VAL:
            gen_class_variable(stm->val.assignment.var);
            gen_class_exp(stm->val.assignment.exp);
            break;
        case STM_SWITCH:
            gen_class_exp(stm->val.switch_stm.exp);
            gen_class_switch(stm->val.switch_stm.s_stm);
            break;
    }
}

void gen_class_decl_list(decl_list *dl, char *type_name, type *t1)
{
    switch (dl->kind)
    {
        case DL_DECL_DL:
            gen_class_decl_list(dl->dl, type_name, t1);
            gen_class_declaration(dl->decl, type_name, t1);
            break;
        case DL_EMPTY:
            break;
    }
}

void gen_class_declaration(declaration *d, char *type_name, type *t1)
{
    switch (d->kind)
    {
        case DECL_TYPE:
            gen_class_type(d->val.type.type, type_name, t1);
            break;
        case DECL_FUNC:
            gen_class_function(d->val.func, type_name, t1);
            break;
        case DECL_VDL:
            gen_class_var_decl_list(d->val.vdl, type_name, t1);
            break;
        case DECL_CONST:
            gen_class_type(d->val.con.type, type_name, t1);
            gen_class_exp(d->val.con.exp);
            break;
    }
}

void gen_class_variable(variable *var)
{
    switch (var->kind)
    {
        case VAR_ID:
            break;
        case VAR_EXP:
            gen_class_variable(var->val.var_exp.var);
            gen_class_exp_list(var->val.var_exp.exp);
            break;
        case VAR_RECORD:
            gen_class_variable(var->val.var_exp.var);
            break;
        case VAR_CLASS:
            gen_class_variable(var->val.var_exp.var);
            break;
        case VAR_FUNC:
            var->val.var_exp.id = var->val.var_exp.original_name;
            gen_class_variable(var->val.var_exp.var);
            gen_class_act_list(var->val.var_exp.al);
            break;
    }
}

void gen_class_term(term *t)
{
    switch (t->kind)
    {
        case TERM_VAR:
            gen_class_variable(t->val.var);
            break;
        case TERM_AL:
            t->val.term_act_list.id = t->val.term_act_list.original_name;
            gen_class_act_list(t->val.term_act_list.al);
            break;
        case TERM_PARENTHESES:
            gen_class_exp(t->val.exp);
            break;
        case TERM_NOT:
            gen_class_term(t->val.not_term);
            break;
        case TERM_ABS:
            gen_class_exp(t->val.exp);
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

void gen_class_act_list(act_list *al)
{
    switch (al->kind)
    {
        case AL_EXP_LIST:
            gen_class_exp_list(al->exp_list);
            break;
        case AL_EMPTY:
            break;
    }

}

void gen_class_exp_list(expression_list *exp_list)
{
    switch (exp_list->kind)
    {
        case EXP_LIST_EXP:
            gen_class_exp(exp_list->exp);
            break;
        case EXP_LIST_EXP_LIST:
            gen_class_exp_list(exp_list->exp_list);
            gen_class_exp(exp_list->exp);
            break;
    }
}

void gen_class_exp(expression *exp)
{
    switch (exp->kind)
    {
        case EXP_TERM:
            gen_class_term(exp->val.term);
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
            gen_class_exp(exp->val.op.left);
            gen_class_exp(exp->val.op.right);
            break;
    }
}

void gen_class_switch(switch_stm *s_stm)
{
    if (s_stm == NULL)
    {
        return;
    }
    gen_class_switch_case(s_stm->switch_case);
    gen_class_switch(s_stm->switch_list);
}

void gen_class_switch_case(switch_case *s_case)
{
    gen_class_exp(s_case->exp);
    gen_class_statement(s_case->stm);
}


void gen_class_tl(temp_list *tl, declaration *d, temp_args_decl_list *tadl)
{
    switch (tl->kind)
    {
        case TL_TDL:
            gen_class_tdl(tl->tdl, d, tadl);
            break;
        case TL_EMPTY:
            break;
    }
}

void gen_class_tdl(temp_decl_list *tdl, declaration *d, temp_args_decl_list *tadl)
{
    switch (tdl->kind)
    {
        case TDL_ID_TDL:
            gen_class_tdl(tdl->tdl, d, tadl->tadl);
            gen_class_decl_list(d->val.type.type->val._class.dl, tdl->id, tadl->t);
            break;
        case TDL_ID:
            gen_class_decl_list(d->val.type.type->val._class.dl, tdl->id, tadl->t);
            break;
    }
}

char *gen_final_class_name(char *name, type *t)
{
    char *res = str_concat("", name);
    temp_args_decl_list *tadl = t->ta->tal->tadl;

    while (tadl != NULL)
    {

        switch (tadl->t->kind)
        {
            case TYPE_ID:
                res = str_concat(res, ".");
                res = str_concat(res, tadl->t->id);
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
            case TYPE_STRING:
                break;
            case TYPE_ARRAY:
                break;
            case TYPE_RECORD:
                break;
            case TYPE_CLASS:
                break;
        }

            tadl = tadl->tadl;
    }
    return res;
}

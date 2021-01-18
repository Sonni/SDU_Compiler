#include <stdio.h>
#include "pretty.h"
#include "../../utils/definitions.h"


const unsigned int INDENTATION = 4;
unsigned int space = 0;

void pretty(body *b)
{
    pretty_body(b);
    printf("\n");
}

void pretty_body(body *b)
{
    pretty_decl_list(b->dl);
    pretty_statement_list(b->sl);
}

void pretty_function(function *f)
{
    pretty_template(f->template);
    pretty_head(f->head);
    space += INDENTATION;
    pretty_body(f->body);
    space -= INDENTATION;
    pretty_tail(f->tail);
}

void pretty_head(head *h)
{
    printf("func %s(", h->id);
    pretty_par_decl_list(h->pdl);
    printf("): ");
    pretty_type(h->type);
}

void pretty_tail(tail *t)
{
    print_indentation();
    printf("end %s\n", t->id);
}

void pretty_type(type *t)
{
    switch (t->kind)
    {
        case TYPE_ID:
            printf("%s", t->id);
            pretty_temp_args(t->ta);
            pretty_sym_type(t->sym_type);
            break;
        case TYPE_INT:
            printf("int");
            break;
        case TYPE_FLOAT:
            printf("float");
            break;
        case TYPE_BOOL:
            printf("bool");
            break;
        case TYPE_STRING:
            printf("string");
            break;
        case TYPE_ARRAY:
            printf("array of ");
            pretty_type(t->val.type);
            break;
        case TYPE_RECORD:
            printf("record of {");
            pretty_var_decl_list(t->val.vdl);
            printf("}");
            break;
        case TYPE_CLASS:
            pretty_template(t->val._class.temp);
            printf(" class {");
            space += INDENTATION;
            pretty_decl_list(t->val._class.dl);
            space -= INDENTATION;

            printf("\n}");
            break;
    }
}

void pretty_par_decl_list(par_decl_list *pdl)
{
    switch (pdl->kind)
    {
        case PDL_VDL:
            pretty_var_decl_list(pdl->vdl);
            break;
        case PDL_EMPTY:
            break;
    }
}

void pretty_var_decl_list(var_decl_list *vdl)
{
    switch (vdl->kind)
    {
        case VDL_VT_VDL:
            pretty_var_type(vdl->vt);
            printf(", ");
            pretty_var_decl_list(vdl->vdl);
            break;
        case VDL_VT:
            pretty_var_type(vdl->vt);
            break;
    }
}

void pretty_var_type(var_type *vt)
{
    printf("%s: ", vt->id);
    pretty_type(vt->type);
}

void pretty_statement_list(statement_list *sl)
{
    if (sl == NULL)
        return;

    pretty_statement_list(sl->sl);
    pretty_statement(sl->stm);
}

void pretty_statement(statement *stm)
{
    if (stm->kind != STM_LIST)
    {
        print_indentation();
    }
    switch (stm->kind)
    {
        case STM_RETURN:
            printf("return ");
            pretty_exp(stm->val.exp);
            printf(";");
            break;
        case STM_WRITE:
            printf("write ");
            pretty_exp(stm->val.exp);
            printf(";");
            break;
        case STM_ALLOCATE:
            printf("allocate ");
            pretty_variable(stm->val.var);
            printf(";");
            break;
        case STM_ALLOCATE_OF_LENGTH:
            printf("allocate ");
            pretty_variable(stm->val.assignment.var);
            printf(" of length ");
            pretty_exp(stm->val.assignment.exp);
            printf(";");
            break;
        case STM_ASSIGNMENT:
            pretty_variable(stm->val.assignment.var);
            printf(" = ");
            pretty_exp(stm->val.assignment.exp);
            printf(";");
            break;
        case STM_ASSIGN_LIST:
            pretty_variable(stm->val.assign_list.var);
            printf(" = {");
            pretty_exp_list(stm->val.assign_list.el);
            printf("};");
            break;
        case STM_IF:
            printf("if ");
            pretty_exp(stm->val.if_then.exp);
            printf(" then");
            if (stm->val.if_then.stm->kind != STM_LIST)
            {
                space += INDENTATION;
                pretty_statement(stm->val.if_then.stm);
            }
            else
            {
                pretty_statement(stm->val.if_then.stm);
                space += INDENTATION;
            }
            space -= INDENTATION;
            break;
        case STM_ELSE:
            printf("if ");
            pretty_exp(stm->val.if_then.exp);
            printf(" then");
            if (stm->val.if_else.stm1->kind != STM_LIST)
            {
                space += INDENTATION;
                pretty_statement(stm->val.if_else.stm1);
            }
            else
            {
                pretty_statement(stm->val.if_else.stm1);
                space += INDENTATION;
            }
            space -= INDENTATION;
            print_indentation();
            printf("else");
            if (stm->val.if_else.stm2->kind != STM_LIST)
            {
                space += INDENTATION;
                pretty_statement(stm->val.if_else.stm2);
            }
            else
            {
                pretty_statement(stm->val.if_else.stm2);
                space += INDENTATION;
            }
            space -= INDENTATION;
            break;
        case STM_WHILE:
            if (stm->val.while_do.kind == 0)
            {
                printf("while ");
                pretty_exp(stm->val.while_do.exp);
                printf(" do");

                if (stm->val.while_do.stm->kind == STM_LIST)
                {
                    pretty_statement(stm->val.while_do.stm);
                }
                else
                {
                    space += INDENTATION;
                    pretty_statement(stm->val.while_do.stm);
                    space -= INDENTATION;
                }
            }
            else
            {
                printf("do");

                if (stm->val.while_do.stm->kind == STM_LIST)
                {
                    pretty_statement(stm->val.while_do.stm);
                }
                else
                {
                    space += INDENTATION;
                    pretty_statement(stm->val.while_do.stm);
                    space -= INDENTATION;
                    print_indentation();
                }

                printf("while ");
                pretty_exp(stm->val.while_do.exp);
                printf(";");
            }

            break;
        case STM_LIST:
            print_indentation();
            printf("{");
            space += INDENTATION;
            pretty_statement_list(stm->val.sl);
            space -= INDENTATION;
            print_indentation();
            printf("}\n");
            break;
        case STM_BREAK:
            printf("break;");
            break;
        case STM_CONTINUE:
            printf("continue;");
            break;
        case STM_IN:
            pretty_variable(stm->val.var);
            printf(" << in");
            break;
        case STM_FREE:
            printf("free(");
            pretty_variable(stm->val.var);
            printf(");");
            break;
        case STM_ASSIGN_VAL:
            pretty_variable(stm->val.assignment.var);
            printf("<-");
            pretty_exp(stm->val.assignment.exp);
            printf(";");
            break;
        case STM_SWITCH:
            printf("switch (");
            pretty_exp(stm->val.switch_stm.exp);
            printf(") {");
            print_indentation();
            pretty_switch(stm->val.switch_stm.s_stm);
            printf("}");
            break;
    }
}

void pretty_decl_list(decl_list *dl)
{
    switch (dl->kind)
    {
        case DL_DECL_DL:
            pretty_decl_list(dl->dl);
            pretty_declaration(dl->decl);
            break;
        case DL_EMPTY:
            break;
    }
}

void pretty_declaration(declaration *d)
{
    print_indentation();
    switch (d->kind)
    {
        case DECL_TYPE:
            printf("type %s = ", d->val.type.id);
            pretty_type(d->val.type.type);
            printf(";");
            break;
        case DECL_FUNC:
            pretty_function(d->val.func);
            break;
        case DECL_VDL:
            printf("var ");
            pretty_var_decl_list(d->val.vdl);
            printf(";");
            break;
        case DECL_CONST:
            printf("const ");
            pretty_type(d->val.con.type);
            printf(" %s = ", d->val.con.id);
            pretty_exp(d->val.con.exp);
            printf("\n");
            break;
    }
}

void pretty_variable(variable *var)
{
    switch (var->kind)
    {
        case VAR_ID:
            printf("%s", var->val.var_exp.id);
            break;
        case VAR_EXP:
            pretty_variable(var->val.var_exp.var);
            printf("[");
            pretty_exp_list(var->val.var_exp.exp);
            printf("]");
            break;
        case VAR_RECORD:
            pretty_variable(var->val.var_exp.var);
            printf(".%s", var->val.var_exp.id);
            break;
        case VAR_CLASS:
            pretty_variable(var->val.var_exp.var);
            printf("->%s", var->val.var_exp.id);
            break;
        case VAR_FUNC:
            pretty_variable(var->val.var_exp.var);
            printf("->%s(", var->val.var_exp.id);
            pretty_act_list(var->val.var_exp.al);
            printf(")");
            break;
    }
}

void pretty_term(term *t)
{
    switch (t->kind)
    {
        case TERM_VAR:
            pretty_variable(t->val.var);
            break;
        case TERM_AL:
            printf("%s", t->val.term_act_list.id);
            pretty_temp_args(t->val.term_act_list.temp_args);
            printf("(");
            pretty_act_list(t->val.term_act_list.al);
            printf(")");
            break;
        case TERM_PARENTHESES:
            printf("(");
            pretty_exp(t->val.exp);
            printf(")");
            break;
        case TERM_NOT:
            printf("!");
            pretty_term(t->val.not_term);
            break;
        case TERM_ABS:
            printf("|");
            pretty_exp(t->val.exp);
            printf("|");
            break;
        case TERM_INT:
            printf("%d", t->val.integer);
            break;
        case TERM_FLOAT:
            printf("%f", t->val.floating_point);
            break;
        case TERM_TRUE:
            printf("true");
            break;
        case TERM_FALSE:
            printf("false");
            break;
        case TERM_NULL:
            printf("null");
            break;
        case TERM_STRING:
            printf("string");
            break;
    }
}

void pretty_act_list(act_list *al)
{
    switch (al->kind)
    {
        case AL_EXP_LIST:
            pretty_exp_list(al->exp_list);
            break;
        case AL_EMPTY:
            break;
    }
}

void pretty_exp_list(expression_list *exp_list)
{
    if (exp_list == NULL)
    {
        return;
    }
    pretty_exp(exp_list->exp);

    if (exp_list->exp_list != NULL)
    {
        printf(", ");
        pretty_exp_list(exp_list->exp_list);
    }
}


void pretty_exp(expression *exp)
{
    switch (exp->kind)
    {
        case EXP_TIMES:
            printf("(");
            pretty_exp(exp->val.op.left);
            printf(" * ");
            pretty_exp(exp->val.op.right);
            printf(")");
            break;
        case EXP_DIV:
            printf("(");
            pretty_exp(exp->val.op.left);
            printf(" / ");
            pretty_exp(exp->val.op.right);
            printf(")");
            break;
        case EXP_PLUS:
            printf("(");
            pretty_exp(exp->val.op.left);
            printf(" + ");
            pretty_exp(exp->val.op.right);
            printf(")");
            break;
        case EXP_MINUS:
            printf("(");
            pretty_exp(exp->val.op.left);
            printf(" - ");
            pretty_exp(exp->val.op.right);
            printf(")");
            break;
        case EXP_TERM:
            pretty_term(exp->val.term);
            break;
        case EXP_EQ:
            printf("(");
            pretty_exp(exp->val.op.left);
            printf(" == ");
            pretty_exp(exp->val.op.right);
            printf(")");
            break;
        case EXP_NEQ:
            printf("(");
            pretty_exp(exp->val.op.left);
            printf(" != ");
            pretty_exp(exp->val.op.right);
            printf(")");
            break;
        case EXP_GREATER:
            printf("(");
            pretty_exp(exp->val.op.left);
            printf(" > ");
            pretty_exp(exp->val.op.right);
            printf(")");
            break;
        case EXP_LESS:
            printf("(");
            pretty_exp(exp->val.op.left);
            printf(" < ");
            pretty_exp(exp->val.op.right);
            printf(")");
            break;
        case EXP_GEQ:
            printf("(");
            pretty_exp(exp->val.op.left);
            printf(" >= ");
            pretty_exp(exp->val.op.right);
            printf(")");
            break;
        case EXP_LEQ:
            printf("(");
            pretty_exp(exp->val.op.left);
            printf(" <= ");
            pretty_exp(exp->val.op.right);
            printf(")");
            break;
        case EXP_AND:
            printf("(");
            pretty_exp(exp->val.op.left);
            printf(" && ");
            pretty_exp(exp->val.op.right);
            printf(")");
            break;
        case EXP_OR:
            printf("(");
            pretty_exp(exp->val.op.left);
            printf(" || ");
            pretty_exp(exp->val.op.right);
            printf(")");
            break;
    }
    pretty_sym_type(exp->sym_type);
}

void pretty_switch(switch_stm *s_stm)
{
    if (s_stm == NULL)
    {
        return;
    }
    pretty_switch_case(s_stm->switch_case);
    pretty_switch(s_stm->switch_list);

}

void pretty_switch_case(switch_case *s_case)
{
    printf("case ");
    pretty_exp(s_case->exp);
    printf(":");
    space += INDENTATION;
    pretty_statement(s_case->stm);
    space -= INDENTATION;
    print_indentation();
}

void pretty_template(template *temp)
{
    switch (temp->kind)
    {
        case TEMP:
            printf("template<");
            pretty_tl(temp->tl);
            printf(">");
            break;
        case TEMP_EMPTY:
            break;
    }
}

void pretty_tl(temp_list *tl)
{
    switch (tl->kind)
    {
        case TL_TDL:
            pretty_tdl(tl->tdl);
            break;
        case TL_EMPTY:
            break;
    }
}

void pretty_tdl(temp_decl_list *tdl)
{
    switch (tdl->kind)
    {
        case TDL_ID_TDL:
            printf("%s", tdl->id);
            printf(", ");
            pretty_tdl(tdl->tdl);
            break;
        case TDL_ID:
            printf("%s", tdl->id);
            break;
    }
}

void pretty_temp_args(temp_args *ta)
{
    switch (ta->kind)
    {
        case TEMP_ARG:
            printf("<");
            pretty_tal(ta->tal);
            printf(">");
            break;
        case TEMP_ARG_EMPTY:
            break;
    }
}

void pretty_tal(temp_args_list *tal)
{
    switch (tal->kind)
    {
        case TL_TDL:
            pretty_tadl(tal->tadl);
            break;
        case TL_EMPTY:
            break;
    }
}

void pretty_tadl(temp_args_decl_list *tadl)
{
    switch (tadl->kind)
    {
        case TDL_ID_TDL:
            pretty_type(tadl->t);
            printf(", ");
            pretty_tadl(tadl->tadl);
            break;
        case TDL_ID:
            pretty_type(tadl->t);
            break;
    }
}

void pretty_sym_type(struct symbol_type *s)
{
    if (!PRINT_TYPES)
    {
        return;
    }

    if (s == NULL)
    {
        fprintf(stderr, "Can't not print type");
        return;
    }
    switch (s->kind)
    {
        case SYMBOL_ID:
            pretty_type(s->val.type_p);
            break;
        case SYMBOL_INT:
            printf(":int");
            break;
        case SYMBOL_FLOAT:
            printf(":float");
            break;
        case SYMBOL_BOOL:
            printf(":bool");
            break;
        case SYMBOL_ARRAY:
            printf(":array");
            break;
        case SYMBOL_RECORD:
            printf(":record");
            break;
        case SYMBOL_FUNC:
            printf(":func");
            break;
        case SYMBOL_NULL:
            printf(":null");
            break;
        case SYMBOL_UNKNOWN:
            printf(":unknown");
            break;
        case SYMBOL_CLASS:
            break;
        case SYMBOL_STRING:
            printf(":string");
            break;
        case SYMBOL_TEMPLATED:
            printf(":templated");
            break;
    }
}


void print_indentation()
{
    printf("\n");
    for (unsigned int i = 0; i < space; i++)
    {
        printf(" ");
    }
}

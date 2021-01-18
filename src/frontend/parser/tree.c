#include <stdio.h>
#include "../../utils/memory/memory.h"
#include "tree.h"
#include "../../utils/string_util.h"

extern int line_num;

symbol_type *make_symbol_type(int kind)
{
    symbol_type *st = NEW(symbol_type);
    st->kind = kind;
    st->has_been_visited = false;
    st->access = AM_PUBLIC;
    return st;
}

function *make_function(template *template, inline_hint *il_hint, head *head, body *body,
                        tail *tail)
{
    function *func = NEW(function);
    func->line_no = line_num;
    func->access = AM_PUBLIC;

    func->template = template;
    func->il_hint = il_hint;
    func->head = head;
    func->body = body;
    func->tail = tail;

    func->sym_type = NULL;
    func->st = NULL;
    func->was_templated = false;

    return func;
}

inline_hint *make_inline_hint(int kind)
{
    inline_hint *il_hint = NEW(inline_hint);
    il_hint->line_no = line_num;
    il_hint->kind = kind;
    il_hint->should_remove = false;
    il_hint->calls_func = false;
    il_hint->num_stm = 0;
    il_hint->num_called = 0;
    return il_hint;
}

tail *make_tail(char *id)
{
    tail *t = NEW(tail);
    t->line_no = line_num;
    t->id = id;
    return t;
}

head *make_head(char *id, par_decl_list *pdl, type *t)
{
    head *h = NEW(head);
    h->line_no = line_num;
    h->id = id;
    h->pdl = pdl;
    h->type = t;
    h->num_args = 0;
    h->st = NULL;
    h->sym_type = NULL;
    h->original_name = str_concat(id, "");
    return h;
}

body *make_body(decl_list *dl, statement_list *sl)
{
    body *b = NEW(body);
    b->line_no = line_num;
    b->dl = dl;
    b->sl = sl;
    b->st = NULL;
    b->sym_type = NULL;
    return b;
}

type *make_type_id(char *id, temp_args *ta)
{
    type *r_t = NEW(type);
    r_t->line_no = line_num;
    r_t->kind = TYPE_ID;
    r_t->id = id;
    r_t->has_been_visited = false;
    r_t->st = NULL;
    r_t->sym_type = NULL;
    r_t->is_temp_arg = false;
    r_t->ta = ta;
    return r_t;
}

type *make_type_int()
{
    type *r_t = NEW(type);
    r_t->line_no = line_num;
    r_t->kind = TYPE_INT;
    r_t->has_been_visited = false;
    r_t->st = NULL;
    r_t->sym_type = NULL;
    r_t->is_temp_arg = false;
    r_t->ta = NULL;
    return r_t;
}

type *make_type_float()
{
    type *r_t = NEW(type);
    r_t->line_no = line_num;
    r_t->kind = TYPE_FLOAT;
    r_t->has_been_visited = false;
    r_t->st = NULL;
    r_t->sym_type = NULL;
    r_t->is_temp_arg = false;
    r_t->ta = NULL;
    return r_t;
}

type *make_type_bool()
{
    type *r_t = NEW(type);
    r_t->line_no = line_num;
    r_t->kind = TYPE_BOOL;
    r_t->has_been_visited = false;
    r_t->st = NULL;
    r_t->sym_type = NULL;
    r_t->is_temp_arg = false;
    r_t->ta = NULL;
    return r_t;
}

type *make_type_string()
{
    type *r_t = NEW(type);
    r_t->line_no = line_num;
    r_t->kind = TYPE_STRING;
    r_t->has_been_visited = false;
    r_t->st = NULL;
    r_t->sym_type = NULL;
    r_t->is_temp_arg = false;
    r_t->ta = NULL;
    return r_t;
}

type *make_type_array(type *t)
{
    type *r_t = NEW(type);
    r_t->line_no = line_num;
    r_t->kind = TYPE_ARRAY;
    r_t->val.type = t;
    r_t->has_been_visited = false;
    r_t->st = NULL;
    r_t->sym_type = NULL;
    r_t->is_temp_arg = false;
    r_t->ta = NULL;
    return r_t;
}

type *make_type_record(var_decl_list *vdl)
{
    type *r_t = NEW(type);
    r_t->line_no = line_num;
    r_t->kind = TYPE_RECORD;
    r_t->val.vdl = vdl;
    r_t->has_been_visited = false;
    r_t->st = NULL;
    r_t->sym_type = NULL;
    r_t->is_temp_arg = false;
    r_t->ta = NULL;
    return r_t;
}

type *make_type_class(template *temp, parent_list *pl, decl_list *dl)
{
    type *r_t = NEW(type);
    r_t->line_no = line_num;
    r_t->kind = TYPE_CLASS;
    r_t->has_been_visited = false;
    r_t->val._class.pl = pl;
    r_t->val._class.dl = dl;
    r_t->val._class.temp = temp;
    r_t->st = NULL;
    r_t->sym_type = NULL;
    r_t->is_temp_arg = false;
    r_t->ta = NULL;
    return r_t;
}

par_decl_list *make_pdl_vdl(var_decl_list *vdl)
{
    par_decl_list *pdl = NEW(par_decl_list);
    pdl->line_no = line_num;
    pdl->kind = PDL_VDL;
    pdl->vdl = vdl;
    pdl->sym_type = NULL;
    pdl->st = NULL;
    return pdl;
}

par_decl_list *make_pdl_empty()
{
    par_decl_list *pdl = NEW(par_decl_list);
    pdl->line_no = line_num;
    pdl->kind = PDL_EMPTY;
    pdl->vdl = NULL;
    pdl->sym_type = NULL;
    pdl->st = NULL;
    return pdl;
}

var_decl_list *make_vdl_vt_list(var_type *vt, var_decl_list *vdl)
{
    var_decl_list *r_vdl = NEW(var_decl_list);
    r_vdl->line_no = line_num;
    r_vdl->kind = VDL_VT_VDL;
    r_vdl->vt = vt;
    r_vdl->vdl = vdl;
    r_vdl->st = NULL;
    r_vdl->sym_type = NULL;
    return r_vdl;
}

var_decl_list *make_vdl_vt(var_type *vt)
{
    var_decl_list *r_vdl = NEW(var_decl_list);
    r_vdl->line_no = line_num;
    r_vdl->kind = VDL_VT;
    r_vdl->vt = vt;
    r_vdl->vdl = NULL;
    r_vdl->st = NULL;
    r_vdl->sym_type = NULL;
    return r_vdl;
}

var_type *make_var_type(char *id, type *t)
{
    var_type *vt = NEW(var_type);
    vt->line_no = line_num;
    vt->id = id;
    vt->type = t;
    vt->st = NULL;
    vt->sym_type = NULL;
    vt->access = AM_PUBLIC;
    return vt;
}

statement_list *make_sl_statement(statement *stm)
{
    statement_list *r_sl = NEW(statement_list);
    r_sl->line_no = line_num;
    r_sl->kind = SL_STM;
    r_sl->stm = stm;
    r_sl->sl = NULL;
    r_sl->st = NULL;
    r_sl->sym_type = NULL;
    return r_sl;
}

statement_list *make_sl_statement_list(statement *stm, statement_list *sl)
{
    statement_list *r_sl = NEW(statement_list);
    r_sl->line_no = line_num;
    r_sl->kind = SL_STM_SL;
    r_sl->stm = stm;
    r_sl->sl = sl;
    r_sl->st = NULL;
    r_sl->sym_type = NULL;
    return r_sl;
}

statement *make_statement_return(expression *exp)
{
    statement *s = NEW(statement);
    s->line_no = line_num;
    s->kind = STM_RETURN;
    s->val.exp = exp;
    s->has_return = s->has_skip = false;
    s->sym_type = NULL;
    s->st = NULL;
    s->is_return = false;
    s->is_stm_end = false;
    return s;
}

statement *make_statement_write(expression *exp)
{
    statement *s = NEW(statement);
    s->line_no = line_num;
    s->kind = STM_WRITE;
    s->val.exp = exp;
    s->has_return = s->has_skip = false;
    s->sym_type = NULL;
    s->st = NULL;
    s->is_return = false;
    s->is_stm_end = false;
    return s;
}

statement *make_statement_allocate(variable *var)
{
    statement *s = NEW(statement);
    s->line_no = line_num;
    s->kind = STM_ALLOCATE;
    s->val.var = var;
    s->has_return = s->has_skip = false;
    s->sym_type = NULL;
    s->st = NULL;
    s->is_return = false;
    s->is_stm_end = false;
    return s;
}

statement *make_statement_allocate_length(variable *var, expression *exp)
{
    statement *s = NEW(statement);
    s->line_no = line_num;
    s->kind = STM_ALLOCATE_OF_LENGTH;
    s->val.assignment.exp = exp;
    s->val.assignment.var = var;
    s->has_return = s->has_skip = false;
    s->sym_type = NULL;
    s->st = NULL;
    s->is_return = false;
    s->is_stm_end = false;
    return s;
}

statement *make_statement_assignment(variable *var, expression *exp)
{
    statement *s = NEW(statement);
    s->line_no = line_num;
    s->kind = STM_ASSIGNMENT;
    s->val.assignment.exp = exp;
    s->val.assignment.var = var;
    s->has_return = s->has_skip = false;
    s->sym_type = NULL;
    s->st = NULL;
    s->is_return = false;
    s->is_stm_end = false;
    return s;
}

statement *make_statement_assignment_list(variable *var, expression_list *el)
{
    statement *s = NEW(statement);
    s->line_no = line_num;
    s->kind = STM_ASSIGN_LIST;
    s->val.assign_list.el = el;
    s->val.assign_list.var = var;
    s->has_return = s->has_skip = false;
    s->sym_type = NULL;
    s->st = NULL;
    s->is_return = false;
    s->is_stm_end = false;
    return s;
}

statement *make_statement_if(expression *exp, statement *stm)
{
    statement *s = NEW(statement);
    s->line_no = line_num;
    s->kind = STM_IF;
    s->val.if_then.exp = exp;
    s->val.if_then.stm = stm;
    s->has_return = s->has_skip = false;
    s->sym_type = NULL;
    s->st = NULL;
    s->is_return = false;
    s->is_stm_end = false;
    return s;
}

statement *make_statement_else(expression *exp, statement *stm1,
                               statement *stm2)
{
    statement *s = NEW(statement);
    s->line_no = line_num;
    s->kind = STM_ELSE;
    s->val.if_else.exp = exp;
    s->val.if_else.stm1 = stm1;
    s->val.if_else.stm2 = stm2;
    s->has_return = s->has_skip = false;
    s->sym_type = NULL;
    s->st = NULL;
    s->is_return = false;
    s->is_stm_end = false;
    return s;
}

statement *make_statement_while(expression *exp, statement *stm, int kind)
{
    statement *s = NEW(statement);
    s->line_no = line_num;
    s->kind = STM_WHILE;
    s->val.while_do.exp = exp;
    s->val.while_do.stm = stm;
    s->val.while_do.kind = kind;
    s->has_return = s->has_skip = false;
    s->sym_type = NULL;
    s->st = NULL;
    s->is_return = false;
    s->is_stm_end = false;
    return s;
}

statement *make_statement_sl(statement_list *sl)
{
    statement *s = NEW(statement);
    s->line_no = line_num;
    s->kind = STM_LIST;
    s->val.sl = sl;
    s->has_return = s->has_skip = false;
    s->sym_type = NULL;
    s->st = NULL;
    s->is_return = false;
    s->is_stm_end = false;
    return s;
}

statement *make_statement_op_assignment(int exp_kind, variable *var,
                                        expression *exp)
{
    statement *s = NEW(statement);
    s->line_no = line_num;
    s->kind = STM_ASSIGNMENT;

    expression *e = make_exp(exp_kind,
                             make_exp_term(make_term_var(var)), exp);

    s->val.assignment.var = var;
    s->val.assignment.exp = e;
    s->has_return = s->has_skip = false;
    s->sym_type = NULL;
    s->st = NULL;
    s->is_return = false;
    s->is_stm_end = false;
    return s;
}

statement *make_statement_keyword(int kind)
{
    statement *s = NEW(statement);
    s->line_no = line_num;
    s->kind = kind;
    s->has_return = s->has_skip = false;
    s->sym_type = NULL;
    s->st = NULL;
    s->is_return = false;
    s->is_stm_end = false;
    return s;
}

statement *make_statement_for_loop(statement *init, expression *cond,
                                   statement *iter, statement* stm)
{
    if (init->kind != STM_ASSIGNMENT || iter->kind != STM_ASSIGNMENT)
    {
        print_error("Init and Iteration statement must be an assignment",
                    line_num, 1);
    }

    statement_list *inc = make_sl_statement_list(iter, make_sl_statement(stm));
    statement *while_loop = make_statement_while(cond, make_statement_sl(inc),
            0);
    statement_list *final = make_sl_statement(init);

    statement_list *whole = make_sl_statement_list(while_loop, final);

    statement *res = make_statement_sl(whole);
    return res;

}

statement *make_statement_to_loop(statement *init, expression *cond,
                                  statement *stm)
{
    if (init->kind != STM_ASSIGNMENT)
    {
        print_error("Init statement must be an assignment", line_num, 1);
    }

    statement *iter = make_statement_assignment(init->val.var,
                make_exp(EXP_PLUS, make_exp_term(make_term_var(init->val.var)),
                make_exp_term(make_term_int(1))));
    statement_list *inc = make_sl_statement_list(iter, make_sl_statement(stm));

    expression *exp =
         make_exp(EXP_LESS, make_exp_term(make_term_var(init->val.var)), cond);

    statement *while_loop = make_statement_while(exp, make_statement_sl(inc), 0);
    statement_list *final = make_sl_statement(init);

    statement_list *whole = make_sl_statement_list(while_loop, final);

    statement *res = make_statement_sl(whole);
    return res;
}

statement *make_statement_in(variable *var)
{
    statement *s = NEW(statement);
    s->line_no = line_num;
    s->kind = STM_IN;
    s->has_return = s->has_skip = false;
    s->sym_type = NULL;
    s->st = NULL;
    s->val.var = var;
    s->is_return = false;
    s->is_stm_end = false;
    return s;
}

statement *make_statement_cond_exp(variable *var, expression* exp1,
                                   expression *exp2, expression *exp3)
{
    statement *first_assign = make_statement_assignment(var, exp2);
    statement *second_assign = make_statement_assignment(var, exp3);
    statement *if_else = make_statement_else(exp1, first_assign,second_assign);

    return if_else;
}

statement *make_statement_free(variable *var)
{
    statement *s = NEW(statement);
    s->line_no = line_num;
    s->kind = STM_FREE;
    s->val.var = var;
    s->has_return = s->has_skip = false;
    s->sym_type = NULL;
    s->st = NULL;
    s->is_return = false;
    s->is_stm_end = false;
    return s;
}

statement *make_statement_assign_val(variable *var, expression *exp)
{
    statement *s = NEW(statement);
    s->line_no = line_num;
    s->kind = STM_ASSIGN_VAL;
    s->val.assignment.var = var;
    s->val.assignment.exp = exp;
    s->has_return = s->has_skip = false;
    s->sym_type = NULL;
    s->st = NULL;
    s->is_return = false;
    s->is_stm_end = false;
    return s;
}

statement *make_statement_switch(expression *exp, switch_stm *s_stm)
{
    statement *s = NEW(statement);
    s->line_no = line_num;
    s->kind = STM_SWITCH;
    s->val.switch_stm.exp = exp;
    s->val.switch_stm.s_stm = s_stm;
    s->has_return = s->has_skip = false;
    s->sym_type = NULL;
    s->st = NULL;
    s->is_return = false;
    s->is_stm_end = false;
    return s;
}

decl_list *make_dl_declaration(declaration *decl, decl_list *dl)
{
    decl_list *d = NEW(decl_list);
    d->line_no = line_num;
    d->kind = DL_DECL_DL;
    d->decl = decl;
    d->dl = dl;
    d->st = NULL;
    d->sym_type = NULL;
    return d;
}

decl_list *make_dl_empty()
{
    decl_list *d = NEW(decl_list);
    d->line_no = line_num;
    d->kind = DL_EMPTY;
    d->decl = NULL;
    d->dl = NULL;
    d->st = NULL;
    d->sym_type = NULL;
    return d;
}

declaration *make_declaration_type(access_modifier *am, char *id, type *type)
{
    declaration *d = NEW(declaration);
    d->line_no = line_num;
    d->kind = DECL_TYPE;
    d->val.type.id = id;
    d->val.type.original_name = id;
    d->val.type.type = type;
    d->st = NULL;
    d->sym_type = NULL;
    d->am = am;
    return d;
}

declaration *make_declaration_function(access_modifier *am, function *func)
{
    declaration *d = NEW(declaration);
    d->line_no = line_num;
    d->kind = DECL_FUNC;
    d->val.func = func;
    d->st = NULL;
    d->sym_type = NULL;
    d->am = am;
    func->access = am->kind;
    return d;
}

declaration *make_declaration_vdl(access_modifier *am, var_decl_list *vdl)
{
    declaration *d = NEW(declaration);
    d->line_no = line_num;
    d->kind = DECL_VDL;
    d->val.vdl = vdl;
    d->st = NULL;
    d->sym_type = NULL;
    d->am = am;

    var_decl_list *next = vdl;
    while (next != NULL)
    {
        next->vt->access = am->kind;
        next = next->vdl;
    }
    return d;
}

declaration *make_decl_enum(enum_list *el)
{
    declaration *d = NEW(declaration);
    var_decl_list *vdl = make_vdl_vt(make_var_type(el->id, make_type_int()));
    vector_push(enum_ids, el->id);

    while (el->e_list != NULL)
    {
        el = el->e_list;
        vector_push(enum_ids, el->id);
        vdl = make_vdl_vt_list(make_var_type(el->id, make_type_int()), vdl);
    }

    d->line_no = line_num;
    d->kind = DECL_VDL;
    d->val.vdl = vdl;
    d->st = NULL;
    d->sym_type = NULL;
    d->am = NULL;
    return d;
}

declaration *make_decl_const(access_modifier *am, type *t, char *id,
                             expression *exp)
{
    declaration *d = NEW(declaration);
    d->line_no = line_num;
    d->kind = DECL_CONST;
    d->val.con.type = t;
    d->val.con.id = id;
    d->val.con.exp = exp;
    d->st = NULL;
    d->sym_type = NULL;
    d->am = am;
    return d;
}


variable *make_var_id(char *id)
{
    variable *v = NEW(variable);
    v->line_no = line_num;
    v->kind = VAR_ID;
    v->val.var_exp.id = id;
    v->num_assign = 0;
    v->sym_type = NULL;
    v->st = NULL;
    return v;
}

variable *make_var_exp(variable *var, expression_list *exp)
{
    variable *v = NEW(variable);
    v->line_no = line_num;
    v->kind = VAR_EXP;
    v->num_assign = 0;
    v->val.var_exp.var = var;
    v->val.var_exp.exp = exp;
    v->sym_type = NULL;
    v->st = NULL;
    return v;
}

variable *make_var_dot(variable *var, char *id)
{
    variable *v = NEW(variable);
    v->line_no = line_num;
    v->kind = VAR_RECORD;
    v->num_assign = 0;
    v->val.var_exp.id = id;
    v->val.var_exp.var = var;
    var->val.var_exp.al = NULL;
    v->sym_type = NULL;
    v->st = NULL;
    return v;
}

variable *make_var_arrow(variable *var, char *id)
{
    variable *v = NEW(variable);
    v->line_no = line_num;
    v->kind = VAR_CLASS;
    v->num_assign = 0;
    v->val.var_exp.id = id;
    v->val.var_exp.var = var;
    var->val.var_exp.al = NULL;
    v->sym_type = NULL;
    v->st = NULL;
    return v;
}

variable *make_var_func(variable *var, char *id, act_list *al)
{
    variable *v = NEW(variable);
    v->line_no = line_num;
    v->kind = VAR_FUNC;
    v->num_assign = 0;
    v->val.var_exp.id = id;
    v->val.var_exp.var = var;
    v->val.var_exp.al = al;
    v->val.var_exp.original_name = str_concat(id, "");
    v->sym_type = NULL;
    v->st = NULL;
    return v;
}

term *make_term_var(variable *var)
{

    term *t = NEW(term);
    t->line_no = line_num;
    t->kind = TERM_VAR;
    t->val.var = var;
    t->sym_type = NULL;
    t->st = NULL;
    t->val.term_act_list.was_templated = false;
    return t;
}

term *make_term_al(char *id, temp_args *temp_args, act_list *al)
{
    term *t = NEW(term);
    t->line_no = line_num;
    t->kind = TERM_AL;
    t->val.term_act_list.original_name = str_concat(id, "");
    t->val.term_act_list.id = id;
    t->val.term_act_list.al = al;
    t->val.term_act_list.func = NULL;
    t->sym_type = NULL;
    t->st = NULL;
    t->val.term_act_list.temp_args = temp_args;
    return t;
}

term *make_term_parentheses(expression *exp)
{

    term *t = NEW(term);
    t->line_no = line_num;
    t->kind = TERM_PARENTHESES;
    t->val.exp = exp;
    t->sym_type = NULL;
    t->st = NULL;
    return t;
}

term *make_term_not(term *t)
{
    term *r_t = NEW(term);
    r_t->line_no = line_num;
    r_t->kind = TERM_NOT;
    r_t->val.not_term = t;
    r_t->sym_type = NULL;
    r_t->st = NULL;
    return r_t;
}

term *make_term_absolute(expression *exp)
{
    term *t = NEW(term);
    t->line_no = line_num;
    t->kind = TERM_ABS;
    t->val.exp = exp;
    t->sym_type = NULL;
    t->st = NULL;
    return t;
}

term *make_term_int(int num)
{
    term *t = NEW(term);
    t->line_no = line_num;
    t->kind = TERM_INT;
    t->val.integer = num;
    t->sym_type = NULL;
    t->st = NULL;
    return t;
}

term *make_term_float(float num)
{
    term *t = NEW(term);
    t->line_no = line_num;
    t->kind = TERM_FLOAT;
    t->val.floating_point = num;
    t->sym_type = NULL;
    t->st = NULL;
    return t;
}

term *make_term(int kind)
{
    term *t = NEW(term);
    t->line_no = line_num;
    t->kind = kind;
    t->sym_type = NULL;
    t->st = NULL;
    return t;
}

term *make_term_string(char *string)
{
    term *t = NEW(term);
    t->line_no = line_num;
    t->kind = TERM_STRING;
    t->sym_type = NULL;
    t->st = NULL;
    t->val.string = string;
    return t;
}

act_list *make_act_list(expression_list *exp_list)
{
    act_list *al = NEW(act_list);
    al->line_no = line_num;
    al->kind = AL_EXP_LIST;
    al->exp_list = exp_list;
    al->st = NULL;
    al->sym_type = NULL;
    return al;
}

act_list *make_act_list_empty()
{
    act_list *al = NEW(act_list);
    al->line_no = line_num;
    al->kind = AL_EMPTY;
    al->exp_list = NULL;
    al->st = NULL;
    al->sym_type = NULL;
    return al;
}

expression_list *make_exp_list_exp(expression *exp)
{
    expression_list *exp_l = NEW(expression_list);
    exp_l->line_no = line_num;
    exp_l->kind = EXP_LIST_EXP;
    exp_l->exp = exp;
    exp_l->exp_list = NULL;
    exp_l->st = NULL;
    exp_l->sym_type = NULL;
    return exp_l;
}

expression_list *make_exp_list_list(expression *exp, expression_list *exp_list)
{
    expression_list *exp_l = NEW(expression_list);
    exp_l->line_no = line_num;
    exp_l->kind = EXP_LIST_EXP_LIST;
    exp_l->exp = exp;
    exp_l->exp_list = exp_list;
    exp_l->st = NULL;
    exp_l->sym_type = NULL;
    return exp_l;
}

expression *make_exp_term(term *term)
{
    expression *exp;
    exp = NEW(expression);
    exp->line_no = line_num;
    exp->kind = EXP_TERM;
    exp->val.term = term;
    exp->st = NULL;
    exp->sym_type = NULL;
    return exp;
}

expression *make_exp(int id, expression *left, expression *right)
{
    expression *exp;
    exp = NEW(expression);
    exp->line_no = line_num;
    exp->kind = id;
    exp->val.op.left = left;
    exp->val.op.right = right;
    exp->st = NULL;
    exp->sym_type = NULL;
    return exp;
}

class_decl_list *make_cdl(class_decl *class_dec)
{
    class_decl_list *cdl = NEW(class_decl_list);
    cdl->line_no = line_num;
    cdl->class_decl = class_dec;
    cdl->kind = CDL_DECL;
    cdl->cdl = NULL;
    cdl->st = NULL;
    cdl->sym_type = NULL;
    return cdl;
}

class_decl_list *make_cdl_list(class_decl *class_dec, class_decl_list *dcl)
{
    class_decl_list *cdl = NEW(class_decl_list);
    cdl->line_no = line_num;
    cdl->class_decl = class_dec;
    cdl->cdl = dcl;
    cdl->kind = CDL_LIST;
    cdl->st = NULL;
    cdl->sym_type = NULL;
    return cdl;
}

class_decl *make_class(char *id, parent_list* pl, decl_list *dl)
{
    class_decl *cd = NEW(class_decl);
    cd->line_no = line_num;
    cd->id = id;
    cd->dl = dl;
    cd->pl = pl;
    cd->kind = CLASS;
    cd->sym_type = NULL;
    cd->st = NULL;
    return cd;
}

class_decl *make_empty_class()
{
    class_decl *cd = NEW(class_decl);
    cd->line_no = line_num;
    cd->kind = CLASS_EMPTY;
    cd->pl = NULL;
    cd->sym_type = NULL;
    cd->st = NULL;
    return cd;
}

enum_list *make_enum_list(char *id, enum_list *list)
{
    enum_list *el = NEW(enum_list);
    el->line_no = line_num;
    el->kind = ENUM_LIST;
    el->id = id;
    el->e_list = list;
    return el;
}

enum_list *make_enum_id(char *id)
{
    enum_list *el = NEW(enum_list);
    el->line_no = line_num;
    el->kind = ENUM_EMPTY;
    el->e_list = NULL;
    el->id = id;
    return el;
}

parent_list *make_parent_list(char *id)
{
    parent_list *pl = NEW(parent_list);
    pl->line_no = line_num;
    pl->kind = PL_PARENT;
    pl->id = id;
    pl->sym_type = NULL;
    pl->st = NULL;
    return pl;
}

parent_list *make_parent_list_empty()
{
    parent_list *pl = NEW(parent_list);
    pl->line_no = line_num;
    pl->kind = PL_EMPTY;
    pl->sym_type = NULL;
    pl->st = NULL;
    return pl;
}

access_modifier *make_access_modifier(int kind)
{
    access_modifier *am = NEW(access_modifier);
    am->line_no = line_num;
    am->kind = kind;
    return am;
}

switch_stm *make_switch_stm_case(switch_case *sc)
{
    switch_stm *s_stm = NEW(switch_stm);
    s_stm->line_no = line_num;
    s_stm->kind = SWITCH_STM;
    s_stm->switch_case = sc;
    s_stm->switch_list = NULL;
    return s_stm;
}

switch_stm *make_switch_stm_list(switch_case *sc, switch_stm *stm)
{
    switch_stm *s_stm = NEW(switch_stm);
    s_stm->line_no = line_num;
    s_stm->kind = SWITCH_LIST;
    s_stm->switch_case = sc;
    s_stm->switch_list = stm;
    return s_stm;
}

switch_case *make_switch_case(expression *exp, statement *stm)
{
    switch_case *s_case = NEW(switch_case);
    s_case->line_no = line_num;
    s_case->stm = stm;
    s_case->exp = exp;
    return s_case;
}

template *make_template(temp_list *tl)
{
    template *temp = NEW(template);
    temp->line_no = line_num;
    temp->kind = TEMP;
    temp->tl = tl;
    temp->num_para = -1;
    return temp;
}

template *make_template_empty()
{
    template *temp = NEW(template);
    temp->line_no = line_num;
    temp->kind = TEMP_EMPTY;
    temp->tl = NULL;
    temp->num_para = -1;
    return temp;
}

temp_list *make_tl(temp_decl_list *tdl)
{
    temp_list *tl = NEW(temp_list);
    tl->line_no = line_num;
    tl->tdl = tdl;
    tl->kind = TL_TDL;
    return tl;
}
temp_list *make_tl_empty()
{
    temp_list *tl = NEW(temp_list);
    tl->line_no = line_num;
    tl->kind = TL_EMPTY;
    tl->tdl = NULL;
    return tl;
}

temp_decl_list *make_tdl_id_list(char *id, temp_decl_list *tdl)
{
    temp_decl_list *tmp_tdl = NEW(temp_decl_list);
    tmp_tdl->line_no = line_num;
    tmp_tdl->kind = TL_TDL;
    tmp_tdl->id = id;
    tmp_tdl->tdl = tdl;
    return tmp_tdl;
}
temp_decl_list *make_tdl_id(char *id)
{
    temp_decl_list *tdl = NEW(temp_decl_list);
    tdl->line_no = line_num;
    tdl->kind = TL_EMPTY;
    tdl->id = id;
    tdl->tdl = NULL;
    return tdl;
}


temp_args *make_temp_args(temp_args_list *tal)
{
    temp_args *temp = NEW(temp_args);
    temp->line_no = line_num;
    temp->kind = TEMP_ARG;
    temp->tal = tal;
    temp->num_args = -1;
    return temp;
}
temp_args *make_temp_args_empty()
{
    temp_args *temp = NEW(temp_args);
    temp->line_no = line_num;
    temp->kind = TEMP_ARG_EMPTY;
    temp->tal = NULL;
    temp->num_args = -1;
    return temp;
}

temp_args_list *make_temp_args_list(temp_args_decl_list *tadl)
{
    temp_args_list *tal = NEW(temp_args_list);
    tal->line_no = line_num;
    tal->tadl = tadl;
    tal->kind = TAL_TADL;
    return tal;
}
temp_args_list *make_temp_args_list_empty()
{
    temp_args_list *tal = NEW(temp_args_list);
    tal->line_no = line_num;
    tal->tadl = NULL;
    tal->kind = TAL_EMPTY;
    return tal;
}

temp_args_decl_list *make_temp_args_decl_type_list(type *t, temp_args_decl_list *tadl)
{
    temp_args_decl_list *tmp_tadl = NEW(temp_args_decl_list);
    tmp_tadl->line_no = line_num;
    tmp_tadl->kind = TADL_TYPE_TADL;
    tmp_tadl->t = t;
    tmp_tadl->tadl = tadl;
    return tmp_tadl;
}
temp_args_decl_list *make_temp_args_decl_type(type *t)
{
    temp_args_decl_list *tmp_tadl = NEW(temp_args_decl_list);
    tmp_tadl->line_no = line_num;
    tmp_tadl->kind = TADL_TYPE;
    tmp_tadl->t = t;
    tmp_tadl->tadl = NULL;
    return tmp_tadl;
}

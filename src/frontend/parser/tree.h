#ifndef COMPILER_TREE_H
#define COMPILER_TREE_H

#include "../../utils/util.h"
#include "../../utils/vector.h"

vector *enum_ids;

typedef struct symbol_type
{
    enum {SYMBOL_ID, SYMBOL_INT, SYMBOL_FLOAT, SYMBOL_BOOL, SYMBOL_ARRAY, SYMBOL_RECORD,
          SYMBOL_CLASS, SYMBOL_FUNC, SYMBOL_NULL, SYMBOL_UNKNOWN, SYMBOL_TEMPLATED,
          SYMBOL_STRING} kind;
    int access;
    union
    {
        struct symbol_table *st;
        struct type *type_p;
        struct var_decl_list *vdl;
        struct
        {
            struct function *func;
            struct type *return_type;
            char *old_name;
        } func;
        struct
        {
            struct type *type_p;
            struct decl_list *dl;
            struct symbol_table *st;
            struct class_decl *self;
            char *name;
        } _class;
    } val;
    bool has_been_visited;
    struct expression *exp;

} symbol_type;

typedef struct expression
{
    struct symbol_table *st;
    symbol_type *sym_type;
    int line_no;
    enum {EXP_TIMES, EXP_DIV, EXP_PLUS, EXP_MINUS, EXP_EQ, EXP_NEQ, EXP_GREATER
          ,EXP_LESS, EXP_GEQ, EXP_LEQ, EXP_AND, EXP_OR, EXP_TERM} kind;
    union
    {
        char *id;
        int int_const;
        struct
        {
            struct expression *left;
            struct expression *right;
        } op;
        struct term *term;
    } val;
} expression;

typedef struct function
{
    struct symbol_table *st;
    symbol_type *sym_type;
    int line_no;
    int access;
    bool was_templated;

    struct template *template;
    struct inline_hint *il_hint;
    struct head *head;
    struct body *body;
    struct tail *tail;

} function;

typedef struct inline_hint
{
    struct symbol_table *st;
    symbol_type *sym_type;
    int line_no;

    bool calls_func;
    bool should_remove;
    int num_stm;
    int num_called;

    enum {IL_IL, IL_NOT, IL_EMPTY} kind;

} inline_hint;

typedef struct head
{
    struct symbol_table *st;
    symbol_type *sym_type;
    int line_no;
    char *id;
    char *new_name;
    char *original_name;

    struct par_decl_list *pdl;
    struct type *type;
    int num_args;
} head;

typedef struct body
{
    struct symbol_table *st;
    symbol_type *sym_type;
    int line_no;
    struct decl_list *dl;
    struct statement_list *sl;
    struct class_decl_list *cdl;
} body;

typedef struct tail
{
    int line_no;
    char *id;
} tail;

typedef struct type
{
    struct symbol_table *st;
    symbol_type *sym_type;
    int line_no;
    char *id;
    enum {TYPE_ID, TYPE_INT, TYPE_FLOAT, TYPE_BOOL, TYPE_STRING, TYPE_ARRAY, TYPE_RECORD,
          TYPE_CLASS} kind;
    int real_type;

    bool is_temp_arg;
    struct temp_args *ta;

    union
    {
        struct {
            struct parent_list *pl;
            struct decl_list *dl;
            char *name;
            struct template *temp;
        } _class;
        struct type *type;
        struct var_decl_list *vdl;
    } val;
    bool has_been_visited;
} type;

typedef struct par_decl_list
{
    struct symbol_table *st;
    symbol_type *sym_type;
    int line_no;
    enum {PDL_VDL, PDL_EMPTY} kind;

    struct var_decl_list *vdl;

} par_decl_list;

typedef struct var_decl_list
{
    struct symbol_table *st;
    symbol_type *sym_type;
    int line_no;
    enum {VDL_VT_VDL, VDL_VT} kind;

    struct var_type *vt;
    struct var_decl_list *vdl;

} var_decl_list;

typedef struct var_type
{
    struct symbol_table *st;
    symbol_type *sym_type;
    int line_no;
    int access;
    char *id;
    struct type *type;
} var_type;

typedef struct decl_list
{
    struct symbol_table *st;
    symbol_type *sym_type;
    int line_no;
    enum {DL_DECL_DL, DL_EMPTY} kind;
    struct declaration *decl;
    struct decl_list *dl;

} decl_list;

typedef struct declaration
{
    struct symbol_table *st;
    symbol_type *sym_type;
    int line_no;
    enum {DECL_TYPE, DECL_FUNC, DECL_VDL, DECL_CONST} kind;
    struct access_modifier *am;
    union
    {
        struct
        {
            char *id;
            char *original_name;
            struct type *type;
        } type;
        struct
        {
            char *id;
            struct type *type;
            struct expression *exp;
        } con;
        struct function *func;
        struct var_decl_list *vdl;
    } val;

} declaration;

typedef struct statement_list
{
    struct symbol_table *st;
    symbol_type *sym_type;
    int line_no;
    enum {SL_STM, SL_STM_SL} kind;
    struct statement_list *sl;
    struct statement *stm;

} statement_list;

typedef struct statement
{
    struct symbol_table *st;
    symbol_type *sym_type;
    function *func;
    int line_no;

    enum {STM_RETURN, STM_WRITE, STM_ALLOCATE, STM_ALLOCATE_OF_LENGTH,
          STM_ASSIGNMENT, STM_IF, STM_ELSE, STM_WHILE, STM_LIST,
          STM_BREAK, STM_CONTINUE, STM_IN, STM_ASSIGN_LIST,
          STM_FREE, STM_ASSIGN_VAL, STM_SWITCH} kind;

    struct statement *loop;
    char *return_label;
    bool is_return;
    bool is_stm_end;
    char *loop_label;
    char *end_label;

    union {
        struct expression *exp;
        struct variable *var;

        struct
        {
            struct variable *var;
            struct expression *exp;
        } assignment;

        struct
        {
            struct variable *var;
            struct expression_list *el;
        } assign_list;

        struct
        {
            struct expression *exp;
            struct statement *stm;
        } if_then;

        struct
        {
            struct expression *exp;
            struct statement *stm1;
            struct statement *stm2;
        } if_else;

        struct
        {
            struct expression *exp;
            struct statement *stm;
            int kind;
        } while_do;

        struct
        {
            struct expression *exp;
            struct switch_stm *s_stm;
        } switch_stm;

        struct statement_list *sl;
    } val;

    bool has_return;
    bool has_skip;      // Used for break and continue
} statement;

typedef struct variable
{
    struct symbol_table *st;
    symbol_type *sym_type;
    int line_no;

    enum {VAR_ID, VAR_EXP, VAR_RECORD, VAR_CLASS, VAR_FUNC} kind;

    union
    {
        struct
        {
            char *id;
            struct variable *var;
            struct expression_list *exp;
            struct act_list *al;
            struct function *func;
            char *original_name;
        } var_exp;
    } val;

    int real_type;
    int num_assign;

} variable;

typedef struct term
{
    struct symbol_table *st;
    symbol_type *sym_type;
    int line_no;
    enum {TERM_VAR, TERM_AL, TERM_PARENTHESES, TERM_NOT, TERM_ABS,
        TERM_INT, TERM_FLOAT, TERM_TRUE, TERM_FALSE, TERM_NULL, TERM_STRING} kind;

    union
    {
        struct variable *var;
        struct
        {
            struct temp_args *temp_args;
            char *id;
            char *temp_id; // template id
            char *oldId;
            struct act_list *al;
            struct function *func;
            struct statement *stm; // Used for templated calls
            bool was_templated;
            char *original_name;
        } term_act_list;
        struct term *not_term;
        struct expression *exp;
        int integer;
        float floating_point;
        char *string;
    } val;
} term;

typedef struct enum_list
{
    int line_no;
    char *id;
    enum {ENUM_LIST, ENUM_EMPTY} kind;
    struct enum_list *e_list;
} enum_list;

typedef struct act_list
{
    struct symbol_table *st;
    symbol_type *sym_type;
    int line_no;
    enum {AL_EXP_LIST, AL_EMPTY} kind;
    struct expression_list *exp_list;
} act_list;

typedef struct expression_list
{
    struct symbol_table *st;
    symbol_type *sym_type;
    int line_no;

    enum {EXP_LIST_EXP, EXP_LIST_EXP_LIST} kind;
    struct expression *exp;
    struct expression_list *exp_list;

} expression_list;

typedef struct class_decl_list
{
    struct symbol_table *st;
    symbol_type *sym_type;
    int line_no;
    enum {CDL_DECL, CDL_LIST} kind;
    struct class_decl_list *cdl;
    struct class_decl *class_decl;

} class_decl_list;

typedef struct class_decl
{
    struct symbol_table *st;
    symbol_type *sym_type;
    int line_no;
    char *id;
    enum {CLASS, CLASS_EMPTY} kind;
    struct decl_list *dl;
    struct parent_list *pl;
} class_decl;

typedef struct parent_list
{
    struct symbol_table *st;
    symbol_type *sym_type;
    int line_no;
    char *id;
    enum {PL_PARENT, PL_EMPTY} kind;
} parent_list;

typedef struct access_modifier
{
    int line_no;
    enum {AM_PRIVATE, AM_PUBLIC, AM_EMPTY} kind;
} access_modifier;

typedef struct switch_stm
{
    struct symbol_table *st;
    symbol_type *sym_type;
    int line_no;

    enum {SWITCH_STM, SWITCH_LIST} kind;

    struct switch_case *switch_case;
    struct switch_stm *switch_list;

} switch_stm;

typedef struct switch_case
{
    struct symbol_table *st;
    symbol_type *sym_type;
    int line_no;

    struct statement *stm;
    struct expression *exp;
} switch_case;

// Template
typedef struct template
{
    struct symbol_table *st;
    symbol_type *sym_type;
    int line_no;
    int num_para;

    enum {TEMP, TEMP_EMPTY} kind;

    struct temp_list *tl;
    // int num_args;
} template;

typedef struct temp_list
{
    struct symbol_table *st;
    symbol_type *sym_type;
    int line_no;
    enum {TL_TDL, TL_EMPTY} kind;

    struct temp_decl_list *tdl;

} temp_list;

typedef struct temp_decl_list
{
    struct symbol_table *st;
    symbol_type *sym_type;
    int line_no;
    enum {TDL_ID_TDL, TDL_ID} kind;

    char *id;
    struct temp_decl_list *tdl;
} temp_decl_list;

//------------

typedef struct temp_args
{
struct symbol_table *st;
symbol_type *sym_type;
int line_no;
int num_args;

enum {TEMP_ARG, TEMP_ARG_EMPTY} kind;

struct temp_args_list *tal;
// int num_args;
} temp_args;

typedef struct temp_args_list
{
    struct symbol_table *st;
    symbol_type *sym_type;
    int line_no;
    enum {TAL_TADL, TAL_EMPTY} kind;

    struct temp_args_decl_list *tadl;

} temp_args_list;

typedef struct temp_args_decl_list
{
    struct symbol_table *st;
    symbol_type *sym_type;
    int line_no;
    enum {TADL_TYPE_TADL, TADL_TYPE} kind;

    type *t;
    struct temp_args_decl_list *tadl;
} temp_args_decl_list;

template *make_template(temp_list *tl);
template *make_template_empty();

temp_list *make_tl(temp_decl_list *tdl);
temp_list *make_tl_empty();

temp_decl_list *make_tdl_id_list(char *id, temp_decl_list *tdl);
temp_decl_list *make_tdl_id(char *id);


temp_args *make_temp_args(temp_args_list *tal);
temp_args *make_temp_args_empty();

temp_args_list *make_temp_args_list(temp_args_decl_list *tadl);
temp_args_list *make_temp_args_list_empty();

temp_args_decl_list *make_temp_args_decl_type_list(type *t, temp_args_decl_list *tadl);
temp_args_decl_list *make_temp_args_decl_type(type *t);

// Others


symbol_type *make_symbol_type(int kind);

function *make_function(template *template, inline_hint *il_hint, head *head, body *body,
                        tail *tail);

inline_hint *make_inline_hint(int kind);
head *make_head(char *id, par_decl_list *pdl, type *type);
body *make_body(decl_list *dl, statement_list *sl);
tail *make_tail(char *id);

type *make_type_id(char *id, temp_args *ta);
type *make_type_int();
type *make_type_float();
type *make_type_bool();
type *make_type_string();
type *make_type_array(type *type);
type *make_type_record(var_decl_list *vdl);
type *make_type_class(template *temp, parent_list *pl, decl_list *dl);

par_decl_list *make_pdl_vdl(var_decl_list *vdl);
par_decl_list *make_pdl_empty();

var_decl_list *make_vdl_vt_list(var_type *vt, var_decl_list *vdl);
var_decl_list *make_vdl_vt(var_type *vt);

var_type *make_var_type(char *id, type *type);

statement_list *make_sl_statement(statement *stm);
statement_list *make_sl_statement_list(statement *stm, statement_list *sl);

statement *make_statement_return(expression *exp);
statement *make_statement_write(expression *exp);
statement *make_statement_allocate(variable *var);
statement *make_statement_allocate_length(variable *var, expression *exp);
statement *make_statement_assignment(variable *var, expression *exp);
statement *make_statement_assignment_list(variable *var, expression_list *el);
statement *make_statement_if(expression *exp, statement *stm);
statement *make_statement_else(expression *exp, statement *stm1,
                               statement *stm2);
statement *make_statement_while(expression *exp, statement *stm, int kind);
statement *make_statement_sl(statement_list *sl);
statement *make_statement_op_assignment(int exp_kind, variable *var,
                                        expression *exp);
statement *make_statement_keyword(int kind);
statement *make_statement_for_loop(statement *init, expression *cond,
                                   statement *iter, statement* stm);
statement *make_statement_to_loop(statement *init, expression *cond,
                                  statement *stm);
statement *make_statement_in(variable *var);
statement *make_statement_cond_exp(variable *var, expression* exp1,
                                   expression *exp2, expression *exp3);
statement *make_statement_free(variable *var);
statement *make_statement_assign_val(variable *var, expression *exp);
statement *make_statement_switch(expression *exp, switch_stm *s_stm);

decl_list *make_dl_declaration(declaration *decl, decl_list *dl);
decl_list *make_dl_empty();

declaration *make_declaration_type(access_modifier *am, char *id, type *type);
declaration *make_declaration_function(access_modifier *am, function *func);
declaration *make_declaration_vdl(access_modifier *am, var_decl_list *vdl);
declaration *make_decl_const(access_modifier *am, type *t, char *id,
                             expression *exp);

declaration *make_decl_enum(enum_list *el);

variable *make_var_id(char *id);
variable *make_var_exp(variable *var, expression_list *exp);
variable *make_var_dot(variable *var, char *id);
variable *make_var_arrow(variable *var, char *id);
variable *make_var_func(variable *var, char *id, act_list *al);

term *make_term_var(variable *var);
term *make_term_al(char *id, temp_args *temp_args, act_list *al);
term *make_term_parentheses(expression *exp);
term *make_term_not(term *t);
term *make_term_absolute(expression *exp);
term *make_term_int(int num);
term *make_term_float(float num);
term *make_term(int kind);
term *make_term_string(char *string);

act_list *make_act_list(expression_list *exp_list);
act_list *make_act_list_empty();

expression_list *make_exp_list_exp(expression *exp);
expression_list *make_exp_list_list(expression *exp,
                                    expression_list *exp_list);

expression *make_exp_term(term *term);
expression *make_exp(int id, expression *left, expression *right);

class_decl_list *make_cdl(class_decl *class_dec);
class_decl_list *make_cdl_list(class_decl *class_dec, class_decl_list *dcl);

class_decl *make_class(char *id, parent_list* pl, decl_list *dl);
class_decl *make_empty_class();

enum_list *make_enum_list(char *id, enum_list *list);
enum_list *make_enum_id(char *id);

parent_list *make_parent_list(char *id);
parent_list *make_parent_list_empty();

access_modifier *make_access_modifier(int kind);

switch_stm *make_switch_stm_case(switch_case *sc);
switch_stm *make_switch_stm_list(switch_case *sc, switch_stm *stm);

switch_case *make_switch_case(expression *exp, statement *stm);

#endif

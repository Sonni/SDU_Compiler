#ifndef COMPILER_ASM_H
#define COMPILER_ASM_H

#include "../reg_alloc/bit_array.h"

#define LABEL_SIZE 15
#define HEAP_SIZE 20000
#define NUM_REG 13
#define WORD_SIZE 8

int num_tmp;
struct vector *const_strings;
struct vector *floats;
int num_body_vars;
struct abstract_asm *offset_index_aac;

typedef struct operand
{
    enum
    {
        OPD_INT, OPD_FLOAT, OPD_TMP, OPD_LABEL, OPD_REGISTER, OPD_STACK, OPD_HEAP,
        OPD_TMP_SPILL, OPD_STRING
    } kind;

    union
    {
        char *id; // Stack, heap, register loc, label
        int value;
        float f_value;
        struct operand *heap_offset_reg;
        struct
        {
            struct operand *stack_reg;
            int offset;
        } stack_reg;
        struct
        {
            int id;
            struct operand *spilled;
        } tmp;
    } val;
    int tmp_id;
    int offset;
    bool is_allocated;
} operand;

typedef struct abstract_asm
{
    enum
    {
        I_ADD, I_SUB, I_MUL, I_DIV, I_INC, I_DEC,
        I_JMP, I_MOV, I_CMP,
        I_AND, I_OR, I_XOR,
        I_PUSH, I_POP, I_RET, I_CALL,
        I_LSHIFT, I_RSHIFT, I_ARSHIFT,
        I_JE, I_JNE, I_JG, I_JGE, I_JL, I_JLE,
        I_JB, I_BT, I_BTS,
        I_LABEL, I_NEG, I_INT, I_CLTD,
        I_CALL_START, I_CALL_END,
        I_CVTSS2SD
    } instruction;

    union
    {
        struct
        {
            enum
            {
                LABEL_NOM, LABEL_FUNC, LABEL_SAVE,
                LABEL_FUNC_BEGIN, LABEL_FUNC_END
            } kind;
            int num_param;
            char *id;
        } label;
        struct
        {
            operand *left;
            operand *right;
        } bin;
        struct
        {
            operand *opd;
        } unary;
    } val;

    int num_opds; //2 for bin op, 1 for unary, 0 for labels
    char *debug_code;
    struct abstract_asm *next, *previous;
    bit_array live_in, live_out;
    bit_array use, def;
} abstract_asm;

operand *get_reg(operand *opd);

operand *get_opd_reg(int num);

int get_reg_num(operand *opd);

void asm_combine_inst(abstract_asm *other, abstract_asm **cur,
                      abstract_asm **start);

void asm_combine(abstract_asm *other, abstract_asm **l, abstract_asm **t);

void asm_make_inst(abstract_asm **cur, abstract_asm **start, int kind,
                   char *dbg_code);

void asm_make_unary_inst(abstract_asm **cur, abstract_asm **start, int kind,
                         operand *opd, char *dbg_code);

void asm_make_bin_inst(abstract_asm **cur, abstract_asm **start, int kind,
                       operand *l, operand *r, char *dbg_code);

void asm_make_label(abstract_asm **cur, abstract_asm **start, int kind,
                    char *label, char *dbg_code);

void asm_make_list_elem(abstract_asm **cur, abstract_asm **start);

char *asm_make_unique_true_label();

char *asm_make_unique_end_label();

char *asm_make_unique_loop_label();

char *asm_make_unique_float_name();

operand *make_opd_label(char *id);

operand *asm_make_tmp();

operand *asm_make_int(int var);

operand *asm_make_float(float var);

operand *asm_make_string(char *string);

operand *make_op_stack_loc(operand *reg, int offset);

operand *make_spilled_tmp();

void replace_tmp_op(operand **op, operand *r_op);

operand *make_opd_heap_loc(operand *off);

extern struct operand ASM_WRITE_INT;
extern struct operand ASM_PRINTF;
extern struct operand ASM_SCANF;

extern struct operand ASM_RBP;
extern struct operand ASM_RSP;
extern struct operand ASM_RAX;
extern struct operand ASM_RBX;

extern struct operand ASM_RCX;
extern struct operand ASM_RDX;
extern struct operand ASM_RSI;
extern struct operand ASM_RDI;

extern struct operand ASM_R8;
extern struct operand ASM_R9;
extern struct operand ASM_R10;
extern struct operand ASM_R11;
extern struct operand ASM_R12;
extern struct operand ASM_R13;
extern struct operand ASM_R14;
extern struct operand ASM_R15;

extern struct operand ASM_HEAP;
extern struct operand ASM_STATIC_LINK;

extern struct operand ASM_XMM0;

#endif

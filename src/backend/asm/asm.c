#include <stdio.h>
#include "asm.h"
#include "../../utils/string_util.h"
#include "../../utils/memory/memory.h"

// Static operands
struct operand ASM_PRINTF       = {OPD_LABEL, {"printf"}, -1, 0, false};
struct operand ASM_SCANF        = {OPD_LABEL, {"scanf"}, -1, 0, false};

struct operand ASM_RBP          = {OPD_REGISTER, {"rbp"}, -1, 0, false};
struct operand ASM_RSP          = {OPD_REGISTER, {"rsp"}, -1, 0, false};
struct operand ASM_RAX          = {OPD_REGISTER, {"rax"}, -1, 0, false};
struct operand ASM_RBX          = {OPD_REGISTER, {"rbx"}, -1, 0, false};

struct operand ASM_RCX          = {OPD_REGISTER, {"rcx"}, -1, 0, false};
struct operand ASM_RDX          = {OPD_REGISTER, {"rdx"}, -1, 0, false};
struct operand ASM_RSI          = {OPD_REGISTER, {"rsi"}, -1, 0, false};
struct operand ASM_RDI          = {OPD_REGISTER, {"rdi"}, -1, 0, false};

struct operand ASM_R8           = {OPD_REGISTER, {"r8"}, -1, 0, false};
struct operand ASM_R9           = {OPD_REGISTER, {"r9"}, -1, 0, false};
struct operand ASM_R10          = {OPD_REGISTER, {"r10"}, -1, 0, false};
struct operand ASM_R11          = {OPD_REGISTER, {"r11"}, -1, 0, false};
struct operand ASM_R12          = {OPD_REGISTER, {"r12"}, -1, 0, false};
struct operand ASM_R13          = {OPD_REGISTER, {"r13"}, -1, 0, false};
struct operand ASM_R14          = {OPD_REGISTER, {"r14"}, -1, 0, false};
struct operand ASM_R15          = {OPD_REGISTER, {"r15"}, -1, 0, false};

struct operand ASM_HEAP         = {OPD_LABEL, {"HEAP"}, -1, 0, false};
struct operand ASM_STATIC_LINK  = {OPD_LABEL, {"16(%rbp)"}, -1, 0, false};

struct operand ASM_XMM0          = {OPD_REGISTER, {"xmm0"}, -1, 0, false};


// Labels
int num_true;
int num_if_then;
int num_loop;
int float_loop;


operand *get_opd_reg(int num)
{
    if (num == 0)
        return &ASM_RAX;
    if (num == 1)
        return &ASM_RBX;
    if (num == 2)
        return &ASM_RCX;
    if (num == 3)
        return &ASM_RDX;
    if (num == 4)
        return &ASM_RSI;
    if (num == 5)
        return &ASM_RDI;
    if (num == 6)
        return &ASM_R8;
    if (num == 7)
        return &ASM_R9;
    if (num == 8)
        return &ASM_R10;
    if (num == 9)
        return &ASM_R11;
    if (num == 10)
        return &ASM_R12;
    if (num == 11)
        return &ASM_R13;
    if (num == 12)
        return &ASM_R14;
    if (num == 13)
        return &ASM_R15;

    printf("Could not find register %i\n", num);
    return NULL;
}

int get_reg_num(operand *opd)
{
    switch (opd->kind)
    {
        case OPD_HEAP:
            return get_reg_num(opd->val.heap_offset_reg);
        case OPD_STACK:
            return get_reg_num(opd->val.stack_reg.stack_reg);
        case OPD_REGISTER:
            if (opd == &ASM_RAX)
                return 0;
            if (opd == &ASM_RBX)
                return 1;
            if (opd == &ASM_RCX)
                return 2;
            if (opd == &ASM_RDX)
                return 3;
            if (opd == &ASM_RSI)
                return 4;
            if (opd == &ASM_RDI)
                return 5;
            if (opd == &ASM_R8)
                return 6;
            if (opd == &ASM_R9)
                return 7;
            if (opd == &ASM_R10)
                return 8;
            if (opd == &ASM_R11)
                return 9;
            if (opd == &ASM_R12)
                return 10;
            if (opd == &ASM_R13)
                return 11;
            if (opd == &ASM_R14)
                return 12;
            if (opd == &ASM_R15)
                return 13;
            break;
        case OPD_TMP:
            return opd->tmp_id;
        case OPD_LABEL:
        case OPD_TMP_SPILL:
        case OPD_INT:
        case OPD_FLOAT:
        case OPD_STRING:
            break;
    }
    return -1;
}

operand *get_reg(operand *opd)
{
    switch (opd->kind)
    {
        case OPD_HEAP:
            return get_reg(opd->val.heap_offset_reg);
        case OPD_STACK:
            return get_reg(opd->val.stack_reg.stack_reg);
        case OPD_REGISTER:
        case OPD_TMP:
        case OPD_LABEL:
        case OPD_TMP_SPILL:
        case OPD_INT:
        case OPD_FLOAT:
        case OPD_STRING:
            return opd;
            break;
    }
    return NULL;
}


void asm_make_label(abstract_asm **cur, abstract_asm **start, int kind,
                    char *label, char *dbg_code)
{
    asm_make_list_elem(&(*cur), &(*start));
    (*cur)->instruction = I_LABEL;
    (*cur)->val.label.id = label;
    (*cur)->val.label.kind = kind;
    (*cur)->next = NULL;
    (*cur)->debug_code = dbg_code;
    (*cur)->live_out = (*cur)->live_in = NULL;
    (*cur)->def = (*cur)->use = NULL;
    (*cur)->num_opds = 0;
    (*cur)->num_opds = 0;
}

void asm_make_inst(abstract_asm **cur, abstract_asm **start, int kind,
                   char *dbg_code)
{
    asm_make_list_elem(&(*cur), &(*start));
    (*cur)->instruction = kind;
    (*cur)->next = NULL;
    (*cur)->debug_code = dbg_code;
    (*cur)->live_out = (*cur)->live_in = NULL;
    (*cur)->def = (*cur)->use = NULL;
    (*cur)->num_opds = 0;
}


void asm_make_unary_inst(abstract_asm **cur, abstract_asm **start, int kind,
                         operand *opd, char *dbg_code)
{
    asm_make_list_elem(&(*cur), &(*start));
    (*cur)->instruction = kind;
    (*cur)->val.unary.opd = opd;
    (*cur)->next = NULL;
    (*cur)->debug_code = dbg_code;
    (*cur)->live_out = (*cur)->live_in = NULL;
    (*cur)->def = (*cur)->use = NULL;
    (*cur)->num_opds = 1;
}

void asm_make_bin_inst(abstract_asm **cur, abstract_asm **start, int kind,
                       operand *l, operand *r, char *dbg_code)
{
    asm_make_list_elem(&(*cur), &(*start));
    (*cur)->instruction = kind;
    (*cur)->val.bin.left = l;
    (*cur)->val.bin.right = r;
    (*cur)->next = NULL;
    (*cur)->debug_code = dbg_code;
    (*cur)->live_out = (*cur)->live_in = NULL;
    (*cur)->def = (*cur)->use = NULL;
    (*cur)->num_opds = 2;
}

void asm_combine_inst(abstract_asm *other, abstract_asm **cur,
                      abstract_asm **start)
{
    if ((*cur) == NULL)
    {
        (*cur) = (*start) = other;
    }
    else
    {
        other->previous = (*cur);
        (*cur) = (*cur)->next = other;
    }
}

void asm_combine(abstract_asm *other, abstract_asm **cur, abstract_asm **start)
{
    abstract_asm *next = NULL;
    if ((*cur) != NULL)
    {
        next = (*cur)->next;
        (*cur)->next = other;
    }
    if (other != NULL)
    {
        other->previous = (*cur);
        if ((*cur) == NULL)
        {
            (*start) = other;
        }
        (*cur) = other;

        for (;(*cur)->next; (*cur)=(*cur)->next);

        (*cur)->next = next;
        if (next != NULL)
        {
            next->previous = (*cur);
        }
    }
    else if ((*cur) != NULL)
    {
        (*cur)->next = next;
    }
}

void asm_make_list_elem(abstract_asm **cur, abstract_asm **start)
{
    if ((*cur) == NULL)
    {
        (*cur) = NEW(abstract_asm);
        (*start) = (*cur);
    }
    else
    {
        (*cur)->next = NEW(abstract_asm);
        (*cur)->next->previous = (*cur);
        (*cur) = (*cur)->next;
    }
}

operand *asm_make_string(char *string)
{
    operand *o = NEW(operand);
    o->kind = OPD_STRING;
    o->val.id = string;
    o->is_allocated = false;

    return o;
}

operand *make_opd_label(char *id)
{
    operand *o = NEW(operand);
    o->kind = OPD_LABEL;
    o->val.id = id;
    o->is_allocated = false;

    return o;
}

operand *asm_make_tmp()
{
    operand *o = NEW(operand);
    o->kind = OPD_TMP;
    o->tmp_id = num_tmp++;
    o->val.tmp.spilled = NULL;
    o->is_allocated = false;

    return o;
}

operand *make_spilled_tmp()
{
    operand *o = NEW(operand);
    o->kind = OPD_TMP_SPILL;
    o->offset = -1;
    o->is_allocated = false;

    return o;
}

operand *asm_make_int(int var)
{
    operand *o = NEW(operand);
    o->kind = OPD_INT;
    o->val.value = var;
    o->is_allocated = false;
    return o;
}

operand *asm_make_float(float var)
{
    operand *o = NEW(operand);
    o->kind = OPD_FLOAT;
    o->val.f_value = var;
    o->is_allocated = false;
    return o;
}

void replace_tmp_op(operand **op, operand *r_op)
{
    operand *new_op;

    switch ((*op)->kind)
    {
        case OPD_TMP_SPILL:
        case OPD_INT:
        case OPD_FLOAT:
        case OPD_STRING:
            break;
        case OPD_TMP:
            (*op) = r_op;
            break;
        case OPD_LABEL:
            break;
        case OPD_REGISTER:
            break;
        case OPD_HEAP:
            new_op = NEW(operand);
            new_op->kind = OPD_HEAP;
            new_op->val.heap_offset_reg = r_op;
            (*op) = new_op;
            break;
        case OPD_STACK:
            new_op = NEW(operand);
            new_op->kind = OPD_STACK;
            new_op->val.stack_reg.stack_reg = r_op;
            new_op->val.stack_reg.offset = (*op)->val.stack_reg.offset;
            (*op) = new_op;
            break;
    }
}

char *asm_make_unique_true_label()
{
    char label[LABEL_SIZE];
    sprintf(label, "true%i:", num_true);
    num_true++;
    return cpy_str(label);
}

char *asm_make_unique_end_label()
{
    char label[LABEL_SIZE];
    sprintf(label, "end%i:", num_if_then);
    num_if_then++;
    return cpy_str(label);
}

char *asm_make_unique_loop_label()
{
    char label[LABEL_SIZE];
    sprintf(label, "loop%i:", num_loop);
    num_loop++;
    return cpy_str(label);
}

char *asm_make_unique_float_name()
{
    char label[LABEL_SIZE];
    sprintf(label, ".LC%i", float_loop);
    float_loop++;
    return cpy_str(label);
}

operand *make_op_stack_loc(operand *reg, int offset)
{
    operand *op = NEW(operand);
    char t[25];
    if (reg == &ASM_RBP)
    {
        op->kind = OPD_LABEL;
        sprintf(t, "%d(%%rbp)", -WORD_SIZE * offset);
        op->val.id = cpy_str(t);
        op->offset = offset;
        return op;
    }
    op->kind = OPD_STACK;
    op->val.stack_reg.stack_reg = reg;
    op->val.stack_reg.offset = offset;
    op->offset = offset;
    return op;
}

operand *make_opd_heap_loc(operand *off)
{
    operand *op = NEW(operand);
    op->kind = OPD_HEAP;
    op->val.heap_offset_reg = off;
    return op;
}

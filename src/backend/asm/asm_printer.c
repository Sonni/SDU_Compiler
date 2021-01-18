#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "asm_printer.h"
#include "../../utils/string_util.h"
#include "../../utils/definitions.h"


void print_indent_and_DBC(int indent, char *c1, char *c2)
{
    printf("%s%*s# %s", c1, indent, "", c2);
}

char *print_operator(operand *o1, operand *o2)
{
    char print_res[500];

    sprintf(print_res, "%s, %s", print_operand(o1), print_operand(o2));

    return cpy_str(print_res);
}

char *print_operand(operand *o)
{
    char print_res[500];

    switch (o->kind)
    {
        case OPD_INT:
            sprintf(print_res, "$%i", o->val.value);
            break;
        case OPD_FLOAT:
            sprintf(print_res, "$%f", o->val.f_value);
            break;
        case OPD_TMP:
            sprintf(print_res, "tmp%d", o->tmp_id);
            break;
        case OPD_LABEL:
            sprintf(print_res, "%s", o->val.id);
            break;
        case OPD_REGISTER:
            sprintf(print_res, "%%%s", o->val.id);
            break;
        case OPD_HEAP:
            sprintf(print_res, "HEAP(,%s,%i)",
                    print_operand(o->val.heap_offset_reg), WORD_SIZE);
            break;
        case OPD_STACK:
            sprintf(print_res, "-%d(%s)", WORD_SIZE * o->val.stack_reg.offset,
                    print_operand(o->val.stack_reg.stack_reg));
            break;
        case OPD_TMP_SPILL:
            sprintf(print_res, "-%d(%%rbp)",
                    WORD_SIZE + WORD_SIZE * o->offset);
            break;
        case OPD_STRING:
            sprintf(print_res, "$%s", o->val.id);
            break;
    }
    return cpy_str(print_res);
}

void print_abstract_asm(abstract_asm *a_asm)
{
    abstract_asm *next = a_asm;
    char print_res[500];
    char tmp_label[500];
    bool skip;

    while (next != NULL)
    {
        skip = false;
        switch (next->instruction)
        {
            case I_CALL_START:
            case I_CALL_END:
                skip = true;
                break;
            case I_CLTD:
                sprintf(print_res, "    cltd");
                break;
            case I_ADD:
                sprintf(print_res, "    addq %s",
                        print_operator(next->val.bin.left,
                                       next->val.bin.right));
                break;
            case I_SUB:
                sprintf(print_res, "    subq %s",
                        print_operator(next->val.bin.left,
                                       next->val.bin.right));
                break;
            case I_MUL:
                sprintf(print_res, "    imulq %s",
                        print_operator(next->val.bin.left,
                                       next->val.bin.right));
                break;
            case I_DIV:
                sprintf(print_res, "    idivq %s",
                        print_operand(next->val.bin.left));
                break;
            case I_INC:
                sprintf(print_res, "    inc %s",
                        print_operand(next->val.unary.opd));
                break;
            case I_DEC:
                sprintf(print_res, "    dec %s",
                        print_operand(next->val.unary.opd));
                break;
            case I_JMP:
                sprintf(tmp_label, "%s", next->val.unary.opd->val.id);
                tmp_label[strlen(tmp_label) - 1] = 0;
                sprintf(print_res, "    jmp %s", tmp_label);
                break;
            case I_MOV:
                sprintf(print_res, "    movq %s",
                        print_operator(next->val.bin.left,
                                       next->val.bin.right));
                break;
            case I_AND:
                sprintf(print_res, "    and %s",
                        print_operator(next->val.bin.left,
                                       next->val.bin.right));
                break;
            case I_OR:
                sprintf(print_res, "    or %s",
                        print_operator(next->val.bin.left,
                                       next->val.bin.right));
                break;
            case I_XOR:
                sprintf(print_res, "    xor %s",
                        print_operator(next->val.bin.left,
                                       next->val.bin.right));
                break;
            case I_PUSH:
                sprintf(print_res, "    pushq %s",
                        print_operand(next->val.unary.opd));
                break;
            case I_POP:
                sprintf(print_res, "    popq %s",
                        print_operand(next->val.unary.opd));
                break;
            case I_RET:
                sprintf(print_res, "    ret");
                break;
            case I_CALL:
                sprintf(print_res, "    call %s",
                        print_operand(next->val.unary.opd));
                break;
            case I_LSHIFT:
                sprintf(print_res, "    sh %s",
                        print_operator(next->val.bin.left,
                                       next->val.bin.right));
                break;
            case I_RSHIFT:
                sprintf(print_res, "    test");
                break;
            case I_ARSHIFT:
                sprintf(print_res, "    test");
                break;
            case I_JE:
                sprintf(tmp_label, "%s", next->val.unary.opd->val.id);
                tmp_label[strlen(tmp_label) - 1] = 0;
                sprintf(print_res, "    je %s", tmp_label);
                break;
            case I_JNE:
                sprintf(tmp_label, "%s", next->val.unary.opd->val.id);
                tmp_label[strlen(tmp_label) - 1] = 0;
                sprintf(print_res, "    jne %s", tmp_label);
                break;
            case I_JG:
                sprintf(tmp_label, "%s", next->val.unary.opd->val.id);
                tmp_label[strlen(tmp_label) - 1] = 0;
                sprintf(print_res, "    jg %s", tmp_label);
                break;
            case I_JGE:
                sprintf(tmp_label, "%s", next->val.unary.opd->val.id);
                tmp_label[strlen(tmp_label) - 1] = 0;
                sprintf(print_res, "    jge %s", tmp_label);
                break;
            case I_JL:
                sprintf(tmp_label, "%s", next->val.unary.opd->val.id);
                tmp_label[strlen(tmp_label) - 1] = 0;
                sprintf(print_res, "    jl %s", tmp_label);
                break;
            case I_JLE:
                sprintf(tmp_label, "%s", next->val.unary.opd->val.id);
                tmp_label[strlen(tmp_label) - 1] = 0;
                sprintf(print_res, "    jle %s", tmp_label);
                break;
            case I_LABEL:
                sprintf(print_res, "%s", next->val.label.id);
                break;
            case I_CMP:
                sprintf(print_res, "    cmp %s",
                        print_operator(next->val.bin.left,
                                       next->val.bin.right));
                break;
            case I_NEG:
                sprintf(print_res, "    neg %s",
                        print_operand(next->val.unary.opd));
                break;
            case I_INT:
                sprintf(print_res, "    int %s",
                        print_operand(next->val.unary.opd));
                break;
            case I_JB:
                sprintf(tmp_label, "%s", next->val.unary.opd->val.id);
                tmp_label[strlen(tmp_label) - 1] = 0;
                sprintf(print_res, "    jnb %s", tmp_label);
                break;
            case I_BT:
                sprintf(print_res, "    btq %s",
                        print_operator(next->val.bin.left,
                                       next->val.bin.right));
                break;
            case I_BTS:
                sprintf(print_res, "    btsq %s",
                        print_operator(next->val.bin.left,
                                       next->val.bin.right));
                break;
            case I_CVTSS2SD:
                sprintf(print_res, "    cvtss2sd %s",
                        print_operator(next->val.bin.left,
                                       next->val.bin.right));
                break;

        }
        if (!skip)
        {
            if (next->debug_code != NULL &&
                strcmp("", next->debug_code) != 0 && print_debug)
            {
                print_indent_and_DBC(ASM_INDENT - strlen(print_res), print_res,
                                     next->debug_code);
            }
            else
            {
                printf("%s", print_res);
            }
            printf("\n");
        }

        next = next->next;
    }
}

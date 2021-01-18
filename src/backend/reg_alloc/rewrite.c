#include "rewrite.h"
#include <stdlib.h>
#include "liveness.h"
#include <string.h>
#include "../../utils/string_util.h"

extern vector *const_strings;
extern abstract_asm *offset_index_aac;
extern int num_body_vars;
extern vector *floats;


/* Floats in ASM.

 For each float, write:
    .LCPI0_I:
            .long   X

 Where 'I' increments after each float and X can be printed by
    float f = 41.14420;                        // The float
    int ff = *(unsigned int*)&f;               // Magic to convert float->hex->int.
    printf(".LCPI0_I:   .long   %d\n", ff);    // Print the

 */


abstract_asm *rewrite_header()
{
    abstract_asm *cur = NULL, *start = NULL;

    while (vector_size(floats) != 0)
    {
        float f = *(float*) vector_get_element(floats, vector_size(floats)-1);
        char buf[500];
        sprintf(buf, "%s:    .long    %d",
                (char*) vector_get_element(floats, vector_size(floats)-2),
                *(unsigned int*)&f);

        vector_pop(floats);
        vector_pop(floats);

        asm_make_label(&cur, &start, LABEL_SAVE, cpy_str(buf), "");
    }

    asm_make_label(&cur, &start, LABEL_SAVE, ".section .data", "");
    asm_make_label(&cur, &start, LABEL_SAVE,
                   "    PRINT_INT:   .string \"%d\\n\"", "");
    asm_make_label(&cur, &start, LABEL_SAVE,
                   "    PRINT_FLOAT:   .string \"%f\\n\"", "");
    asm_make_label(&cur, &start, LABEL_SAVE,
                   "    PRINT_TRUE:  .string \"True\\n\"", "");
    asm_make_label(&cur, &start, LABEL_SAVE,
                   "    PRINT_FALSE: .string \"False\\n\"", "");
    asm_make_label(&cur, &start, LABEL_SAVE, "    SCAN_INT:    .string \"%d\"",
                   "");
    while (vector_size(const_strings) != 0)
    {
        char buf[500];
        sprintf(buf, "    %s: .string \"%s\"",
                (char*) vector_get_element(const_strings,
                                           vector_size(const_strings)-1),
                (char*) vector_get_element(const_strings,
                                           vector_size(const_strings)-2));
        vector_pop(const_strings);
        vector_pop(const_strings);

        asm_make_label(&cur, &start, LABEL_SAVE, cpy_str(buf), "");

    }
    char *txt_buf = malloc(sizeof(char) * 50);
    sprintf(txt_buf, "    HEAP:        .space  %d", HEAP_SIZE * WORD_SIZE);
    asm_make_label(&cur, &start, LABEL_SAVE, txt_buf, "");
    asm_make_label(&cur, &start, LABEL_SAVE, ".section .text", "");
    asm_make_label(&cur, &start, LABEL_SAVE, ".globl main", "");

    return start;
}

abstract_asm *rewrite_footer()
{
    abstract_asm *cur = NULL, *start = NULL;

    asm_make_label(&cur, &start, LABEL_SAVE, "", "");
    asm_make_label(&cur, &start, LABEL_SAVE,
                   ".credits \"++C, a Shere Khan Compiler made "
                   "by Sonni Hedelund Jensen.\"",
                   "");
    asm_make_label(&cur, &start, LABEL_SAVE, "", "");

    return start;
}

void rewrite_operand(operand **opd, int *color)
{
    switch ((*opd)->kind)
    {
        case OPD_HEAP:
            rewrite_operand(&(*opd)->val.heap_offset_reg, color);
            break;
        case OPD_STACK:
            rewrite_operand(&(*opd)->val.stack_reg.stack_reg, color);
            break;
        case OPD_REGISTER:
            break;
        case OPD_TMP:
            if (color[(*opd)->tmp_id] == -1)
            {
                fprintf(stderr, "Color = -1!\n");
                return;
            }
            (*opd) = get_opd_reg(color[(*opd)->tmp_id]);
            break;
        case OPD_LABEL:
        case OPD_TMP_SPILL:
        case OPD_INT:
        case OPD_FLOAT:
        case OPD_STRING:
            break;
    }
}

int *get_used_regs(abstract_asm *program, int *color, char *label)
{
    int *regs = malloc(sizeof(int) * NUM_REG);


    for (int i = 0; i < NUM_REG; i++)
    {
        regs[i] = 0;
    }

    if (strcmp(label, "printf") == 0 || strcmp(label, "scanf") == 0)
    {
        regs[get_reg_num(&ASM_RAX)] = 1;
        regs[get_reg_num(&ASM_RCX)] = 1;
        regs[get_reg_num(&ASM_RDX)] = 1;
        regs[get_reg_num(&ASM_RSI)] = 1;
        regs[get_reg_num(&ASM_RDI)] = 1;
        regs[get_reg_num(&ASM_R8)] = 1;
        regs[get_reg_num(&ASM_R9)] = 1;
        regs[get_reg_num(&ASM_R10)] = 1;
        regs[get_reg_num(&ASM_R11)] = 1;
        return regs;
    }


    abstract_asm *code = program;
    while (code != NULL)
    {
        if (code->instruction == I_LABEL &&
            code->val.label.kind == LABEL_FUNC_BEGIN)
        {
            char new_l[500];
            sprintf(new_l, "%s:", label);
            if (strcmp(new_l, code->val.label.id) == 0)
            {
                while (code != NULL && code->instruction != I_RET)
                {
                    for (int i = 0; i < num_tmp; i++)
                    {
                        if (code->def != NULL && bit_test(code->def, i))
                        {
                            regs[color[i]] = 1;
                        }
                    }

                    code = code->next;
                }
                return regs;
            }
        }
        code = code->next;
    }
    printf("Could not find function %s\n", label);
    return regs;
}

abstract_asm *rewrite_call(abstract_asm *program, int kind, int *color,
                           abstract_asm *call)
{
    abstract_asm *aac_cur = NULL, *aac_start = NULL;

    int *used_regs = get_used_regs(program, color,
                                   call->val.unary.opd->val.id);

    int regs[NUM_REG];

    for (int i = 0; i < NUM_REG; i++)
    {
        regs[i] = 1;
    }

  


    // TODO fix color[i] = -1
    if (kind == I_PUSH)
    {
        
                // This fixes bug with templated classes
        asm_make_unary_inst(&aac_cur, &aac_start, I_PUSH, &ASM_RBX, "");
    asm_make_unary_inst(&aac_cur, &aac_start, I_PUSH, &ASM_RCX, "");
    asm_make_unary_inst(&aac_cur, &aac_start, I_PUSH, &ASM_RDX, "");
    asm_make_unary_inst(&aac_cur, &aac_start, I_PUSH, &ASM_RSI, "");
    asm_make_unary_inst(&aac_cur, &aac_start, I_PUSH, &ASM_RDI, "");
    asm_make_unary_inst(&aac_cur, &aac_start, I_PUSH, &ASM_R9, "");
    asm_make_unary_inst(&aac_cur, &aac_start, I_PUSH, &ASM_R8, "");
        for (int i = 0; i < num_tmp; i++)
        {
            if (bit_test(call->live_in, i) && used_regs[color[i]] &&
                regs[color[i]] && color[i] != -1)
            {
                regs[color[i]] = 0;
                asm_make_unary_inst(&aac_cur, &aac_start, I_PUSH,
                                    get_opd_reg(color[i]),
                                    "Is live, so must be saved");

            }
        }
          
    }
    else
    {
        for (int i = num_tmp - 1; i >= 0; i--)
        {
            if (bit_test(call->live_in, i) && used_regs[color[i]] &&
                regs[color[i]] && color[i] != -1)
            {
                regs[color[i]] = 0;
                asm_make_unary_inst(&aac_cur, &aac_start, I_POP,
                                    get_opd_reg(color[i]),
                                    "Is live, so must be saved");

            }
        }

        // This and above fixes bug with templated classes
         asm_make_unary_inst(&aac_cur, &aac_start, I_POP, &ASM_R8, "");
         asm_make_unary_inst(&aac_cur, &aac_start, I_POP, &ASM_R9, "");
         asm_make_unary_inst(&aac_cur, &aac_start, I_POP, &ASM_RDI, "");
        asm_make_unary_inst(&aac_cur, &aac_start, I_POP, &ASM_RSI, "");
        asm_make_unary_inst(&aac_cur, &aac_start, I_POP, &ASM_RDX, "");
        asm_make_unary_inst(&aac_cur, &aac_start, I_POP, &ASM_RCX, "");
        asm_make_unary_inst(&aac_cur, &aac_start, I_POP, &ASM_RBX, "");
    }

   

    return aac_start;
}

abstract_asm *rewrite_func_head(int esp_offset)
{
    abstract_asm *aac_cur = NULL, *aac_start = NULL;

    asm_make_unary_inst(&aac_cur, &aac_start, I_PUSH, &ASM_RBP,
                        "Push old base pointer on stack");
    asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, &ASM_RSP, &ASM_RBP,
                      "stack pointer = base pointer");
    asm_make_bin_inst(&aac_cur, &aac_start, I_SUB, asm_make_int(esp_offset),
                      &ASM_RSP, "Allocate stack frame");

    return aac_start;
}

abstract_asm *rewrite_func_tail()
{
    abstract_asm *aac_cur = NULL, *aac_start = NULL;

    asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, &ASM_RBP, &ASM_RSP,
                      "Restore stack pointer");
    asm_make_unary_inst(&aac_cur, &aac_start, I_POP, &ASM_RBP,
                        "Restore base pointer");

    return aac_start;
}

abstract_asm *rewrite_body_offset(int esp_offset)
{
    abstract_asm *aac_cur = NULL, *aac_start = NULL;

    asm_make_bin_inst(&aac_cur, &aac_start, I_SUB, asm_make_int(esp_offset),
                      &ASM_RSP, "Allocate stack frame");

    return aac_start;
}

abstract_asm *rewrite_program(abstract_asm *program, int *color)
{
    abstract_asm *cur = program;
    abstract_asm *aac_start = NULL;
    abstract_asm *call_begin, *call;
    abstract_asm *func_begin;
    int esp_offset = 0;


    asm_combine(rewrite_header(), &cur, &aac_start);
    while (cur != NULL)
    {
        switch (cur->instruction)
        {
            case I_CALL_START:
                call_begin = cur;
                break;
            case I_CALL_END:
                asm_combine(rewrite_call(program, I_POP, color, call), &cur,
                            &aac_start);
                break;
            case I_LABEL:
                if (cur->val.label.kind == LABEL_FUNC_BEGIN)
                {
                    esp_offset = cur->val.label.num_param * WORD_SIZE;
                    func_begin = cur;
                }
                else if (cur->val.label.kind == LABEL_FUNC_END)
                {
                    // Collect esp offset before assembling header
                    asm_combine(rewrite_func_head(esp_offset), &func_begin,
                                &aac_start);

                    asm_combine(rewrite_func_tail(), &cur, &aac_start);
                }
                break;
            case I_MUL:
            case I_MOV:
            case I_AND:
            case I_OR:
            case I_SUB:
            case I_XOR:
            case I_CMP:
            case I_DIV:
            case I_ADD:
                break;
            case I_PUSH:
            case I_POP:
            case I_RET:
                break;
            case I_CALL:
                //  All live in variable must be pushed to stack,
                // they can be overwritten
                call = cur;
                asm_combine(rewrite_call(program, I_PUSH, color, cur),
                            &call_begin, &aac_start);
                break;
            case I_LSHIFT:
            case I_RSHIFT:
            case I_ARSHIFT:
                break;
            case I_INT:
            case I_JE:
            case I_JNE:
            case I_JG:
            case I_JGE:
            case I_JL:
            case I_JLE:
            case I_JMP:
            case I_CLTD:
            case I_JB:
                break;
            case I_INC:
            case I_DEC:
            case I_NEG:
            case I_BT:
            case I_BTS:
            case I_CVTSS2SD:
                break;
        }


        if (cur->num_opds == 1)
        {
            rewrite_operand(&cur->val.unary.opd, color);
        }
        else if (cur->num_opds == 2)
        {
            rewrite_operand(&cur->val.bin.left, color);
            rewrite_operand(&cur->val.bin.right, color);
        }
        cur = cur->next;
    }

    asm_combine(rewrite_footer(), &cur, &aac_start);


    asm_combine(rewrite_body_offset(num_body_vars * WORD_SIZE),
                &offset_index_aac, &aac_start);


    return program->next;
}

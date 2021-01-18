#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "asm_gen.h"
#include "../../frontend/symbol/symbol.h"
#include "../../utils/memory/memory.h"
#include "../../utils/vector.h"
#include "../../utils/string_def.h"
#include "../../utils/string_util.h"
#include "garbage_collector.h"

extern int num_body_vars;
extern vector *const_strings;
extern vector *floats;
vector *func_ids;
abstract_asm *func_start, *func_cur;
bool collect_body_var;
bool in_class;
vector *classes;
vector *opd_stack;
vector *switch_vec;

int floatOffset;


void gen_fail(int return_val, char *label, abstract_asm **cur,
              abstract_asm **start)
{
    asm_make_label(cur, start, LABEL_NOM, label, "");
    asm_make_bin_inst(cur, start, I_MOV, asm_make_int(1), &ASM_RAX,
                      "1 in EAX = exit with failure");
    asm_make_bin_inst(cur, start, I_MOV, asm_make_int(return_val), &ASM_RBX,
                      "Mov return code in EBX");
    asm_make_unary_inst(cur, start, I_INT, make_opd_label("$0x80"),
                        "Kernel interrupt");
}

abstract_asm *gen_start()
{
    abstract_asm *cur = NULL, *start = NULL;

    asm_make_label(&cur, &start, LABEL_SAVE, "TMP, REMOVE IN REWRITE", "");

    func_cur = cur;
    func_start = start;

    asm_make_label(&cur, &start, LABEL_SAVE, "main:", "");


    asm_make_unary_inst(&cur, &start, I_PUSH, &ASM_RBP, "Push rbp on stack");
    asm_make_bin_inst(&cur, &start, I_MOV, &ASM_RSP, &ASM_RBP,
                      "Set base pointer = stack pointer");
    offset_index_aac = cur;

    return start;
}

abstract_asm *gen_fail_points()
{
    abstract_asm *aac_cur = NULL, *aac_start = NULL;
    gen_fail(2, ARR_OUT_OF_BOUNDS, &aac_cur, &aac_start);
    gen_fail(3, DIV_BY_0, &aac_cur, &aac_start);
    gen_fail(4, POSI_ARG_ARR_ALLOC, &aac_cur, &aac_start);
    gen_fail(5, USE_OF_UNINIT_OR_NULL, &aac_cur, &aac_start);
    gen_fail(6, OUT_OF_MEM, &aac_cur, &aac_start);
    return aac_start;
}

abstract_asm *gen_end()
{
    abstract_asm *aac_cur = NULL, *aac_start = NULL;

    asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, &ASM_RBP, &ASM_RSP,
                      "Restore stack pointer");
    asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, asm_make_int(0), &ASM_RAX,
                      "0 (success) in RAX as return value");
    asm_make_inst(&aac_cur, &aac_start, I_RET, "End program");

    if (runtime_check)
    {
        asm_combine(gen_fail_points(), &aac_cur, &aac_start);
    }

    return aac_start;
}

abstract_asm *asm_gen(body *b)
{
    collect_body_var = true;
    in_class = false;
    const_strings = vector_init();
    floats = vector_init();
    func_ids = vector_init();
    classes = vector_init();
    opd_stack = vector_init();
    switch_vec = vector_init();
    abstract_asm *aac_cur = NULL, *aac_start = NULL;

    asm_combine(gen_start(), &aac_cur, &aac_start);

    asm_combine(asm_body(b), &aac_cur, &aac_start);

    if (gb_collect)
    {
        asm_combine(asm_dec_reference(), &func_cur, &aac_start);
        asm_combine(asm_find_free_loc(), &func_cur, &aac_start);
    }
    asm_combine(gen_end(), &aac_cur, &aac_start);
    return aac_start;
}

abstract_asm *asm_function(function *f)
{
    abstract_asm *aac_cur = NULL, *aac_start = NULL;

    char *func_end = malloc(sizeof(char) * (strlen(f->head->new_name) + 100));
    sprintf(func_end, "end%s:", f->head->new_name);

    int num_vars = get_local_vars(f->body->dl, 1);
    char *func_name = malloc(sizeof(char) * (strlen(f->head->new_name) + 100));
    sprintf(func_name, "%s:", f->head->new_name);


    asm_make_label(&aac_cur, &aac_start, LABEL_FUNC_BEGIN, func_name, "");
    aac_cur->val.label.num_param = num_vars;

    vector_push(func_ids, func_end);
    asm_combine(asm_head(f->head), &aac_cur, &aac_start);

    asm_combine(asm_body(f->body), &aac_cur, &aac_start);
    vector_pop(func_ids);


    asm_make_label(&aac_cur, &aac_start, LABEL_FUNC_END, func_end, "");

    asm_make_inst(&aac_cur, &aac_start, I_RET, "");

    return aac_start;
}

abstract_asm *asm_body(body *b)
{
    abstract_asm *aac_cur = NULL, *aac_start = NULL;

    if (collect_body_var)
    {
        num_body_vars = get_local_vars(b->dl, 1);
        collect_body_var = false;
        asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, asm_make_int(1),
                          &ASM_HEAP, "Init HEAP to 1");
    }

    asm_combine(asm_decl_list(b->dl), &aac_cur, &aac_start);

    if (runtime_check || gb_collect)
    {
        asm_combine(zero_init_local_vars(b->dl), &aac_cur, &aac_start);
    }

    asm_combine(asm_statement_list(b->sl), &aac_cur, &aac_start);


    return aac_start;
}

abstract_asm *asm_head(head *h)
{
    abstract_asm *aac_cur = NULL, *aac_start = NULL;


    asm_combine(asm_par_decl_list(h->pdl), &aac_cur, &aac_start);

    return aac_start;
}

abstract_asm *asm_type(type *t)
{
    abstract_asm *aac_cur = NULL, *aac_start = NULL;

    if (t->kind == TYPE_CLASS)
    {
        in_class = true;
        vector_push(classes, t->val._class.dl->st);
        asm_combine(asm_decl_list(t->val._class.dl), &aac_cur, &aac_start);
        vector_pop(classes);
        in_class = false;
    }

    return aac_start;
}

abstract_asm *asm_var_type(var_type *vt)
{
    abstract_asm *aac_cur = NULL, *aac_start = NULL;

    asm_combine(asm_type(vt->type), &aac_cur, &aac_start);

    return aac_start;
}

abstract_asm *asm_par_decl_list(par_decl_list *pdl)
{
    abstract_asm *aac_cur = NULL, *aac_start = NULL;

    switch (pdl->kind)
    {
        case PDL_VDL:
            asm_combine(asm_var_decl_list(pdl->vdl, 1), &aac_cur, &aac_start);
            return aac_start;
        case PDL_EMPTY:
            break;
    }
    return aac_start;
}

abstract_asm *asm_var_decl_list(var_decl_list *vdl, int offset)
{
    abstract_asm *aac_cur = NULL, *aac_start = NULL;
    symbol *s = get_symbol(vdl->vt->st, vdl->vt->id);
    s->offset = -2 - offset;

    switch (vdl->kind)
    {
        case VDL_VT_VDL:
            asm_combine(asm_var_type(vdl->vt), &aac_cur, &aac_start);
            asm_combine(asm_var_decl_list(vdl->vdl, offset + 1), &aac_cur,
                        &aac_start);
            break;
        case VDL_VT:
            break;
    }

    return aac_start;

}

abstract_asm *asm_statement_list(statement_list *sl)
{
    abstract_asm *aac_cur = NULL, *aac_start = NULL;

    /*if (sl != NULL)
    {
        if (sl->sl != NULL)
        {
            asm_combine(asm_statement_list(sl->sl), &aac_cur, &aac_start);
        }
        asm_combine(asm_statement(sl->stm), &aac_cur, &aac_start);
    }*/

    switch (sl->kind)
    {
        case SL_STM:

            asm_combine(asm_statement(sl->stm), &aac_cur, &aac_start);

            break;
        case SL_STM_SL:

            asm_combine(asm_statement_list(sl->sl), &aac_cur, &aac_start);
            asm_combine(asm_statement(sl->stm), &aac_cur, &aac_start);

            break;
    }


    return aac_start;
}

operand *arr_offset;

abstract_asm *asm_statement(statement *stm)
{
    abstract_asm *aac_cur = NULL, *aac_start = NULL;

    operand *opd, *opd2, *var, *end_label, *else_label, *loop_label, *length;
    operand *print_other;
    int size;
    switch (stm->kind)
    {
        case STM_RETURN:
            asm_combine(asm_exp(stm->val.exp), &aac_cur, &aac_start);
            opd = aac_cur->val.bin.right;
            asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, opd, &ASM_RAX,
                              "Move return value in RAX");

            asm_make_unary_inst(&aac_cur, &aac_start, I_JMP, make_opd_label(
                    vector_get_element(func_ids, vector_size(func_ids) - 1)),
                                "Return from function");
            break;
        case STM_WRITE:
            asm_combine(asm_exp(stm->val.exp), &aac_cur, &aac_start);

            if (stm->val.exp->sym_type->kind == SYMBOL_INT)
            {
                opd = aac_cur->val.bin.right;

                asm_combine(asm_printf(make_opd_label("$PRINT_INT"), opd,
                                       asm_make_int(0)), &aac_cur, &aac_start);
            }
            else if (stm->val.exp->sym_type->kind == SYMBOL_FLOAT)
            {
                opd = aac_cur->val.bin.right;

                asm_make_bin_inst(&aac_cur, &aac_start, I_CVTSS2SD, opd,
                                  &ASM_XMM0, "Do some magic");

                asm_combine(asm_printf(make_opd_label("$PRINT_FLOAT"), opd,
                                       asm_make_int(1)), &aac_cur, &aac_start);
            }
            else if (stm->val.exp->sym_type->kind == SYMBOL_BOOL)
            {
                opd = aac_cur->val.bin.right;

                end_label = make_opd_label(asm_make_unique_end_label());
                else_label = make_opd_label(asm_make_unique_end_label());
                print_other = make_opd_label(asm_make_unique_end_label());

                asm_make_bin_inst(&aac_cur, &aac_start, I_CMP, asm_make_int(0),
                                  opd, "Check if value is False");
                asm_make_unary_inst(&aac_cur, &aac_start, I_JE, print_other,
                                    "Jump if False");

                asm_combine(asm_printf(make_opd_label("$PRINT_TRUE"), NULL,
                                       asm_make_int(0)), &aac_cur, &aac_start);

                asm_make_unary_inst(&aac_cur, &aac_start, I_JMP, end_label,
                                    "True has been printed, skip printing False");
                asm_make_label(&aac_cur, &aac_start, LABEL_NOM,
                               print_other->val.id, "");

                asm_combine(asm_printf(make_opd_label("$PRINT_FALSE"), NULL,
                                       asm_make_int(0)), &aac_cur, &aac_start);


                asm_make_label(&aac_cur, &aac_start, LABEL_NOM,
                               end_label->val.id, "");
            }
            else if (stm->val.exp->sym_type->kind == SYMBOL_STRING)
            {
                opd = aac_cur->val.bin.right;

                asm_combine(asm_printf(opd, NULL, asm_make_int(0)), &aac_cur,
                            &aac_start);
            }
            else if (stm->val.exp->sym_type->kind == SYMBOL_ARRAY)
            {
                opd = aac_cur->val.bin.right;

                end_label = make_opd_label(asm_make_unique_end_label());
                loop_label = make_opd_label(asm_make_unique_loop_label());

                opd2 = asm_make_tmp();
                length = asm_make_tmp();
                operand *var_opd = asm_make_tmp();
                asm_make_bin_inst(&aac_cur, &aac_start, I_MOV,
                                  make_opd_heap_loc(opd), length,
                                  "Move length in reg");

                if (gb_collect)
                {
                    asm_make_bin_inst(&aac_cur, &aac_start, I_MOV,
                                      asm_make_int(3), opd2,
                                      "GB-collect is enabled, offset is 3");
                }
                else
                {
                    asm_make_bin_inst(&aac_cur, &aac_start, I_MOV,
                                      asm_make_int(1), opd2, "Offset is 0");
                }

                asm_make_label(&aac_cur, &aac_start, LABEL_NOM,
                               loop_label->val.id, "Loop start");


                asm_make_bin_inst(&aac_cur, &aac_start, I_CMP, length, opd2,
                                  "");
                asm_make_unary_inst(&aac_cur, &aac_start, I_JG, end_label, "");


                asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, opd, var_opd,
                                  "");
                asm_make_bin_inst(&aac_cur, &aac_start, I_ADD, opd2, var_opd,
                                  "");
                asm_make_bin_inst(&aac_cur, &aac_start, I_MOV,
                                  make_opd_heap_loc(var_opd), var_opd, "");
                asm_combine(asm_printf(make_opd_label("$PRINT_INT"), var_opd,
                                       asm_make_int(0)), &aac_cur, &aac_start);


                asm_make_unary_inst(&aac_cur, &aac_start, I_INC, opd2, "i++");
                asm_make_unary_inst(&aac_cur, &aac_start, I_JMP, loop_label,
                                    "Jump to start of loop");

                asm_make_label(&aac_cur, &aac_start, LABEL_NOM,
                               end_label->val.id, "");
            }

            break;
        case STM_ALLOCATE:
            length = asm_make_tmp();
            opd = asm_make_tmp();
            if (stm->val.var->sym_type->kind == SYMBOL_CLASS)
            {
                size = get_class_size(stm->val.var->sym_type->val._class.st) *
                       WORD_SIZE;
            }
            else
            {
                if (gb_collect)
                {
                    size = get_table_size(
                            stm->val.var->sym_type->val.type_p->st) +
                           WORD_SIZE * 3;
                }
                else
                {
                    size = get_table_size(
                            stm->val.var->sym_type->val.type_p->st) *
                           WORD_SIZE + WORD_SIZE + WORD_SIZE;

                }
            }

            asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, asm_make_int(size),
                              length, "Move length of record into reg");

            opd2 = asm_make_tmp();

            asm_combine(asm_variable(stm->val.var), &aac_cur, &aac_start);
            var = aac_cur->val.bin.right;

            if (gb_collect)
            {
                // Do reference counting and setting HEAP to a free location
                asm_combine(asm_garbage_collect(var, length), &aac_cur,
                            &aac_start);
            }


            asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, &ASM_HEAP, opd2,
                              "Move heap pointer to reg");
            asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, opd2, var, "");


            asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, asm_make_int(size),
                              make_opd_heap_loc(opd2),
                              "Move len of rec to heap");


            asm_make_bin_inst(&aac_cur, &aac_start, I_ADD, var, length,
                              "add heap pointer and rec len");

            // Check for out of memory
            if (runtime_check)
            {
                asm_make_bin_inst(&aac_cur, &aac_start, I_CMP,
                                  asm_make_int(HEAP_SIZE - WORD_SIZE), length,
                                  "Check for out of memory");
                asm_make_unary_inst(&aac_cur, &aac_start, I_JGE,
                                    make_opd_label(OUT_OF_MEM),
                                    "Jump to OOM label if cmp is true");
            }

            asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, length, &ASM_HEAP,
                              "Move length of rec to heap");

            // Set bits to indicate which element in record is a pointer
            if (gb_collect)
            {
                if (stm->val.var->kind != VAR_ID)
                {
                    asm_combine(asm_variable(stm->val.var->val.var_exp.var),
                                &aac_cur, &aac_start);
                    operand *parent = aac_cur->val.bin.right;

                    operand *offset = asm_make_tmp();
                    operand *bit_arr = asm_make_tmp();
                    asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, parent,
                                      offset, "");

                    asm_make_bin_inst(&aac_cur, &aac_start, I_ADD,
                                      asm_make_int(1), offset, "");

                    asm_make_bin_inst(&aac_cur, &aac_start, I_MOV,
                                      make_opd_heap_loc(offset), bit_arr, "");

                    if (stm->val.var->kind == VAR_RECORD)
                    {
                        symbol *s = get_symbol(
                                stm->val.var->sym_type->val.vdl->st,
                                stm->val.var->val.var_exp.id);
                        asm_make_bin_inst(&aac_cur, &aac_start, I_BTS,
                                          asm_make_int(s->offset), bit_arr,
                                          "");
                    }
                    else if (stm->val.var->kind == VAR_EXP)
                    {

                        asm_make_bin_inst(&aac_cur, &aac_start, I_BTS,
                                          arr_offset, bit_arr, "");
                    }
                    asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, bit_arr,
                                      make_opd_heap_loc(offset), "");
                }
                // If var is an id, then it is the first allocation,
                // thus no pointer should be set
                asm_combine(asm_set_bits(var), &aac_cur, &aac_start);
            }

            asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, var, var, "");
            break;
        case STM_ALLOCATE_OF_LENGTH:
            asm_combine(asm_exp(stm->val.assignment.exp), &aac_cur,
                        &aac_start);
            length = aac_cur->val.bin.right;
            opd = asm_make_tmp();
            // Runtime check for negative array size
            if (runtime_check)
            {
                asm_make_bin_inst(&aac_cur, &aac_start, I_CMP, asm_make_int(0),
                                  length, "Compare 0 with array size");
                asm_make_unary_inst(&aac_cur, &aac_start, I_JL,
                                    make_opd_label(POSI_ARG_ARR_ALLOC),
                                    "Jump if array size is less than 0");
            }
            asm_combine(asm_variable(stm->val.assignment.var), &aac_cur,
                        &aac_start);
            var = aac_cur->val.bin.right;

            operand *real_len = length;
            if (gb_collect)
            {
                // Do reference counting and setting HEAP to a free location
                asm_make_bin_inst(&aac_cur, &aac_start, I_ADD, asm_make_int(2),
                                  real_len, "Add space for 3 words");
                asm_combine(asm_garbage_collect(var, real_len), &aac_cur,
                            &aac_start);
            }

            asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, &ASM_HEAP, opd,
                              "Move heap pointer to reg");
            asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, opd, var, "");

            asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, real_len,
                              make_opd_heap_loc(opd),
                              "Move length of array to heap");
            asm_make_bin_inst(&aac_cur, &aac_start, I_ADD, asm_make_int(2),
                              opd, "");

            asm_make_bin_inst(&aac_cur, &aac_start, I_ADD, opd, length, "");

            // Check for out of memory
            if (runtime_check)
            {
                asm_make_bin_inst(&aac_cur, &aac_start, I_CMP,
                                  asm_make_int(HEAP_SIZE - WORD_SIZE), length,
                                  "Check for out of memory");
                asm_make_unary_inst(&aac_cur, &aac_start, I_JGE,
                                    make_opd_label(OUT_OF_MEM),
                                    "Jump to OOM label if cmp is true");
            }


            // Set bits to indicate which element in record is a pointer
            if (gb_collect)
            {
                if (stm->val.var->kind != VAR_ID)
                {
                    asm_combine(asm_variable(stm->val.var->val.var_exp.var),
                                &aac_cur, &aac_start);
                    operand *parent = aac_cur->val.bin.right;

                    operand *offset = asm_make_tmp();
                    operand *bit_arr = asm_make_tmp();
                    asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, parent,
                                      offset, "");

                    asm_make_bin_inst(&aac_cur, &aac_start, I_ADD,
                                      asm_make_int(1), offset, "");

                    asm_make_bin_inst(&aac_cur, &aac_start, I_MOV,
                                      make_opd_heap_loc(offset), bit_arr, "");

                    if (stm->val.var->kind == VAR_RECORD)
                    {
                        symbol *s = get_symbol(
                                stm->val.var->sym_type->val.vdl->st,
                                stm->val.var->val.var_exp.id);
                        asm_make_bin_inst(&aac_cur, &aac_start, I_BTS,
                                          asm_make_int(s->offset), bit_arr,
                                          "");
                    }
                    else if (stm->val.var->kind == VAR_EXP)
                    {

                        asm_make_bin_inst(&aac_cur, &aac_start, I_BTS,
                                          arr_offset, bit_arr, "");
                    }
                    asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, bit_arr,
                                      make_opd_heap_loc(offset), "");

                }
                // If var is an id, then it is the first allocation, thus no pointer should be set
                asm_combine(asm_set_bits(var), &aac_cur, &aac_start);
            }

            asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, length, &ASM_HEAP,
                              "Save heap pointer");
            asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, var, var, "");
            break;
        case STM_ASSIGNMENT:

            asm_combine(asm_exp(stm->val.assignment.exp), &aac_cur,
                        &aac_start);
            opd = aac_cur->val.bin.right;
            asm_combine(asm_variable(stm->val.assignment.var), &aac_cur,
                        &aac_start);
            var = aac_cur->val.bin.right;
            if (gb_collect)
            {

                if (stm->val.assignment.var->sym_type->kind == SYMBOL_RECORD ||
                    stm->val.assignment.var->sym_type->kind == SYMBOL_ARRAY ||
                    stm->val.assignment.var->sym_type->kind == SYMBOL_CLASS)
                {
                    end_label = make_opd_label(asm_make_unique_end_label());

                    asm_make_bin_inst(&aac_cur, &aac_start, I_CMP,
                                      asm_make_int(0), var,
                                      "Compare pointer to null");
                    asm_make_unary_inst(&aac_cur, &aac_start, I_JE, end_label,
                                        "Jump to un-init label");

                    asm_combine(asm_call_collect(var), &aac_cur, &aac_start);


                    asm_make_label(&aac_cur, &aac_start, LABEL_NOM,
                                   end_label->val.id, "");

                    //asm_combine(asm_dec_reference(var), &aac_cur, &aac_start);
                    if (stm->val.assignment.exp->sym_type->kind ==
                        SYMBOL_RECORD ||
                        stm->val.assignment.exp->sym_type->kind ==
                        SYMBOL_ARRAY ||
                        stm->val.assignment.exp->sym_type->kind ==
                        SYMBOL_CLASS)
                    {
                        asm_combine(asm_inc_reference(opd), &aac_cur,
                                    &aac_start);

                    }
                }
            }

            asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, opd, var,
                              "Set variable equal result of expression");
            break;
        case STM_ASSIGN_LIST:
            break;
        case STM_IF:
            end_label = make_opd_label(asm_make_unique_end_label());

            asm_combine(asm_exp(stm->val.if_then.exp), &aac_cur, &aac_start);
            opd = aac_cur->val.bin.right;

            asm_make_bin_inst(&aac_cur, &aac_start, I_CMP, asm_make_int(1),
                              opd, "Compare result from expression");

            asm_make_unary_inst(&aac_cur, &aac_start, I_JNE, end_label,
                                "Jump to end if not equal");
            asm_combine(asm_statement(stm->val.if_then.stm), &aac_cur,
                        &aac_start);

            asm_make_label(&aac_cur, &aac_start, LABEL_NOM, end_label->val.id,
                           "");

            break;
        case STM_ELSE:

            end_label = make_opd_label(asm_make_unique_end_label());
            else_label = make_opd_label(asm_make_unique_end_label());

            asm_combine(asm_exp(stm->val.if_else.exp), &aac_cur, &aac_start);
            opd = aac_cur->val.bin.right;

            asm_make_bin_inst(&aac_cur, &aac_start, I_CMP, asm_make_int(1),
                              opd, "Compare result from expression");

            asm_make_unary_inst(&aac_cur, &aac_start, I_JNE, else_label,
                                "Jump to else if not equal");
            asm_combine(asm_statement(stm->val.if_else.stm1), &aac_cur,
                        &aac_start);
            asm_make_unary_inst(&aac_cur, &aac_start, I_JMP, end_label,
                                "Jump to end label");

            asm_make_label(&aac_cur, &aac_start, LABEL_NOM, else_label->val.id,
                           "");
            asm_combine(asm_statement(stm->val.if_else.stm2), &aac_cur,
                        &aac_start);

            asm_make_label(&aac_cur, &aac_start, LABEL_NOM, end_label->val.id,
                           "");


            break;
        case STM_WHILE:
            end_label = make_opd_label(asm_make_unique_end_label());
            loop_label = make_opd_label(asm_make_unique_loop_label());

            stm->end_label = end_label->val.id;
            stm->loop_label = loop_label->val.id;

            asm_make_label(&aac_cur, &aac_start, LABEL_NOM, loop_label->val.id,
                           "Loop start");

            if (stm->val.while_do.kind == 0)
            {
                asm_combine(asm_exp(stm->val.while_do.exp), &aac_cur,
                            &aac_start);
                opd = aac_cur->val.bin.right;

                asm_make_bin_inst(&aac_cur, &aac_start, I_CMP, asm_make_int(1),
                                  opd, "Compare result from expression");
                asm_make_unary_inst(&aac_cur, &aac_start, I_JNE, end_label,
                                    "Jump to end if exp does not hold");
            }


            asm_combine(asm_statement(stm->val.while_do.stm), &aac_cur,
                        &aac_start);

            if (stm->val.while_do.kind == 1)
            {
                asm_combine(asm_exp(stm->val.while_do.exp), &aac_cur,
                            &aac_start);
                opd = aac_cur->val.bin.right;

                asm_make_bin_inst(&aac_cur, &aac_start, I_CMP, asm_make_int(1),
                                  opd, "Compare result from expression");
                asm_make_unary_inst(&aac_cur, &aac_start, I_JNE, end_label,
                                    "Jump to end if exp does not hold");
            }

            asm_make_unary_inst(&aac_cur, &aac_start, I_JMP, loop_label,
                                "Jump to start of loop");

            asm_make_label(&aac_cur, &aac_start, LABEL_NOM, end_label->val.id,
                           "");

            break;
        case STM_LIST:
            asm_combine(asm_statement_list(stm->val.sl), &aac_cur, &aac_start);
            break;
        case STM_BREAK:
            loop_label = make_opd_label(
                    stm->loop->end_label);
            asm_make_unary_inst(&aac_cur, &aac_start, I_JMP, loop_label,
                                "Break: Jump to end of loop");
            break;
        case STM_CONTINUE:
            end_label = make_opd_label(
                    stm->loop->loop_label);
            asm_make_unary_inst(&aac_cur, &aac_start, I_JMP, end_label,
                                "Continue: Jump to start of loop");
            break;
        case STM_IN:

            asm_combine(asm_variable(stm->val.var), &aac_cur, &aac_start);
            opd = aac_cur->val.bin.right;
            asm_make_inst(&aac_cur, &aac_start, I_CALL_START, "Call start");

            asm_make_bin_inst(&aac_cur, &aac_start, I_SUB, asm_make_int(8),
                              &ASM_RSP, "Allocate 8 bytes on stack");
            asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, asm_make_int(0),
                              &ASM_RAX, "0 in EAX");
            asm_make_bin_inst(&aac_cur, &aac_start, I_MOV,
                              make_opd_label("$SCAN_INT"), &ASM_RDI,
                              "Load format string");
            asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, &ASM_RSP, &ASM_RSI,
                              "Set storage");

            asm_make_unary_inst(&aac_cur, &aac_start, I_CALL, &ASM_SCANF,
                                "Calling C scanf");
            asm_make_unary_inst(&aac_cur, &aac_start, I_POP, opd,
                                "Move res from scanf in tmp");

            asm_make_inst(&aac_cur, &aac_start, I_CALL_END, "Call end");
            break;
        case STM_FREE:
            asm_combine(asm_variable(stm->val.var), &aac_cur, &aac_start);
            var = aac_cur->val.bin.right;

            if (runtime_check)
            {
                asm_make_bin_inst(&aac_cur, &aac_start, I_CMP, asm_make_int(0),
                                  var, "Compare pointer to null");
                asm_make_unary_inst(&aac_cur, &aac_start, I_JE,
                                    make_opd_label(USE_OF_UNINIT_OR_NULL),
                                    "Jump to un-init label");
            }

            asm_gb_free(var);


            break;
        case STM_ASSIGN_VAL:
            end_label = make_opd_label(asm_make_unique_end_label());
            loop_label = make_opd_label(asm_make_unique_loop_label());

            asm_combine(asm_exp(stm->val.assignment.exp), &aac_cur,
                        &aac_start);
            opd = aac_cur->val.bin.right;
            asm_combine(asm_variable(stm->val.assignment.var), &aac_cur,
                        &aac_start);
            var = aac_cur->val.bin.right;

            opd2 = asm_make_tmp();
            length = asm_make_tmp();
            operand *exp_opd = asm_make_tmp();
            operand *var_opd = asm_make_tmp();
            asm_make_bin_inst(&aac_cur, &aac_start, I_MOV,
                              make_opd_heap_loc(var), length,
                              "mov len in tmp");
            asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, asm_make_int(0),
                              opd2, "Set i = 0");

            if (gb_collect)
            {
                asm_make_bin_inst(&aac_cur, &aac_start, I_ADD, asm_make_int(2),
                                  length, "");
                asm_gb_free(var);
            }

            asm_make_label(&aac_cur, &aac_start, LABEL_NOM, loop_label->val.id,
                           "Loop start");

            asm_make_bin_inst(&aac_cur, &aac_start, I_CMP, length, opd2, "");
            asm_make_unary_inst(&aac_cur, &aac_start, I_JG, end_label, "");

            asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, opd, exp_opd, "");
            asm_make_bin_inst(&aac_cur, &aac_start, I_ADD, opd2, exp_opd, "");
            asm_make_bin_inst(&aac_cur, &aac_start, I_MOV,
                              make_opd_heap_loc(exp_opd), exp_opd, "");

            asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, var, var_opd, "");
            asm_make_bin_inst(&aac_cur, &aac_start, I_ADD, opd2, var_opd, "");
            asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, exp_opd,
                              make_opd_heap_loc(var_opd), "");


            asm_make_unary_inst(&aac_cur, &aac_start, I_INC, opd2, "i++");
            asm_make_unary_inst(&aac_cur, &aac_start, I_JMP, loop_label,
                                "Jump to start of loop");

            asm_make_label(&aac_cur, &aac_start, LABEL_NOM, end_label->val.id,
                           "");
            break;
        case STM_SWITCH:
            end_label = make_opd_label(asm_make_unique_end_label());
            stm->loop->end_label = end_label->val.id;
            stm->end_label = end_label->val.id;

            asm_combine(asm_exp(stm->val.switch_stm.exp), &aac_cur,
                        &aac_start);
            opd = aac_cur->val.bin.right;

            asm_combine(asm_switch(stm->val.switch_stm.s_stm, opd, end_label),
                        &aac_cur,
                        &aac_start);

            for (unsigned int i = 0; i < vector_size(switch_vec); i++)
            {
                switch_info *info = (switch_info *) vector_get_element(
                        switch_vec, i);
                asm_make_label(&aac_cur, &aac_start, LABEL_NOM,
                               str_concat(info->jmp_label, ""),
                               "");
                asm_combine(asm_statement(info->stm), &aac_cur, &aac_start);
            }

            while (vector_size(switch_vec) != 0)
            {
                vector_pop(switch_vec);
            }

            asm_make_label(&aac_cur, &aac_start, LABEL_NOM, end_label->val.id,
                           "");
            break;
    }
    if (stm->is_return)
    {
        char *func_end = str_concat(stm->return_label, ":");

        asm_make_unary_inst(&aac_cur, &aac_start, I_JMP,
                            make_opd_label(func_end),
                            "Return from func was here");
    }
    if (stm->is_stm_end)
    {
        char *func_end = str_concat(stm->return_label, ":");
        asm_make_label(&aac_cur, &aac_start, LABEL_NOM, func_end, "");
    }
    return aac_start;
}

abstract_asm *asm_decl_list(decl_list *dl)
{
    abstract_asm *aac_cur = NULL, *aac_start = NULL;

    switch (dl->kind)
    {
        case DL_DECL_DL:
            asm_combine(asm_decl_list(dl->dl), &aac_cur, &aac_start);
            if (dl->decl->kind == DECL_FUNC && dl->decl->val.func->template->kind == TEMP_EMPTY)
            {
                asm_combine(asm_declaration(dl->decl), &aac_cur, &aac_start);
            }
            else if (dl->decl->kind != DECL_FUNC)
            {
                asm_combine(asm_declaration(dl->decl), &aac_cur, &aac_start);
            }
            break;
        case DL_EMPTY:
            break;
    }
    return aac_start;

}

abstract_asm *asm_declaration(declaration *d)
{
    abstract_asm *aac_start = NULL;

    switch (d->kind)
    {
        case DECL_FUNC:

            asm_combine(asm_function(d->val.func), &func_cur, &aac_start);

            break;
        case DECL_TYPE:
            asm_combine(asm_type(d->val.type.type), &func_cur, &aac_start);
            break;
        case DECL_VDL:
            break;
        case DECL_CONST:
            asm_combine(asm_exp(d->val.con.exp), &func_cur, &aac_start);
            if (d->val.type.type->kind == TYPE_STRING)
            {
                vector_push(const_strings, d->val.con.id);
            }
            break;
    }
    return aac_start;
}

abstract_asm *asm_multiply(expression *exp, int counter)
{
    abstract_asm *aac_cur = NULL, *aac_start = NULL;

    operand *tmp = asm_make_tmp();
    operand *tmp2 = asm_make_tmp();

    asm_combine(asm_exp(exp), &aac_cur, &aac_start);
    operand *exp_eval = aac_cur->val.bin.right;

    asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, exp_eval, tmp, "i");

    if (counter != 0)
    {
        asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, asm_make_int(3), tmp2,
                          "i * counter");
        asm_make_bin_inst(&aac_cur, &aac_start, I_MUL, asm_make_int(counter),
                          tmp2,
                          "i * counter * J");
        asm_make_bin_inst(&aac_cur, &aac_start, I_MUL, tmp2, tmp, "i");

    }

    return aac_start;
}

abstract_asm *asm_variable(variable *var)
{
    abstract_asm *aac_cur = NULL, *aac_start = NULL;
    symbol *s, *s2;
    int depth = 0, counter = 0, dimension = 0;
    operand *op, *opd2, *opd3, *v, *static_link, *offset, *arr_size, *size_loc;
    operand *tmp;
    expression_list *exp_list;
    int num_args;
    symbol_table *st;
    switch (var->kind)
    {
        case VAR_ID:
            s = get_symbol(var->st, var->val.var_exp.id);
            if (s->sym_type != NULL && s->sym_type->kind == SYMBOL_STRING)
            {
                asm_make_bin_inst(&aac_cur, &aac_start, I_MOV,
                                  asm_make_string(s->string), asm_make_tmp(),
                                  "Move const string to reg");
                break;
            }
            if (s->is_const)
            {

                asm_combine(asm_exp(s->sym_type->exp), &aac_cur, &aac_start);

                break;
            }

            depth = get_symbol_depth(var->st, var->val.var_exp.id);
            if (in_class)
            {

                if (depth == 0)
                { // Local
                    if (s->offset == 0)
                    {
                        v = s->asm_operand;
                        asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, v, v,
                                          "Variable is a tmp");
                        return aac_start;
                    }
                    else
                    {

                        v = make_op_stack_loc(&ASM_RBP, s->offset);
                        asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, v, v,
                                          "Variable is on stack");
                        return aac_start;
                    }
                }
                else
                { // In Heap

                        s = get_symbol(vector_get_element(classes,
                                                          vector_size(
                                                                  classes) -
                                                          1),
                                       var->val.var_exp.id);

                        v = make_op_stack_loc(&ASM_RBP, -2);

                        op = asm_make_tmp();
                        asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, v, op,
                                          "");

                        asm_make_bin_inst(&aac_cur, &aac_start, I_ADD,
                                          asm_make_int(s->offset), op,
                                          "Setting offset for record");

                        opd3 = make_opd_heap_loc(op);
                        asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, opd3,
                                          opd3,
                                          "Dummy move");


                    return aac_start;

                }
            }
            else
            {
                if (depth == 0)
                {   // Variable is in local scope
                    if (s->offset == 0)
                    {
                        v = s->asm_operand;
                        asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, v, v,
                                          "Variable is a tmp");
                        return aac_start;
                    }
                    else
                    {

                        v = make_op_stack_loc(&ASM_RBP, s->offset);
                        asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, v, v,
                                          "Variable is on stack");
                        return aac_start;
                    }
                }
                else
                { // Current is in an other scope,
                    // thus must be in another stack frame
                    static_link = asm_make_tmp();
                    asm_make_bin_inst(&aac_cur, &aac_start, I_MOV,
                                      &ASM_STATIC_LINK, static_link,
                                      "First static link");
                    while (--depth > 0)
                    {
                        operand *op = make_op_stack_loc(static_link, 0);
                        static_link = asm_make_tmp();
                        asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, op,
                                          static_link, "Next static link");
                    }
                    v = make_op_stack_loc(static_link, 2 + (s->offset));
                    asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, v, v,
                                      "Variable is in register");
                }
            }
            break;
        case VAR_EXP:
            asm_combine(asm_variable(var->val.var_exp.var), &aac_cur,
                        &aac_start);
            v = aac_cur->val.bin.right;

            exp_list = var->val.var_exp.exp;
            while (exp_list != NULL)
            {
                dimension++;
                exp_list = exp_list->exp_list;
            }


            if (dimension > 1 || runtime_check)
            {
                arr_size = asm_make_tmp();
                size_loc = asm_make_tmp();
                asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, v, size_loc,
                                  "Move index to reg");
                asm_make_bin_inst(&aac_cur, &aac_start, I_MOV,
                                  make_opd_heap_loc(size_loc), arr_size,
                                  "Move size from heap to reg");
            }

            if (dimension > 1)
            {
                tmp = asm_make_tmp();
                asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, asm_make_int(0),
                                  tmp, "Init index to 0");

                counter = dimension - 1;
                exp_list = var->val.var_exp.exp;
                while (exp_list != NULL)
                {
                    asm_combine(
                            asm_multiply(exp_list->exp, counter),
                            &aac_cur, &aac_start);
                    op = aac_cur->val.bin.right;
                    asm_make_bin_inst(&aac_cur, &aac_start, I_ADD, op, tmp,
                                      "");

                    counter--;
                    exp_list = exp_list->exp_list;
                }

                //asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, tmp, offset,
                //                "Move calculated offset in reg");
                offset = tmp;
            }
            else
            {
                asm_combine(asm_exp(var->val.var_exp.exp->exp), &aac_cur,
                            &aac_start);
                offset = aac_cur->val.bin.right;

            }


            if (runtime_check)
            {

                asm_make_bin_inst(&aac_cur, &aac_start, I_CMP, asm_make_int(0),
                                  v, "Compare pointer to NULL");
                asm_make_unary_inst(&aac_cur, &aac_start, I_JE,
                                    make_opd_label(USE_OF_UNINIT_OR_NULL),
                                    "Jump to un-init label");

                if (gb_collect)
                {
                    asm_make_bin_inst(&aac_cur, &aac_start, I_SUB,
                                      asm_make_int(2), arr_size,
                                    "Subtract 2 from array size (GB-collect)");

                }
                asm_make_bin_inst(&aac_cur, &aac_start, I_CMP, arr_size,
                                  offset, "Compare size of array and offset");
                asm_make_unary_inst(&aac_cur, &aac_start, I_JGE,
                                    make_opd_label(ARR_OUT_OF_BOUNDS),
                                    "Jump if offset >= arr_size");
                asm_make_bin_inst(&aac_cur, &aac_start, I_CMP, asm_make_int(0),
                                  offset, "Compare offset with 0");
                asm_make_unary_inst(&aac_cur, &aac_start, I_JL,
                                    make_opd_label(ARR_OUT_OF_BOUNDS),
                                    "Jump if offset < 0");
            }

            tmp = asm_make_tmp();
            if (gb_collect)
            {
                asm_make_bin_inst(&aac_cur, &aac_start, I_ADD, asm_make_int(3),
                                  offset, "");
            }
            else
            {
                asm_make_bin_inst(&aac_cur, &aac_start, I_ADD, asm_make_int(1),
                                  offset, "");

            }
            arr_offset = offset;

            asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, offset, tmp,
                              "Move offset in reg to avoid overwriting");
            asm_make_bin_inst(&aac_cur, &aac_start, I_ADD, v, tmp, "");
            op = make_opd_heap_loc(tmp);
            asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, op, op, "");

            break;
        case VAR_RECORD:
            s = get_symbol(var->val.var_exp.var->sym_type->val.type_p->st,
                           var->val.var_exp.id);
            asm_combine(asm_variable(var->val.var_exp.var), &aac_cur,
                        &aac_start);
            opd2 = aac_cur->val.bin.right;

            // Check if pointer is null
            if (runtime_check)
            {
                asm_make_bin_inst(&aac_cur, &aac_start, I_CMP, asm_make_int(0),
                                  opd2, "Compare pointer to null");
                asm_make_unary_inst(&aac_cur, &aac_start, I_JE,
                                    make_opd_label(USE_OF_UNINIT_OR_NULL),
                                    "Jump to un-init label");
            }

            op = asm_make_tmp();
            asm_make_bin_inst(&aac_cur, &aac_start, I_MOV,
                              asm_make_int(s->offset), op,
                              "Setting offset for record");
            asm_make_bin_inst(&aac_cur, &aac_start, I_ADD, opd2, op,
                              "Setting record start and offset");

            opd3 = make_opd_heap_loc(op);
            asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, opd3, opd3, "");
            break;
        case VAR_CLASS:
            s = get_symbol(var->val.var_exp.var->sym_type->val._class.dl->st,
                           var->val.var_exp.id);
            asm_combine(asm_variable(var->val.var_exp.var), &aac_cur,
                        &aac_start);
            opd2 = aac_cur->val.bin.right;

            if (runtime_check)
            {
                // Check if pointer is null
                asm_make_bin_inst(&aac_cur, &aac_start, I_CMP, asm_make_int(0),
                                  opd2, "Compare pointer to NULL");
                asm_make_unary_inst(&aac_cur, &aac_start, I_JE,
                                    make_opd_label(USE_OF_UNINIT_OR_NULL),
                                    "Jump to un-init label");
            }


            op = asm_make_tmp();
            asm_make_bin_inst(&aac_cur, &aac_start, I_MOV,
                              asm_make_int(s->offset), op,
                              "Setting offset for class");
            asm_make_bin_inst(&aac_cur, &aac_start, I_ADD, opd2, op,
                              "Setting class start and offset");

            opd3 = make_opd_heap_loc(op);
            asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, opd3, opd3, "");
            break;
        case VAR_FUNC:
            asm_make_inst(&aac_cur, &aac_start, I_CALL_START, "Call start");

            asm_combine(asm_variable(var->val.var_exp.var), &aac_cur,
                        &aac_start);
            opd2 = aac_cur->val.bin.right;

            if (var->val.var_exp.var->val.var_exp.var != NULL)
            {
                st = var->val.var_exp.var->val.var_exp.var->sym_type->val._class.st;
            }
            else
            {
                st = var->st;
            }

            s = get_symbol(st, var->val.var_exp.var->val.var_exp.id);
            s2 = get_symbol(s->sym_type->val._class.dl->st,
                            var->val.var_exp.func->head->id);

            num_args = s2->sym_type->val.func.func->head->num_args;

            if (num_args > 0)
            {
                expression_list *el = var->val.var_exp.al->exp_list;
                vector *v = vector_init();
                int size = 0;
                while (el != NULL)
                {
                    vector_push(v, el->exp);
                    el = el->exp_list;
                    size++;
                }
                while (size != 0)
                {
                    asm_combine(asm_exp(vector_get_element(v, size - 1)),
                                &aac_cur, &aac_start);
                    op = aac_cur->val.bin.right;
                    asm_make_unary_inst(&aac_cur, &aac_start, I_PUSH, op,
                                        "Push argument on stack");
                    size--;
                }
            }


            operand *v;

            //printf("Depth %s \n", var->val.var_exp.var->val.var_exp.id);
            /*depth = get_symbol_depth(st, var->val.var_exp.var->val.var_exp.id);

            if (depth == 0)
            {   // Variable is in local scope
                if (s->offset == 0)
                {
                    v = s->asm_operand;

                }
                else
                {

                    v = make_op_stack_loc(&ASM_RBP, s->offset);

                }
            }
            else
            { // Current is in an other scope,
                // thus must be in another stack frame

                static_link = asm_make_tmp();
                asm_make_bin_inst(&aac_cur, &aac_start, I_MOV,
                                  &ASM_STATIC_LINK, static_link,
                                  "First static link");
                while (--depth > 0)
                {
                    operand *op = make_op_stack_loc(static_link, 0);
                    static_link = asm_make_tmp();
                    asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, op,
                                      static_link, "Next static link");
                }
                v = make_op_stack_loc(static_link, 2 + (s->offset));

            }*/

            v = asm_make_tmp();
            asm_make_bin_inst(&aac_cur, &aac_start, I_MOV,
                              asm_make_int(s->offset), v,
                              "Setting offset for class");
            asm_make_bin_inst(&aac_cur, &aac_start, I_ADD, opd2, v,
                              "Setting class start and offset");



            asm_make_unary_inst(&aac_cur, &aac_start, I_PUSH, opd2,
                                "Push self");


            asm_make_unary_inst(&aac_cur, &aac_start, I_CALL, make_opd_label(
                    var->val.var_exp.func->head->new_name),
                                "Call the function");

            asm_make_unary_inst(&aac_cur, &aac_start, I_POP, opd2,
                                "Pop self");

            asm_make_bin_inst(&aac_cur, &aac_start, I_ADD,
                              asm_make_int(WORD_SIZE * num_args),
                              &ASM_RSP, "Popping arguments");

            op = asm_make_tmp();
            asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, &ASM_RAX, op,
                              "Add return value to tmp");
            asm_make_inst(&aac_cur, &aac_start, I_CALL_END, "Call end");
            asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, op, op,
                              "Dummy move");
            break;
    }
    return aac_start;
}

abstract_asm *asm_term(term *t)
{
    abstract_asm *aac_cur = NULL, *aac_start = NULL, *exp;
    operand *op, *static_link, *last, *v, *float_label;
    symbol *s;
    char *label;
    int num_args, depth;
    switch (t->kind)
    {
        case TERM_VAR:
            asm_combine(asm_variable(t->val.var), &aac_cur, &aac_start);
            op = aac_cur->val.bin.right;
            asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, op, asm_make_tmp(),
                              "Move result in tmp to avoid overwrite");
            break;
        case TERM_AL:

            asm_make_inst(&aac_cur, &aac_start, I_CALL_START, "Call start");
            s = get_symbol(t->st, t->val.term_act_list.id);
            num_args = s->sym_type->val.func.func->head->num_args;

            if (num_args > 0)
            {
                expression_list *el = t->val.term_act_list.al->exp_list;
                vector *v = vector_init();
                int size = 0;
                while (el != NULL)
                {
                    asm_combine(asm_exp(el->exp), &aac_cur, &aac_start);
                    op = aac_cur->val.bin.right;
                    vector_push(v, op);
                    el = el->exp_list;
                    size++;
                }
                while (size != 0)
                {

                    asm_make_unary_inst(&aac_cur, &aac_start, I_PUSH,
                                        vector_get_element(v, size - 1),
                                        "Push function argument");
                    size--;
                }
            }

            depth = get_symbol_depth(t->st, t->val.term_act_list.id);
            static_link = asm_make_tmp();

            if (!in_class)
            {
                if (depth == 0)
                {
                    asm_make_bin_inst(&aac_cur, &aac_start, I_MOV,
                                      asm_make_int(WORD_SIZE * 2), static_link,
                                      "Mov WORD_SIZE * 2 into static link");
                    asm_make_bin_inst(&aac_cur, &aac_start, I_ADD, &ASM_RBP,
                                      static_link, "Add RBP to static link");
                    asm_make_unary_inst(&aac_cur, &aac_start, I_PUSH,
                                        static_link,
                                        "Push static link to stack");
                }
                else
                {
                    asm_make_bin_inst(&aac_cur, &aac_start, I_MOV,
                                      &ASM_STATIC_LINK, static_link,
                                      "Create static link");
                    while (--depth > 0)
                    {
                        last = static_link;
                        static_link = asm_make_tmp();
                        asm_make_bin_inst(&aac_cur, &aac_start, I_MOV,
                                          make_op_stack_loc(last, 0),
                                          static_link,
                                          "Getting next link");
                    }
                    asm_make_unary_inst(&aac_cur, &aac_start, I_PUSH,
                                        static_link,
                                        "Push static link to stack");
                }
            }
            else
            {
                v = make_op_stack_loc(&ASM_RBP, -2);
                asm_make_unary_inst(&aac_cur, &aac_start, I_PUSH, v,
                                    "Push self");

            }

            asm_make_unary_inst(&aac_cur, &aac_start, I_CALL, make_opd_label(
                    s->sym_type->val.func.func->head->new_name),
                                "Call the function");

            if (in_class)
            {
                //asm_make_unary_inst(&aac_cur, &aac_start, I_POP, v,
                //                  "Pop self");

            }
            asm_make_bin_inst(&aac_cur, &aac_start, I_ADD,
                              asm_make_int(WORD_SIZE + WORD_SIZE * num_args),
                              &ASM_RSP, "Popping arguments and static link");


            op = asm_make_tmp();
            asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, &ASM_RAX, op,
                              "Add return value to reg");
            asm_make_inst(&aac_cur, &aac_start, I_CALL_END, "Call end");
            asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, op, op,
                              "Dummy move");


            break;
        case TERM_PARENTHESES:
            asm_combine(asm_exp(t->val.exp), &aac_cur, &aac_start);
            break;
        case TERM_NOT:
            exp = asm_term(t->val.not_term);
            asm_combine(exp, &aac_cur, &aac_start);
            asm_make_bin_inst(&aac_cur, &aac_start, I_XOR, asm_make_int(1),
                              exp->val.bin.right, "");
            break;
        case TERM_ABS:
            asm_combine(asm_exp(t->val.exp), &aac_cur, &aac_start);
            if (t->val.exp->sym_type->kind == SYMBOL_INT)
            {
                operand *tmp = aac_cur->val.bin.right;
                op = asm_make_tmp();
                operand *true_label = make_opd_label(
                        asm_make_unique_true_label());

                asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, tmp, op,
                           "Move result from exp to reg to avoid overwriting");

                asm_make_bin_inst(&aac_cur, &aac_start, I_CMP, asm_make_int(0),
                                  op, "Compare 0 and result of expression");
                asm_make_unary_inst(&aac_cur, &aac_start, I_JGE, true_label,
                                    "Skip negation if res >= 0");
                asm_make_unary_inst(&aac_cur, &aac_start, I_NEG, op,
                                    "Do negation (0-exp)");
                asm_make_label(&aac_cur, &aac_start, LABEL_NOM,
                               true_label->val.id, "");
                asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, op, op, "");
            }
            else if (t->val.exp->sym_type->kind == SYMBOL_ARRAY)
            {
                asm_combine(asm_exp(t->val.exp), &aac_cur, &aac_start);
                operand *tmp = aac_cur->val.bin.right;
                op = asm_make_tmp();

                asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, tmp, op,
                                  "Move result from exp to avoid overwriting");
                operand *len = asm_make_tmp();
                asm_make_bin_inst(&aac_cur, &aac_start, I_MOV,
                                  make_opd_heap_loc(op), len, "");
                if (gb_collect)
                {
                    asm_make_bin_inst(&aac_cur, &aac_start, I_SUB,
                                      asm_make_int(2), len, "");
                }

            }
            break;
        case TERM_INT:
            asm_make_bin_inst(&aac_cur, &aac_start, I_MOV,
                              asm_make_int(t->val.integer), asm_make_tmp(),
                              "Move integer value to reg");
            break;
        case TERM_FLOAT:
            label = asm_make_unique_float_name();
            float* value = mem_alloc(sizeof(float));
            *value = t->val.floating_point;
            vector_push(floats, label);
            vector_push(floats, value);

            float_label = make_opd_label(str_concat(label, "(%rip)"));

            asm_make_bin_inst(&aac_cur, &aac_start, I_MOV,
                              float_label, &ASM_XMM0,
                              "Move float value to reg");
            asm_make_bin_inst(&aac_cur, &aac_start, I_MOV,
                              &ASM_XMM0, make_op_stack_loc(&ASM_RBP, floatOffset),
                              "Move float to stack");
            floatOffset++;
            break;
        case TERM_TRUE:
            asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, asm_make_int(1),
                              asm_make_tmp(), "Move True to reg");
            break;
        case TERM_FALSE:
            asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, asm_make_int(0),
                              asm_make_tmp(), "Move False to reg");
            break;
        case TERM_NULL:
            asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, asm_make_int(0),
                              asm_make_tmp(), "Move NULL to reg");
            break;
        case TERM_STRING:
            vector_push(const_strings, t->val.string);
            break;
    }
    return aac_start;
}

abstract_asm *asm_exp(expression *exp)
{
    abstract_asm *aac_cur = NULL, *aac_start = NULL;
    abstract_asm *left, *right;
    operand *l, *r;

    if (exp->kind == EXP_TERM)
    {
        asm_combine(asm_term(exp->val.term), &aac_cur, &aac_start);
        return aac_start;
    }
    else if (exp->val.op.left != NULL && exp->val.op.right != NULL)
    {
        left = asm_exp(exp->val.op.left);
        asm_combine(left, &aac_cur, &aac_start);
        l = aac_cur->val.bin.right;

        // true || exp (exp can be skipped). false && exp (exp can be skipped)
        if (exp->kind == EXP_AND)
        {
            asm_and_or(0, l, exp, &aac_cur, &aac_start);
            return aac_start;
        }
        else if (exp->kind == EXP_OR)
        {
            asm_and_or(1, l, exp, &aac_cur, &aac_start);
            return aac_start;
        }

        right = asm_exp(exp->val.op.right);
        asm_combine(right, &aac_cur, &aac_start);
        r = aac_cur->val.bin.right;
        switch (exp->kind)
        {
            case EXP_TIMES:
                asm_make_bin_inst(&aac_cur, &aac_start, I_MUL, r, l,
                                  "Multiply right and left operand");
                break;
            case EXP_DIV:
                asm_combine(asm_div(l, r), &aac_cur, &aac_start);
                break;
            case EXP_PLUS:
                asm_make_bin_inst(&aac_cur, &aac_start, I_ADD, r, l,
                                  "Add right and left operand");
                break;
            case EXP_MINUS:
                asm_make_bin_inst(&aac_cur, &aac_start, I_SUB, r, l,
                                  "Subtract right and left operands");
                break;
            case EXP_EQ:
                asm_general_exp(I_JE, l, r, aac_cur, aac_start);
                break;
            case EXP_NEQ:
                asm_general_exp(I_JNE, l, r, aac_cur, aac_start);
                break;
            case EXP_GREATER:
                asm_general_exp(I_JG, l, r, aac_cur, aac_start);
                break;
            case EXP_LESS:
                asm_general_exp(I_JL, l, r, aac_cur, aac_start);
                break;
            case EXP_GEQ:
                asm_general_exp(I_JGE, l, r, aac_cur, aac_start);
                break;
            case EXP_LEQ:
                asm_general_exp(I_JLE, l, r, aac_cur, aac_start);
                break;
            case EXP_AND:
            case EXP_OR:
            case EXP_TERM:
                break;
        }
    }
    return aac_start;
}

abstract_asm *
asm_switch(switch_stm *s_stm, operand *switch_val, operand *switch_end)
{
    abstract_asm *aac_cur = NULL, *aac_start = NULL;

    if (s_stm == NULL)
    {
        asm_make_unary_inst(&aac_cur, &aac_start, I_JMP, switch_end,
                            "Jump to statement if equal");
        return aac_start;
    }

    asm_combine(asm_switch_case(s_stm->switch_case, switch_val), &aac_cur,
                &aac_start);
    asm_combine(asm_switch(s_stm->switch_list, switch_val, switch_end),
                &aac_cur, &aac_start);


    return aac_start;
}

abstract_asm *asm_switch_case(switch_case *s_case, operand *switch_val)
{
    abstract_asm *aac_cur = NULL, *aac_start = NULL;

    operand *stm_jmp = make_opd_label(asm_make_unique_end_label());

    asm_combine(asm_exp(s_case->exp), &aac_cur, &aac_start);
    operand *opd = aac_cur->val.bin.right;
    asm_make_bin_inst(&aac_cur, &aac_start, I_CMP, opd, switch_val,
                      "Compare switch value and case value");
    asm_make_unary_inst(&aac_cur, &aac_start, I_JE, stm_jmp,
                        "Jump to statement if equal");

    switch_info *info = NEW(switch_info);
    info->stm = s_case->stm;
    info->jmp_label = stm_jmp->val.id;
    vector_push(switch_vec, info);

    return aac_start;
}

void asm_general_exp(int kind, operand *l, operand *r, abstract_asm *aac_cur,
                     abstract_asm *aac_start)
{
    operand *tmp = asm_make_tmp();
    operand *end_label = make_opd_label(asm_make_unique_end_label());
    operand *true_label = make_opd_label(asm_make_unique_true_label());

    asm_make_bin_inst(&aac_cur, &aac_start, I_CMP, r, l,
                      "Compare left and right operand");
    asm_make_unary_inst(&aac_cur, &aac_start, kind, true_label,
                        "Jump if evaluation is true");
    asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, asm_make_int(0), tmp,
                      "Set result to 0 (False)");

    asm_make_unary_inst(&aac_cur, &aac_start, I_JMP, end_label, "Jump to end");
    asm_make_label(&aac_cur, &aac_start, LABEL_NOM, true_label->val.id, "");
    asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, asm_make_int(1), tmp,
                      "Set expression to 1 (True)");
    asm_make_label(&aac_cur, &aac_start, LABEL_NOM, end_label->val.id, "");

    asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, tmp, tmp,
                      "Dummy move");
}

void asm_and_or(int kind, operand *l, expression *exp, abstract_asm **aac_cur,
                abstract_asm **aac_start)
{
    operand *r;
    operand *tmp = asm_make_tmp();
    operand *end_label = make_opd_label(asm_make_unique_end_label());
    abstract_asm *right = asm_exp(exp->val.op.right);


    asm_make_bin_inst(aac_cur, aac_start, I_MOV, asm_make_int(kind), tmp,
                      "Setting False / True");
    asm_make_bin_inst(aac_cur, aac_start, I_CMP, asm_make_int(kind), l,
                      "Comparing left side");
    asm_make_unary_inst(aac_cur, aac_start, I_JE, end_label,
                        "Jump if equal to skip evaluating right operand");

    asm_combine(right, aac_cur, aac_start);
    r = (*aac_cur)->val.bin.right;

    asm_make_bin_inst(aac_cur, aac_start, I_MOV, r, tmp,
                      "Move result from right operand in reg");
    asm_make_label(aac_cur, aac_start, LABEL_NOM, end_label->val.id, "");
    asm_make_bin_inst(aac_cur, aac_start, I_MOV, tmp, tmp, "");
}

abstract_asm *asm_div(operand *l, operand *r)
{
    abstract_asm *aac_cur = NULL, *aac_start = NULL;

    asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, l, &ASM_RAX,
                      "Move value in EAX for division");

    if (runtime_check)
    {
        // Check for division by 0
        asm_make_bin_inst(&aac_cur, &aac_start, I_CMP, asm_make_int(0), r,
                          "Check for division by 0");
        asm_make_unary_inst(&aac_cur, &aac_start, I_JE,
                            make_opd_label(DIV_BY_0),
                            "Jump to division fail label");
    }

    asm_make_inst(&aac_cur, &aac_start, I_CLTD, "");
    asm_make_bin_inst(&aac_cur, &aac_start, I_DIV, r, &ASM_RAX, "Do division");
    asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, &ASM_RAX, asm_make_tmp(),
                      "Move result in reg");
    return aac_start;
}

int get_local_vars(decl_list *dl, int offset)
{
    decl_list *tmp = dl;
    var_decl_list *vdl_tmp;
    symbol *s;
    int has_func_de = 0;

    while (tmp->kind != DL_EMPTY)
    {
        if (tmp->decl->kind == DECL_FUNC)
        {
            has_func_de = 1;
            break;
        }
        tmp = tmp->dl;
    }
    tmp = dl;
    // Offset is only need if local variables could be used in an inner scope
    if (has_func_de)
    {
        while (tmp->kind != DL_EMPTY)
        {
            if (tmp->decl->kind == DECL_VDL)
            {
                vdl_tmp = tmp->decl->val.vdl;
                while (vdl_tmp != NULL)
                {
                    s = get_symbol(dl->st, vdl_tmp->vt->id);
                    s->offset = offset;
                    offset++;

                    vdl_tmp = vdl_tmp->vdl;
                }
            }
            tmp = tmp->dl;
        }
        return offset;
    }

    while (tmp->kind != DL_EMPTY)
    {
        if (tmp->decl->kind == DECL_VDL)
        {
            vdl_tmp = tmp->decl->val.vdl;
            while (vdl_tmp != NULL)
            {
                s = get_symbol(dl->st, vdl_tmp->vt->id);
                s->offset = 0;
                s->asm_operand = asm_make_tmp();
                vdl_tmp = vdl_tmp->vdl;
            }
        }
        tmp = tmp->dl;
    }
    return 0;
}

abstract_asm *zero_init_local_vars(decl_list *dl)
{
    abstract_asm *aac_cur = NULL, *aac_start = NULL;

    symbol *s;
    var_decl_list *vdl_tmp;
    decl_list *tmp = dl;

    while (tmp->kind != DL_EMPTY)
    {
        if (tmp->decl->kind == DECL_VDL)
        {
            vdl_tmp = tmp->decl->val.vdl;
            while (vdl_tmp != NULL)
            {
                s = get_symbol(tmp->decl->st, vdl_tmp->vt->id);
                if (s->offset == 0)
                {
                    operand *var = s->asm_operand;
                    asm_make_bin_inst(&aac_cur, &aac_start, I_MOV,
                                      asm_make_int(0), var,
                                      "Init variable to 0");

                }
                else
                {
                    asm_make_bin_inst(&aac_cur, &aac_start, I_MOV,
                                      asm_make_int(0),
                                      make_op_stack_loc(&ASM_RBP, s->offset),
                                      "init variable to 0");
                }
                vdl_tmp = vdl_tmp->vdl;
            }
        }
        tmp = tmp->dl;
    }

    return aac_start;
}

abstract_asm *asm_printf(operand *arg3, operand *arg2, operand *arg1)
{
    abstract_asm *aac_cur = NULL, *aac_start = NULL;

    asm_make_inst(&aac_cur, &aac_start, I_CALL_START, "Call start");

    if (arg3 != NULL)
    {
        asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, arg3, &ASM_RDI,
                          "Push third argument");
    }

    if (arg2 != NULL)
    {
        asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, arg2, &ASM_RSI,
                          "Push second argument");
    }

    if (arg1 != NULL)
    {
        asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, arg1, &ASM_RAX,
                          "Push first argument");
    }

    asm_make_unary_inst(&aac_cur, &aac_start, I_CALL, &ASM_PRINTF,
                        "Calling C printf");
    asm_make_inst(&aac_cur, &aac_start, I_CALL_END, "Call end");
    return aac_start;
}

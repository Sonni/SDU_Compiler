#include <stdlib.h>
#include "garbage_collector.h"
#include "../../utils/string_def.h"
#include "asm_gen.h"

abstract_asm *asm_set_bits(operand *tmp_heap_loc)
{
    abstract_asm *aac_cur = NULL, *aac_start = NULL;
    operand *opd = asm_make_tmp();

    asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, asm_make_int(2), opd,
                      "offset 2 is reference counter");
    asm_make_bin_inst(&aac_cur, &aac_start, I_ADD, tmp_heap_loc, opd,
                      "Setting record start and offset");
    operand *opd3 = make_opd_heap_loc(opd);
    asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, asm_make_int(1), opd3,
                      "Set reference count to 1");

    asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, asm_make_int(1), opd,
                      "offset 1 is bitmap counter");
    asm_make_bin_inst(&aac_cur, &aac_start, I_ADD, tmp_heap_loc, opd,
                      "Setting record start and offset");
    opd3 = make_opd_heap_loc(opd);

    operand *tmp = asm_make_tmp();
    asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, opd3, tmp,
                      "Mov reference counter into tmp");
    asm_make_bin_inst(&aac_cur, &aac_start, I_XOR, tmp, tmp,
                      "put new value into heap");
    asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, tmp, opd3,
                      "Mov reference counter into tmp");

    return aac_start;
}

abstract_asm *asm_inc_reference(operand *var)
{
    abstract_asm *aac_cur = NULL, *aac_start = NULL;

    operand *opd = asm_make_tmp();
    operand *opd2 = asm_make_tmp();

    asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, asm_make_int(2), opd,
                      "offset 2 is reference counter");
    asm_make_bin_inst(&aac_cur, &aac_start, I_ADD, var, opd,
                      "Adding rec start and offset");
    operand *opd3 = make_opd_heap_loc(opd);
    asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, opd3, opd2,
                      "Mov reference counter into tmp");

    asm_make_unary_inst(&aac_cur, &aac_start, I_INC, opd2,
                        "inc reference counter");
    asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, opd2, opd3,
                      "put new value into heap");

    return aac_start;
}

// Length, bit string, reference counter
abstract_asm *asm_dec_reference()
{
    abstract_asm *aac_cur = NULL, *aac_start = NULL;

    operand *opd = asm_make_tmp();
    operand *opd2 = asm_make_tmp();
    operand *rec_len_opd = asm_make_tmp();
    operand *bit_arr = asm_make_tmp();

    operand *end_label = make_opd_label(asm_make_unique_end_label());
    operand *bit_end_label = make_opd_label(asm_make_unique_end_label());
    operand *loop_end_label = make_opd_label(asm_make_unique_end_label());
    operand *loop_label = make_opd_label(asm_make_unique_loop_label());
    operand *var = asm_make_tmp();
    asm_make_label(&aac_cur, &aac_start, LABEL_FUNC_BEGIN, "collect_garbage:",
                   "");
    aac_cur->val.label.num_param = 0;

    asm_make_bin_inst(&aac_cur, &aac_start, I_MOV,
                      make_op_stack_loc(&ASM_RBP, -2), var, "pop rec start");

    // Get rec length
    asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, asm_make_int(0), opd2,
                      "offset 0 is len");
    asm_make_bin_inst(&aac_cur, &aac_start, I_ADD, var, opd2,
                      "offset 0 is len");
    operand *rec_len = make_opd_heap_loc(opd2);
    asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, rec_len, rec_len_opd,
                      "Mov len of rec in tmp");

    // Get bit array
    asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, asm_make_int(1), opd,
                      "offset 1 is bitmap");
    asm_make_bin_inst(&aac_cur, &aac_start, I_ADD, var, opd,
                      "Adding rec start and offset");
    operand *opd3 = make_opd_heap_loc(opd);
    asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, opd3, bit_arr,
                      "Mov reference counter into tmp");


    // Decrement reference counter
    asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, asm_make_int(2), opd,
                      "offset 2 is reference counter");
    asm_make_bin_inst(&aac_cur, &aac_start, I_ADD, var, opd,
                      "Adding rec start and offset");
    opd3 = make_opd_heap_loc(opd);
    asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, opd3, opd2,
                      "Mov reference counter into tmp");

    asm_make_bin_inst(&aac_cur, &aac_start, I_SUB, asm_make_int(1), opd2,
                      "Inc reference counter");
    asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, opd2, opd3,
                      "put new value into heap");

    // Check if reference counter is 0
    asm_make_bin_inst(&aac_cur, &aac_start, I_CMP, asm_make_int(0), opd2,
                      "Check if reference counter is 0");
    asm_make_unary_inst(&aac_cur, &aac_start, I_JNE, end_label,
                        "Check if reference counter is 0");
    operand *div = asm_make_tmp();

    asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, asm_make_int(8), div,
                      "compare rec size and i");

    asm_combine(asm_div(rec_len_opd, div), &aac_cur, &aac_start);
    operand *size = aac_cur->val.bin.right;


    // Check bit string to find pointers in record

    // While i < rec size
    asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, asm_make_int(2), opd,
                      "set i = 0");
    asm_make_label(&aac_cur, &aac_start, LABEL_NOM, loop_label->val.id,
                   "Loop start");

    asm_make_bin_inst(&aac_cur, &aac_start, I_CMP, size, opd,
                      "compare rec size and i");
    asm_make_unary_inst(&aac_cur, &aac_start, I_JG, loop_end_label,
                        "Jump to end if i < rec size");

    // if bit is set

    asm_make_bin_inst(&aac_cur, &aac_start, I_BT, opd, bit_arr,
                      "Check if bit is set");
    asm_make_unary_inst(&aac_cur, &aac_start, I_JB, bit_end_label,
                        "Jmp if bit is not set");

    asm_make_inst(&aac_cur, &aac_start, I_CALL_START, "Call start");
    operand *new_tmp = asm_make_tmp();
    operand *new_tmp2 = asm_make_tmp();
    asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, var, new_tmp,
                      "Check if bit is set");
    asm_make_bin_inst(&aac_cur, &aac_start, I_ADD, opd, new_tmp,
                      "Check if bit is set");
    asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, make_opd_heap_loc(new_tmp),
                      new_tmp2, "Check if bit is set");

    asm_make_unary_inst(&aac_cur, &aac_start, I_PUSH, new_tmp2,
                        "Push rec start");
    asm_make_unary_inst(&aac_cur, &aac_start, I_CALL,
                        make_opd_label("collect_garbage"), "Push rec start");
    asm_make_unary_inst(&aac_cur, &aac_start, I_POP, new_tmp2,
                        "Pop rec start");
    asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, asm_make_int(0),
                      make_opd_heap_loc(new_tmp), "Set pointer to null");
    asm_make_inst(&aac_cur, &aac_start, I_CALL_END, "Call end");

    asm_make_label(&aac_cur, &aac_start, LABEL_NOM, bit_end_label->val.id, "");

    // end if

    // loop end
    asm_make_unary_inst(&aac_cur, &aac_start, I_INC, opd, "Inc i");
    asm_make_unary_inst(&aac_cur, &aac_start, I_JMP, loop_label,
                        "Jump to start of loop");
    asm_make_label(&aac_cur, &aac_start, LABEL_NOM, loop_end_label->val.id,
                   "");
    // end while


    // End of following pointers
    asm_make_label(&aac_cur, &aac_start, LABEL_NOM, end_label->val.id, "");
    asm_make_label(&aac_cur, &aac_start, LABEL_FUNC_END,
                   "end_collect_garbage:", "");
    asm_make_inst(&aac_cur, &aac_start, I_RET, "");

    return aac_start;
}

abstract_asm *asm_find_free_loc()
{
    abstract_asm *aac_cur = NULL, *aac_start = NULL;

    asm_make_label(&aac_cur, &aac_start, LABEL_FUNC_BEGIN, "get_free_mem:",
                   "");

    operand *end2_label = make_opd_label(asm_make_unique_end_label());
    operand *end3_label = make_opd_label(asm_make_unique_end_label());
    operand *loop_end_label = make_opd_label(asm_make_unique_end_label());
    operand *loop_label = make_opd_label(asm_make_unique_loop_label());
    operand *opd2 = asm_make_tmp();
    operand *opd3 = asm_make_tmp();
    operand *length = asm_make_tmp();


    asm_make_bin_inst(&aac_cur, &aac_start, I_MOV,
                      make_op_stack_loc(&ASM_RBP, -2), length,
                      "pop requested length");

    operand *opd_tmp = asm_make_tmp();
    asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, &ASM_HEAP, opd_tmp, "");
    asm_make_bin_inst(&aac_cur, &aac_start, I_ADD, length, opd_tmp, "");
    asm_make_bin_inst(&aac_cur, &aac_start, I_CMP,
                      asm_make_int(HEAP_SIZE - WORD_SIZE), opd_tmp, "");
    asm_make_unary_inst(&aac_cur, &aac_start, I_JGE, end3_label, "");

    asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, &ASM_HEAP, opd_tmp, "");
    asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, make_opd_heap_loc(opd_tmp),
                      opd_tmp, "");


    asm_make_bin_inst(&aac_cur, &aac_start, I_CMP, asm_make_int(0), opd_tmp,
                      "Heap might be at unalloc place in heap");
    asm_make_unary_inst(&aac_cur, &aac_start, I_JE, loop_end_label,
                        "Jmp if free space is not found");
    asm_make_bin_inst(&aac_cur, &aac_start, I_CMP, asm_make_int(1), opd_tmp,
                      "Heap might be at start");
    asm_make_unary_inst(&aac_cur, &aac_start, I_JE, loop_end_label,
                        "Jmp if free space is not found");

    asm_make_label(&aac_cur, &aac_start, LABEL_NOM, end3_label->val.id, "");


    asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, asm_make_int(1), opd2, "");
    asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, make_opd_heap_loc(opd2),
                      opd3, "");

    // While i < rec size
    asm_make_label(&aac_cur, &aac_start, LABEL_NOM, loop_label->val.id,
                   "Loop start");

    // Check for out of memory
    if (runtime_check)
    {
        asm_make_bin_inst(&aac_cur, &aac_start, I_CMP,
                          asm_make_int(HEAP_SIZE - WORD_SIZE), opd2,
                          "Check for out of memory");
        asm_make_unary_inst(&aac_cur, &aac_start, I_JGE,
                            make_opd_label(OUT_OF_MEM),
                            "Jump to OOM label if cmp is true");
    }


    operand *reference_counter = asm_make_tmp();
    asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, opd2, reference_counter,
                      "offset 2 is reference counter");
    asm_make_bin_inst(&aac_cur, &aac_start, I_ADD, asm_make_int(2),
                      reference_counter, "Adding rec start and offset");
    asm_make_bin_inst(&aac_cur, &aac_start, I_MOV,
                      make_opd_heap_loc(reference_counter), reference_counter,
                      "Mov reference counter into tmp");

    asm_make_bin_inst(&aac_cur, &aac_start, I_CMP, asm_make_int(0),
                      reference_counter, "Check if space is free");
    asm_make_unary_inst(&aac_cur, &aac_start, I_JNE, end2_label,
                        "Jmp if free space is not found");


    asm_make_bin_inst(&aac_cur, &aac_start, I_CMP, opd3, length,
                      "Compare len and requested space");
    asm_make_unary_inst(&aac_cur, &aac_start, I_JG, end2_label,
                        "Jmp if not enough space");

    asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, opd2, &ASM_HEAP,
                      "Mov reference counter into tmp");
    asm_make_unary_inst(&aac_cur, &aac_start, I_JMP, loop_end_label,
                        "Space found, heap set, jmp to end");


    asm_make_label(&aac_cur, &aac_start, LABEL_NOM, end2_label->val.id, "");


    asm_make_bin_inst(&aac_cur, &aac_start, I_ADD, opd3, opd2, "");
    asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, make_opd_heap_loc(opd2),
                      opd3, "");


    asm_make_unary_inst(&aac_cur, &aac_start, I_JMP, loop_label,
                        "Jump to start of loop");
    asm_make_label(&aac_cur, &aac_start, LABEL_NOM, loop_end_label->val.id,
                   "");

    asm_make_label(&aac_cur, &aac_start, LABEL_FUNC_END, "end_get_free_mem:",
                   "");

    asm_make_inst(&aac_cur, &aac_start, I_RET, "");


    return aac_start;
}

abstract_asm *asm_garbage_collect(operand *var, operand *length)
{
    abstract_asm *aac_cur = NULL, *aac_start = NULL;

    operand *end_label = make_opd_label(asm_make_unique_end_label());

    asm_make_bin_inst(&aac_cur, &aac_start, I_CMP, asm_make_int(0), var,
                      "Compare pointer to null");
    asm_make_unary_inst(&aac_cur, &aac_start, I_JE, end_label,
                        "Jump to un-init label");


    asm_combine(asm_call_collect(var), &aac_cur, &aac_start);


    //asm_combine(asm_dec_reference(var), &aac_cur, &aac_start);
    asm_make_label(&aac_cur, &aac_start, LABEL_NOM, end_label->val.id, "");


    asm_make_inst(&aac_cur, &aac_start, I_CALL_START, "Call start");

    asm_make_unary_inst(&aac_cur, &aac_start, I_PUSH, length,
                        "Push requested length");
    asm_make_unary_inst(&aac_cur, &aac_start, I_CALL,
                        make_opd_label("get_free_mem"),
                        "Find free space in heap");
    asm_make_unary_inst(&aac_cur, &aac_start, I_POP, length,
                        "Pop requested length");

    asm_make_inst(&aac_cur, &aac_start, I_CALL_END, "Call end");
    return aac_start;

}

abstract_asm *asm_call_collect(operand *var)
{
    abstract_asm *aac_cur = NULL, *aac_start = NULL;


    asm_make_inst(&aac_cur, &aac_start, I_CALL_START, "Call start");

    if (var->kind == OPD_STACK)
    {
        operand *tmp = asm_make_tmp();
        asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, var, tmp,
                          "Move stack element in tmp");

        asm_make_unary_inst(&aac_cur, &aac_start, I_PUSH, tmp,
                            "Push rec start");
        asm_make_unary_inst(&aac_cur, &aac_start, I_CALL,
                            make_opd_label("collect_garbage"),
                            "Do reference counting");
        asm_make_unary_inst(&aac_cur, &aac_start, I_POP, tmp, "Pop rec start");
    }
    else
    {
        asm_make_unary_inst(&aac_cur, &aac_start, I_PUSH, get_reg(var),
                            "Push rec start");
        asm_make_unary_inst(&aac_cur, &aac_start, I_CALL,
                            make_opd_label("collect_garbage"),
                            "Do reference counting");
        asm_make_unary_inst(&aac_cur, &aac_start, I_POP, get_reg(var),
                            "Pop rec start");
    }


    asm_make_inst(&aac_cur, &aac_start, I_CALL_END, "Call end");
    return aac_start;
}

abstract_asm *asm_gb_free(operand *var)
{
    abstract_asm *aac_cur = NULL, *aac_start = NULL;

    operand *opd = asm_make_tmp();
    asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, asm_make_int(2),
                      opd, "offset 2 is reference counter");
    asm_make_bin_inst(&aac_cur, &aac_start, I_ADD, var, opd,
                      "Adding rec start and offset");
    operand *opd2 = make_opd_heap_loc(opd);
    asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, opd2, opd,
                      "Move reference counter into tmp");

    asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, asm_make_int(1),
                      opd,
                      "Move 1 in reference counter, freeing in GB");
    asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, opd, opd2,
                      "Put new value into heap");


    asm_combine(asm_call_collect(var), &aac_cur, &aac_start);



    return aac_start;
}

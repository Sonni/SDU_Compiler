#include <string.h>
#include "peephole.h"
#include "../../utils/memory/memory.h"
#include <stdio.h>
#include <stdlib.h>

bool has_changed;

void peephole(abstract_asm *aac, bool first)
{
    abstract_asm *res = aac;
    do
    {
        has_changed = false;
        res = aac;
        res = peep_self_mov(res);
        if (first)
            res = peep_double_mem_moves(res);

        res = peep_add_to_inc(res);
        res = peep_sub_to_dec(res);
        res = peep_mul_1(res);
        res = peep_add_sub_0(res);
        res = peep_never_used(res);
        res = peep_dead_code(res);
        res = peep_double_move(res);
        res = peep_double_label(res);
        res = peep_dead_jump(res);
        res = peep_constant_prop(res);
        //res = peep_unused_label(res);
        res = peep_unused_op(res);
        //res = peep_pop_push(res);
    } while (has_changed);
}


abstract_asm *peep_double_label(abstract_asm *aac)
{
    abstract_asm *next = aac;

    while (next->next != NULL)
    {
        next = next->next;
        if (next->instruction == I_LABEL &&
            next->previous->instruction == I_LABEL)
        {
            if (next->val.label.kind == LABEL_NOM &&
                next->previous->val.label.kind == LABEL_NOM)
            {
                peep_new_jmp(next->val.label.id, next->previous->val.label.id,
                             aac);
                next->previous->next = next->next;
                next->next->previous = next->previous;
                has_changed = true;
            }
            else if (next->previous->val.label.kind == LABEL_NOM)
            {
                if (next->previous->previous != NULL)
                {
                    peep_new_jmp(next->previous->val.label.id,
                                 next->val.label.id, aac);
                    next->previous->previous->next = next;
                    next->previous = next->previous->previous;
                    has_changed = true;
                }
            }
        }
    }

    return aac;
}

abstract_asm *peep_mul_1(abstract_asm *aac)
{
    abstract_asm *next = aac;

    while (next->next != NULL)
    {
        next = next->next;
        if (next->instruction == I_MUL && next->val.bin.left->kind == OPD_INT)
        {
            if (next->val.bin.left->val.value == 1)
            {
                next->previous->next = next->next;
                next->next->previous = next->previous;
                has_changed = true;
            }
        }
    }

    return aac;
}

abstract_asm *peep_add_sub_0(abstract_asm *aac)
{
    abstract_asm *next = aac;

    while (next->next != NULL)
    {
        next = next->next;
        if (next->instruction == I_ADD || next->instruction == I_SUB)
        {
            if (next->val.bin.left->kind == OPD_INT &&
                next->val.bin.left->val.value == 0)
            {
                next->previous->next = next->next;
                next->next->previous = next->previous;
                has_changed = true;
            }
        }
    }

    return aac;
}

abstract_asm *peep_constant_prop(abstract_asm *aac)
{
    abstract_asm *next = aac;

    while (next->next != NULL)
    {
        next = next->next;
        int i = next->instruction;
        if ((i == I_MOV) && (next->val.bin.left->kind == OPD_INT))
        {
            abstract_asm *next_use = peep_next_use(next, next->val.bin.right);
            if (next_use != NULL)
            {
                if (!peep_label_inbetween(next, next_use) &&
                    !peep_jump_inbetween(next, next_use))
                {
                    if (next_use->num_opds == 2)
                    {
                        if (next_use->val.bin.left == next->val.bin.right &&
                            next_use->instruction != I_DIV)
                        {

                            next_use->val.bin.left = next->val.bin.left;
                            has_changed = true;

                        }
                    }
                    else if (next_use->num_opds == 1)
                    {
                        if (next_use->instruction == I_PUSH)
                        {
                            next_use->val.unary.opd = next->val.bin.left;
                            has_changed = true;
                        }
                    }
                }
            }
        }
    }

    return aac;
}


abstract_asm *peep_double_mem_moves(abstract_asm *aac)
{
    abstract_asm *next = aac;

    while (next->next != NULL)
    {
        if (next->instruction == I_MOV)
        {
            int lt = next->val.bin.left->kind;
            int rt = next->val.bin.right->kind;
            if ((lt == OPD_HEAP && rt == OPD_HEAP) ||
                (lt == OPD_STACK && rt == OPD_STACK) ||
                (lt == OPD_HEAP && rt == OPD_STACK) ||
                (lt == OPD_STACK && rt == OPD_HEAP))
            {
                operand *opd = asm_make_tmp();
                operand *tmp = next->val.bin.right;
                next->val.bin.right = opd;
                abstract_asm *new_inst = NEW(abstract_asm);
                new_inst->instruction = I_MOV;
                new_inst->num_opds = 2;
                new_inst->val.bin.left = opd;
                new_inst->val.bin.right = tmp;

                next->next->previous = new_inst;

                abstract_asm *tmp2 = next->next;
                next->next = new_inst;
                new_inst->next = tmp2;
                new_inst->previous = next;
                has_changed = true;
            }
        }
        next = next->next;
    }

    return aac;
}

/* Remove:
 * mov x, x
 */
abstract_asm *peep_self_mov(abstract_asm *aac)
{
    abstract_asm *next = aac;

    while (next != NULL)
    {
        if (next->instruction == I_MOV)
        {
            if (peep_is_op_equal(next->val.bin.left, next->val.bin.right))
            {
                next->previous->next = next->next;
                next->next->previous = next->previous;
                has_changed = true;
            }
        }
        next = next->next;
    }

    return aac;
}

/* Replace:
 * add $1, reg
 * ---with---
 * inc reg
 */
abstract_asm *peep_add_to_inc(abstract_asm *aac)
{
    abstract_asm *next = aac;

    while (next != NULL)
    {
        if (next->instruction == I_ADD)
        {
            if (next->val.bin.left->val.value == 1)
            {
                next->instruction = I_INC;
                next->val.unary.opd = next->val.bin.right;
                next->num_opds = 1;
                next->debug_code = "Inc reg";
                has_changed = true;
            }
        }
        next = next->next;
    }

    return aac;
}

/* Replace:
 * sub $1, reg
 * ---with---
 * dec reg
 */
abstract_asm *peep_sub_to_dec(abstract_asm *aac)
{
    abstract_asm *next = aac;

    while (next != NULL)
    {
        if (next->instruction == I_SUB)
        {
            if (next->val.bin.left->val.value == 1)
            {
                next->instruction = I_DEC;
                next->val.unary.opd = next->val.bin.right;
                next->debug_code = "Dec reg";
                next->num_opds = 1;

                has_changed = true;
            }
        }
        next = next->next;
    }

    return aac;
}

/* Replace:
 * mov x, y
 * mov y, z
 * --with--
 * mov x, y
 * mov x, z
 */
abstract_asm *peep_double_move(abstract_asm *aac)
{
    abstract_asm *next = aac;

    while (next->next != NULL)
    {
        next = next->next;
        int i = next->previous->instruction;
        if (i == I_MOV)
        {
            if (next->num_opds == 2 &&
                next->previous->val.bin.left != next->val.bin.left &&
                    next->previous->val.bin.right != &ASM_XMM0 &&
                    next->previous->val.bin.left != &ASM_XMM0)
            {
                if (peep_is_op_equal(next->previous->val.bin.right,
                                     next->val.bin.left))
                {
                    if (!(is_memory(next->previous->val.bin.left) &&
                          is_memory(next->val.bin.right)))
                    {
                        next->val.bin.left = next->previous->val.bin.left;
                        has_changed = true;

                    }
                }
            }
        }
    }

    return aac;
}

/* Replace:
 * jmp LABEL
 * inst
 * inst
 * someLabel:
 * ---with---
 * jmp LABEL
 * someLabel:
 */

abstract_asm *peep_dead_code(abstract_asm *aac)
{
    abstract_asm *next = aac;

    while (next->next != NULL)
    {
        next = next->next;
        if (next->instruction != I_LABEL &&
            next->previous->instruction == I_JMP)
        {
            if (peep_is_op_equal(next->previous->val.unary.opd,
                                 next->val.bin.left))
            {
                next->previous->next = next->next;
                next->next->previous = next->previous;
                has_changed = true;
            }
        }
    }

    return aac;
}

/* Replace: (Only if t1 is not used)
 * mov x, t1
 * inst
 * inst
 * mov t1, t2
 * ---with---
 * mov x, t1
 * inst
 * inst
 * mov x, t2
 */
abstract_asm *peep_constant_fold(abstract_asm *aac)
{
    abstract_asm *next = aac;


    while (next->next != NULL)
    {
        next = next->next;

        if (next->num_opds == 2)
        {
            abstract_asm *next_use = peep_next_use(next, next->val.bin.left);

            if (next_use != NULL &&
                peep_is_op_equal(next->previous->val.bin.left,
                                 next->val.bin.left) &&
                !peep_label_inbetween(next, next_use) &&
                !peep_is_used(next->next, next_use, next->val.bin.left) &&
                !peep_jump_inbetween(next, next_use) &&
                next_use->val.bin.left != next->val.bin.left)
            {
                if (next_use->num_opds == 2)
                {
                    next_use->val.bin.left = next->val.bin.left;
                    has_changed = true;
                }
            }
        }
    }


    return aac;
}

/* Replace:
 * jge / ... / jmp Label
 * Label:
 * -with-
 * Label:
 */
abstract_asm *peep_dead_jump(abstract_asm *aac)
{
    abstract_asm *next = aac;

    while (next != NULL)
    {
        int i = next->instruction;
        if (i == I_JE || i == I_JNE || i == I_JG || i == I_JGE ||
            i == I_JL || i == I_JLE || i == I_JMP)
        {
            if (next->next != NULL && next->next->instruction == I_LABEL)
            {

                if (strcmp(next->val.unary.opd->val.id,
                           next->next->val.label.id) == 0)
                {
                    next->previous->next = next->next;
                    next->next->previous = next->previous;
                    has_changed = true;
                }
            }
        }
        next = next->next;
    }

    return aac;
}

/* Remove:
 * pop x
 * push x
 */
abstract_asm *peep_pop_push(abstract_asm *aac)
{
    abstract_asm *next = aac;

    while (next->next != NULL)
    {
        next = next->next;
        int i = next->instruction;
        if (i == I_PUSH && next->previous->instruction == I_POP)
        {
            if (peep_is_op_equal(next->val.unary.opd,
                                 next->previous->val.unary.opd))
            {
                next->previous->previous->next = next->next;
                next->next->previous = next->previous->previous;
                has_changed = true;

            }
        }
    }

    return aac;
}

/* Remove:
 * labels that are never used
 */
abstract_asm *peep_unused_label(abstract_asm *aac)
{
    abstract_asm *next = aac;

    while (next != NULL)
    {
        if (next->instruction == I_LABEL && next->val.label.kind == LABEL_NOM)
        {
            if (!peep_is_label_used(aac, next->val.label.id))
            {
                next->previous->next = next->next;
                next->next->previous = next->previous;
                has_changed = true;
            }
        }
        next = next->next;

    }

    return aac;
}

/* Remove: (if y is not used elsewhere)
 * add/sub x, y
 */
abstract_asm *peep_unused_op(abstract_asm *aac)
{
    abstract_asm *next = aac;

    while (next != NULL)
    {
        if (next->instruction == I_ADD || next->instruction == I_SUB)
        {
            if (!peep_is_used(aac, next, next->val.bin.right) &&
                !peep_is_used(next->next, NULL, next->val.bin.right))
            {
                next->previous->next = next->next;
                if (next->next != NULL)
                    next->next->previous = next->previous;
                has_changed = true;
            }
        }
        next = next->next;

    }

    return aac;
}

bool peep_is_label_used(abstract_asm *start, char *label)
{
    abstract_asm *next = start;

    while (next != NULL)
    {
        int i = next->instruction;
        if (i == I_JE || i == I_JNE || i == I_JG || i == I_JGE ||
            i == I_JL || i == I_JLE || i == I_JMP || i == I_JB)
        {
            if (next->val.unary.opd->kind == OPD_LABEL &&
                strcmp(next->val.unary.opd->val.id, label) == 0)
            {

                return true;
            }
        }
        next = next->next;
    }
    return false;
}

bool peep_label_inbetween(abstract_asm *from, abstract_asm *to)
{
    abstract_asm *next = from;

    while (next->next != to)
    {
        next = next->next;
        if (next->instruction == I_LABEL)
        {
            return true;
        }
    }

    return false;
}

abstract_asm *peep_never_used(abstract_asm *aac)
{
    abstract_asm *next = aac;

    while (next->next != NULL)
    {
        next = next->next;

        if (next->instruction == I_MOV)
        {
            if (!peep_is_used(aac, NULL, next->val.bin.right) &&
                next->val.bin.left != &ASM_RAX &&
                next->val.bin.left != &ASM_RBP &&
                next->val.bin.left != &ASM_RSP &&
                next->val.bin.right->kind != OPD_HEAP &&
                next->val.bin.right->kind != OPD_STACK)
            {
                next->previous->next = next->next;
                if (next->next != NULL)
                {
                    next->next->previous = next->previous;
                }
                has_changed = true;
            }
        }
    }

    return aac;
}

bool peep_is_op_equal(operand *opd1, operand *opd2)
{
    if (opd1->kind != opd2->kind)
    {
        return false;
    }

    switch (opd1->kind)
    {
        case OPD_INT:
            return opd1->val.value == opd2->val.value;
        case OPD_FLOAT:
            return opd1->val.f_value == opd2->val.f_value;
        case OPD_TMP:
            return opd1->tmp_id == opd2->tmp_id;
        case OPD_REGISTER:
        case OPD_LABEL:
            return !strcmp(opd1->val.id, opd2->val.id);
        case OPD_HEAP:
            return peep_is_op_equal(opd1->val.heap_offset_reg,
                                    opd2->val.heap_offset_reg);
        case OPD_STACK:
            return peep_is_op_equal(opd1->val.stack_reg.stack_reg,
                                    opd2->val.stack_reg.stack_reg);
        case OPD_TMP_SPILL:
            return opd1 == opd2;
        case OPD_STRING:
            return false;
    }

    return false;
}

bool peep_recursive_compare(operand *opd1, operand *opd2)
{
    if (opd1->kind == opd2->kind)
    {
        return peep_is_op_equal(opd1, opd2);
    }
    if (opd1->kind == OPD_HEAP)
    {
        return peep_recursive_compare(opd1->val.heap_offset_reg, opd2);
    }
    if (opd1->kind == OPD_STACK)
    {
        return peep_recursive_compare(opd1->val.stack_reg.stack_reg, opd2);
    }
    return false;
}

bool peep_is_used(abstract_asm *from, abstract_asm *to, operand *op)
{
    while (from->next != NULL && from != to)
    {
        if (from->num_opds == 1 &&
            peep_recursive_compare(from->val.unary.opd, op))
        {
            return true;
        }
        if (from->num_opds == 2 &&
            (peep_recursive_compare(from->val.bin.left, op)
             || (from->instruction != I_MOV &&
                 peep_recursive_compare(from->val.bin.right, op))
             || (from->instruction == I_MOV &&
                 from->val.bin.right->kind != OPD_TMP &&
                 peep_recursive_compare(from->val.bin.right, op))))
        {
            return true;
        }
        from = from->next;
    }
    return false;
}

abstract_asm *peep_next_use(abstract_asm *from, operand *opd)
{
    abstract_asm *next = from;
    while (next->next != NULL)
    {
        next = next->next;
        if (next->num_opds == 1 &&
            peep_recursive_compare(next->val.unary.opd, opd))
        {
            return next;
        }
        if (next->num_opds == 2 &&
            (peep_recursive_compare(next->val.bin.left, opd) ||
             peep_recursive_compare(next->val.bin.right, opd)))
        {
            return next;
        }
    }
    return NULL;
}

bool
peep_aac_inbetween(abstract_asm *from, abstract_asm *to, abstract_asm *test)
{
    abstract_asm *next = from;
    while (next->next != NULL)
    {
        next = next->next;
        if (next == test)
            return true;
        if (next == to)
            return false;
    }
    return false;
}

bool peep_jump_inbetween(abstract_asm *from, abstract_asm *to)
{
    abstract_asm *next = from;
    int inst;
    while (next->next != to)
    {
        next = next->next;
        inst = next->instruction;
        if (inst == I_JE || inst == I_JNE || inst == I_JG || inst == I_JGE ||
            inst == I_JL || inst == I_JLE || inst == I_JMP || inst == I_CALL ||
            inst == I_RET)
        {
            return true;
        }
    }

    return false;
}

bool is_memory(operand *opd)
{
    if (opd == &ASM_HEAP)
        return true;


    switch (opd->kind)
    {
        case OPD_HEAP:
        case OPD_STACK:
        case OPD_TMP_SPILL:
            return true;
        case OPD_STRING:
        case OPD_INT:
        case OPD_FLOAT:
        case OPD_TMP:
        case OPD_REGISTER:
            return false;
        case OPD_LABEL:
            return true;
    }
    return true;
}

void peep_new_jmp(char *old_label, char *new_label, abstract_asm *aac)
{
    abstract_asm *next = aac;

    while (next->next != NULL)
    {
        next = next->next;
        int i = next->instruction;
        if (i == I_JE || i == I_JNE || i == I_JG || i == I_JGE ||
            i == I_JL || i == I_JLE || i == I_JMP || i == I_JB)
        {
            char *n_buf = malloc(sizeof(char) * 100);
            sprintf(n_buf, "%s", next->val.unary.opd->val.id);

            if (strcmp(n_buf, old_label) == 0)
            {
                sprintf(n_buf, "%s", new_label);
                next->val.unary.opd->val.id = n_buf;
            }
        }
    }
}

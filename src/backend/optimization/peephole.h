#ifndef COMPILER_PEEPHOLE_H
#define COMPILER_PEEPHOLE_H

#include "../asm/asm.h"
#include "../../utils/util.h"


void peephole(abstract_asm *aac, bool first);

abstract_asm *peep_test(abstract_asm *aac);

abstract_asm *peep_double_mem_moves(abstract_asm *aac);
abstract_asm *peep_self_mov(abstract_asm *aac);
abstract_asm *peep_mul_1(abstract_asm *aac);
abstract_asm *peep_add_sub_0(abstract_asm *aac);
abstract_asm *peep_add_to_inc(abstract_asm *aac);
abstract_asm *peep_sub_to_dec(abstract_asm *aac);
abstract_asm *peep_double_move(abstract_asm *aac);
abstract_asm *peep_never_used(abstract_asm *aac);
abstract_asm *peep_dead_code(abstract_asm *aac);
abstract_asm *peep_constant_prop(abstract_asm *aac);
abstract_asm *peep_constant_fold(abstract_asm *aac);
abstract_asm *peep_dead_jump(abstract_asm *aac);
abstract_asm *peep_unused_label(abstract_asm *aac);
abstract_asm *peep_unused_op(abstract_asm *aac);
abstract_asm *peep_pop_push(abstract_asm *aac);
abstract_asm *peep_double_label(abstract_asm *aac);

bool peep_is_label_used(abstract_asm *start, char *label);
bool peep_label_inbetween(abstract_asm *from, abstract_asm *to);
bool peep_is_op_equal(operand *opd1, operand *opd2);
bool peep_is_used(abstract_asm *from, abstract_asm *to, operand *op);
abstract_asm *peep_next_use(abstract_asm *from, operand *opd);
bool peep_jump_inbetween(abstract_asm *from, abstract_asm *to);
bool is_memory(operand *opd);
bool peep_aac_inbetween(abstract_asm *from, abstract_asm *to,
                        abstract_asm *test);
void peep_new_jmp(char *old_label, char *new_label, abstract_asm *aac);

#endif

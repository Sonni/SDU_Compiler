#ifndef COMPILER_GARBAGE_COLLECTOR_H
#define COMPILER_GARBAGE_COLLECTOR_H

#include "asm.h"

abstract_asm *asm_set_bits(operand *tmp_heap_loc);
abstract_asm *asm_dec_reference();
abstract_asm *asm_inc_reference(operand *var);
abstract_asm *asm_find_free_loc();
abstract_asm *asm_garbage_collect(operand *var, operand *length);
abstract_asm *asm_call_collect(operand *var);
abstract_asm *asm_gb_free(operand *var);

#endif

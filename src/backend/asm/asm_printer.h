#ifndef COMPILER_ASM_PRINTER_H
#define COMPILER_ASM_PRINTER_H


#include "asm.h"

#define ASM_INDENT 30

char *print_operand(operand *o);
void print_abstract_asm(abstract_asm *a_asm);

#endif

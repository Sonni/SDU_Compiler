#ifndef COMPILER_REG_ALLOC_H
#define COMPILER_REG_ALLOC_H

/*
 * reg_alloc uses the information from the livesness analysis to built up a
 * interference graph. This graph is then color with k colors equal to the
 * numbers of registers, a color then corrosponds to a register.
 *
 * The coloring of the graph is done by push all verticies with edges < k.
 * When no vertex has lower than k edges, then the vertex with most edges is
 * pushed to the stack. Next step is to rebuilt the graph and color the
 * verticies on the fly. If no color can be assigned to a vertex, then it is
 * spilled and has to be placed on the stack.
 */

#include "../asm/asm.h"

int *reg_alloc(abstract_asm *aac);

#endif

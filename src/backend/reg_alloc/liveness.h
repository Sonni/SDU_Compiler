#ifndef COMPILER_LIVENESS_H
#define COMPILER_LIVENESS_H

/*
 * Liveness analysis does analysis on temporaries, if two tmps are live at the
 * same time, then they cannot share a register in that part of the program.
 *
 * The algorithm for the analysis has been taking from:
 *
 * Appel: Modern Compiler Implementation in C Page 221
 *
 * in[n] = use[n] union (out[n]-def[n])
 * out[n] = union in[s]     s in succ[n]
 *
 * This is repeated until no change has occurred
 */

#include "../asm/asm.h"
#include "../../utils/vector.h"
#include "graph.h"

typedef struct flow_graph
{
    int num_nodes;
    vector *nodes;
} flow_graph;

typedef struct flow_node
{
    vector *succ;
    vector *pred;
    int num_succ;
    bit_array bit_succ;
    abstract_asm *aac;
    int tmp_id;
    int color;
} flow_node;


flow_graph *liveness_flow_graph(abstract_asm *aac);
void liveness(abstract_asm *aac);

#endif

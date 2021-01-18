#include <stdio.h>
#include <stdlib.h>
#include "reg_alloc.h"
#include "liveness.h"
#include "../../utils/memory/memory.h"
#include "bit_array.h"


int *color;
bit_array spilled_tmps;
int stack_offset = 2;
bool run_again;
bit_array bit_graph;

abstract_asm *reg_alloc_rewrite_spill(operand **opd, operand **tmp,
                                      bool save_on_stack)
{
    int id = get_reg_num((*opd));
    if (id == -1)
    {
        return NULL;
    }
    if (bit_test(spilled_tmps, id))
    {
        abstract_asm *aac_cur = NULL, *aac_start = NULL;
        operand *opd_spill, *tmp_opd, **cur_opd = NULL;
        switch ((*opd)->kind)
        {
            case OPD_HEAP:
                cur_opd = &(*opd)->val.heap_offset_reg;
                break;
            case OPD_STACK:
                cur_opd = &(*opd)->val.stack_reg.stack_reg;
                break;
            case OPD_TMP:
                cur_opd = opd;
                break;
            case OPD_TMP_SPILL:
            case OPD_REGISTER:
            case OPD_LABEL:
            case OPD_INT:
            case OPD_FLOAT:
            case OPD_STRING:
                return NULL;
                break;
        }

        if ((*cur_opd)->val.tmp.spilled == NULL)
        {
            (*cur_opd)->val.tmp.spilled = make_spilled_tmp();
        }

        opd_spill = (*cur_opd)->val.tmp.spilled;
        if (opd_spill->offset == -1)
        {
            opd_spill->offset = stack_offset;
            stack_offset++;
        }
        if (tmp == NULL)
        {
            tmp_opd = asm_make_tmp();
            replace_tmp_op(cur_opd, tmp_opd);
            run_again = true;

        }
        else if ((*tmp) == NULL)
        {
            tmp_opd = asm_make_tmp();
            (*tmp) = tmp_opd;
            run_again = true;
        }
        else
        {
            tmp_opd = (*tmp);
            replace_tmp_op(cur_opd, tmp_opd);

        }
        if (save_on_stack)
        {
            asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, tmp_opd, opd_spill,
                              "Save spilled tmp on stack");
        }
        else
        {
            asm_make_bin_inst(&aac_cur, &aac_start, I_MOV, opd_spill, tmp_opd,
                              "load spilled tmp from stack");
        }

        return aac_start;
    }
    return NULL;
}


abstract_asm *reg_alloc_rewrite_spills(abstract_asm *aac)
{
    abstract_asm *aac_cur = NULL, *aac_start = NULL;
    abstract_asm *list = aac;
    while (list != NULL)
    {
        abstract_asm *next = list->next;
        operand *tmp = NULL;
        switch (list->instruction)
        {
            case I_CALL_START:
            case I_CALL_END:
                asm_combine_inst(list, &aac_cur, &aac_start);
                break;
            case I_DIV:
            case I_POP:
                asm_combine_inst(list, &aac_cur, &aac_start);
                asm_combine(reg_alloc_rewrite_spill(&list->val.unary.opd, NULL,
                                                    true), &aac_cur,
                            &aac_start);
                break;
            case I_MOV:
                asm_combine(reg_alloc_rewrite_spill(&list->val.bin.left, NULL,
                                                    false), &aac_cur,
                            &aac_start);
                asm_combine_inst(list, &aac_cur, &aac_start);
                asm_combine(reg_alloc_rewrite_spill(&list->val.bin.right, NULL,
                                                    true), &aac_cur,
                            &aac_start);
                break;
            case I_LSHIFT:
            case I_RSHIFT:
            case I_ARSHIFT:
            case I_MUL:
            case I_AND:
            case I_OR:
            case I_ADD:
            case I_SUB:
            case I_XOR:
                asm_combine(reg_alloc_rewrite_spill(&list->val.bin.left, NULL,
                                                    false), &aac_cur,
                            &aac_start);
                asm_combine(reg_alloc_rewrite_spill(&list->val.bin.right, &tmp,
                                                    false), &aac_cur,
                            &aac_start);
                asm_combine_inst(list, &aac_cur, &aac_start);
                asm_combine(reg_alloc_rewrite_spill(&list->val.bin.right, &tmp,
                                                    true), &aac_cur,
                            &aac_start);
                break;
            case I_PUSH:
                asm_combine(reg_alloc_rewrite_spill(&list->val.unary.opd, NULL,
                                                    false), &aac_cur,
                            &aac_start);
                asm_combine_inst(list, &aac_cur, &aac_start);
                break;
            case I_JE:
            case I_JNE:
            case I_JG:
            case I_JGE:
            case I_JL:
            case I_JLE:
            case I_LABEL:
            case I_JMP:
            case I_CALL:
            case I_RET:
            case I_INT:
            case I_CLTD:
            case I_JB:
                asm_combine_inst(list, &aac_cur, &aac_start);
                break;
            case I_CMP:
            case I_BT:
            case I_BTS:
                asm_combine(reg_alloc_rewrite_spill(&list->val.bin.left, NULL,
                                                    false), &aac_cur,
                            &aac_start);
                asm_combine(reg_alloc_rewrite_spill(&list->val.bin.right, NULL,
                                                    false), &aac_cur,
                            &aac_start);
                asm_combine_inst(list, &aac_cur, &aac_start);
                break;
            case I_INC:
            case I_DEC:
            case I_NEG:
                asm_combine(reg_alloc_rewrite_spill(&list->val.unary.opd, &tmp,
                                                    false), &aac_cur,
                            &aac_start);
                asm_combine_inst(list, &aac_cur, &aac_start);
                asm_combine(reg_alloc_rewrite_spill(&list->val.unary.opd, &tmp,
                                                    true), &aac_cur,
                            &aac_start);
                break;
            case I_CVTSS2SD:
                break;
        }
        list = next;
    }
    return aac_start;

}

int find_node_with_most_succ(vector *node)
{
    int res = 0;

    for (int i = 1; i < (int) vector_size(node); i++)
    {
        flow_node *cur_node = vector_get_element(node, i);
        flow_node *res_node = vector_get_element(node, res);
        if (vector_size(res_node->succ) < vector_size(cur_node->succ))
        {
            res = i;
        }
    }
    return res;
}


flow_graph *reg_alloc_built_conflict_graph(abstract_asm *aac)
{
    // Insert graph nodes
    flow_graph *fg = NEW(flow_graph);
    fg->nodes = vector_init();
    for (int i = 0; i < num_tmp; i++)
    {
        flow_node *node = NEW(flow_node);
        node->color = -1;
        node->succ = vector_init();
        node->pred = vector_init();
        node->tmp_id = i;
        node->bit_succ = bit_init();
        vector_push(fg->nodes, node);
    }

    // Adding edge between tmps that are in same live-out list of an inst
    abstract_asm *code = aac;
    while (code != NULL)
    {
        // mov tmp1, tmp2 is not a conflict
        int skip = -1;
        if (code->instruction == I_MOV && get_reg_num(code->val.bin.left) != -1
            && get_reg_num(code->val.bin.right) != -1)
        {
            // skip = get_reg_num(code->val.bin.right);
        }

        for (int i = 0; i < num_tmp; i++)
        {
            if (bit_test(code->def, i))
            {
                flow_node *node_i = vector_get_element(fg->nodes, i);
                for (int j = 0; j < num_tmp; j++)
                {
                    if (i != j && bit_test(code->live_out, j) && skip != j)
                    {
                        flow_node *node_j = vector_get_element(fg->nodes,
                                                               j);
                        vector_push(node_i->succ, node_j);
                        vector_push(node_j->succ, node_i);
                        bit_set(node_i->bit_succ, node_j->tmp_id);
                        bit_set(node_j->bit_succ, node_i->tmp_id);

                    }
                }
            }
        }

        code = code->next;
    }
    return fg;
}

vector *reg_alloc_simplify(flow_graph *fg)
{
    vector *stack = vector_init();
    bool has_changed = true;

    while (vector_size(fg->nodes) != 0)
    {
        while (has_changed)
        {
            has_changed = false;
            for (int i = 0; i < (int) vector_size(fg->nodes); i++)
            {
                flow_node *node = vector_get_element(fg->nodes, i);
                if (vector_size(node->succ) < NUM_REG)
                {
                    has_changed = true;
                    vector_push(stack, node);
                    vector_remove_element(fg->nodes, i);
                    i = 0;
                }
            }
        }
        if (vector_size(fg->nodes) == 0)
        {
            break;
        }
        int i = find_node_with_most_succ(fg->nodes);
        flow_node *node = vector_get_element(fg->nodes, i);
        vector_push(stack, node);
        vector_remove_element(fg->nodes, i);
        has_changed = true;
    }

    return stack;
}

void reg_alloc_color(vector *stack)
{
    // Color each node
    for (int i = 0; i < NUM_REG; i++)
    {
        bit_set(bit_graph, i);
    }
    for (int i = vector_size(stack) - 1; i >= 0; i--)
    {
        flow_node *node = vector_get_element(stack, i);
        if (color[node->tmp_id] != -1)
        {
            continue;
        }
        bool found_reg = false;
        for (int k = 0; k < NUM_REG; k++)
        {
            bool can_use_reg = true;
            for (int j = 0; j < (int) vector_size(node->succ); j++)
            {
                flow_node *succ = vector_get_element(node->succ, j);
                if (bit_test(node->bit_succ, succ->tmp_id) &&
                    bit_test(bit_graph, succ->tmp_id))
                {

                    if (color[succ->tmp_id] == k && color[succ->tmp_id] != -1)
                    {
                        can_use_reg = false;
                        break;
                    }
                }
            }
            if (can_use_reg)
            {
                color[node->tmp_id] = k;
                node->color = k;
                found_reg = true;
                break;
            }
        }
        // No color was found, tmp must be spilled
        if (!found_reg)
        {
            bit_set(spilled_tmps, node->tmp_id);
        }
        else
        {
            bit_set(bit_graph, node->tmp_id); // 'add' edges
        }
    }
}


int *reg_alloc(abstract_asm *aac)
{
    do
    {
        liveness(aac);
        color = malloc(sizeof(int) * num_tmp);
        run_again = false;
        spilled_tmps = bit_init();
        bit_graph = bit_init();

        for (int i = 0; i < NUM_REG; i++)
            color[i] = i;
        for (int i = NUM_REG; i < num_tmp; i++)
            color[i] = -1;


        flow_graph *fg = reg_alloc_built_conflict_graph(aac);
        vector *stack = reg_alloc_simplify(fg);
        reg_alloc_color(stack);
        if (!bit_empty(spilled_tmps))
        {
            reg_alloc_rewrite_spills(aac);
        }

    }
    while (run_again);

    return color;
}

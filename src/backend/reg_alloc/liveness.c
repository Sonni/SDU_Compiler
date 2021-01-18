#include <string.h>
#include <stdio.h>
#include "liveness.h"
#include "../../utils/memory/memory.h"
#include "../../utils/string_util.h"
#include "../../utils/definitions.h"

void liveness_set_opd(bit_array use, bit_array def, operand *op, bool set_use,
                      bool set_def)
{
    int reg;
    switch (op->kind)
    {
        case OPD_HEAP:
            liveness_set_opd(use, def, op->val.heap_offset_reg, true, false);
            break;
        case OPD_STACK:
            liveness_set_opd(use, def, op->val.stack_reg.stack_reg,
                             true, false);
            break;
        case OPD_REGISTER:
        case OPD_TMP:
            reg = get_reg_num(op);

            if (reg != -1)
            {

                if (set_use)
                {
                    bit_set(use, reg);
                }
                if (set_def)
                {
                    bit_set(def, reg);
                }
            }
            break;
        case OPD_LABEL:
        case OPD_TMP_SPILL:
        case OPD_INT:
        case OPD_FLOAT:
        case OPD_STRING:
            break;
    }
}

void liveness_set_use_def(abstract_asm *aac)
{
    switch (aac->instruction)
    {
        case I_CALL_START:
        case I_CALL_END:
            break;
        case I_MUL:
        case I_DIV:
            liveness_set_opd(aac->use, aac->def, aac->val.bin.left,
                             true, true);
            liveness_set_opd(aac->use, aac->def, aac->val.bin.right,
                             true, true);
            break;
        case I_MOV:
            liveness_set_opd(aac->use, aac->def, aac->val.bin.left,
                             true, false);
            liveness_set_opd(aac->use, aac->def, aac->val.bin.right,
                             false, true);
            break;
        case I_AND:
        case I_OR:
        case I_ADD:
        case I_SUB:
        case I_XOR:
            liveness_set_opd(aac->use, aac->def, aac->val.bin.left,
                             true, false);
            liveness_set_opd(aac->use, aac->def, aac->val.bin.right,
                             true, true);
            break;
        case I_PUSH:
            liveness_set_opd(aac->use, aac->def, aac->val.unary.opd,
                             true, false);
            break;
        case I_POP:
            liveness_set_opd(aac->use, aac->def, aac->val.unary.opd,
                             false, true);
            break;
        case I_RET:
            bit_set(aac->use, get_reg_num(&ASM_RAX));
            break;
        case I_CALL:
            bit_set(aac->def, get_reg_num(&ASM_RAX));
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
        case I_LABEL:
        case I_JB:
        case I_JMP:
            break;
        case I_BT:
            liveness_set_opd(aac->use, aac->def, aac->val.bin.left,
                             true, false);
            liveness_set_opd(aac->use, aac->def, aac->val.bin.right,
                             true, false);
            break;
        case I_BTS:
            liveness_set_opd(aac->use, aac->def, aac->val.bin.left,
                             true, false);
            liveness_set_opd(aac->use, aac->def, aac->val.bin.right,
                             true, true);
            break;
        case I_CLTD:
            bit_set(aac->def, get_reg_num(&ASM_RAX));
            bit_set(aac->def, get_reg_num(&ASM_RDX));
            bit_set(aac->use, get_reg_num(&ASM_RAX));
            break;
        case I_CMP:
            liveness_set_opd(aac->use, aac->def, aac->val.bin.left,
                             true, false);
            liveness_set_opd(aac->use, aac->def, aac->val.bin.right,
                             true, false);
            break;
        case I_INC:
        case I_DEC:
        case I_NEG:
            liveness_set_opd(aac->use, aac->def, aac->val.unary.opd,
                             true, true);
            break;
        case I_CVTSS2SD:
            break;
    }

    if (print_use_def && !print_liveness)
    {
        char debug[500];
        sprintf(debug, "use: %s, def: %s", bit_get_string(aac->use),
                bit_get_string(aac->def));
        aac->debug_code = cpy_str(debug);
    }
}

g_node_list *liveness_find_label(g_graph *g, char *label)
{
    g_node_list *list = g_get_nodes(g);
    abstract_asm *i;

    while (list != NULL)
    {
        i = (abstract_asm *) g_node_info(list->head);
        if (i->instruction == I_LABEL && i->val.label.kind != LABEL_FUNC)
        {
            if (strcmp(i->val.label.id, label) == 0)
            {
                return list;
            }
        }
        list = list->tail;
    }
    fprintf(stderr, "Could not find label: %s\n", label);
    return NULL;
}


g_graph *liveness_create_flow_graph(abstract_asm *aac)
{
    g_graph *g = g_init_graph();
    abstract_asm *i = aac;
    g_node *last = NULL;
    g_node_list *nodes, *jump;

    while (i != NULL)
    {
        g_init_node(g, i);
        i = i->next;
    }

    nodes = g_get_nodes(g);
    while (nodes != NULL)
    {
        if (last != NULL)
        {
            g_add_edge(last, nodes->head);
        }
        last = nodes->head;
        i = (abstract_asm *) g_node_info(nodes->head);

        switch (i->instruction)
        {
            case I_CALL_START:
            case I_CALL_END:
            case I_MUL:
            case I_DIV:
                break;
            case I_MOV:
                break;
            case I_AND:
            case I_OR:
            case I_ADD:
            case I_SUB:
            case I_XOR:
                break;
            case I_PUSH:
                break;
            case I_POP:
                break;
            case I_CALL:
                break;
            case I_LSHIFT:
                break;
            case I_RSHIFT:
                break;
            case I_ARSHIFT:
            case I_BT:
            case I_BTS:
                break;
            case I_JE:
            case I_JNE:
            case I_JG:
            case I_JGE:
            case I_JL:
            case I_JLE:
            case I_JB:
                jump = liveness_find_label(g, i->val.unary.opd->val.id);
                g_add_edge(nodes->head, jump->head);
                break;
            case I_LABEL:
                break;
            case I_JMP:
                jump = liveness_find_label(g, i->val.unary.opd->val.id);
                g_add_edge(nodes->head, jump->head);
            case I_RET:
                last = NULL;
                break;
            case I_CMP:
                break;
            case I_INC:
            case I_DEC:
            case I_NEG:
            case I_INT:
            case I_CVTSS2SD:
            case I_CLTD:
                break;
        }
        nodes = nodes->tail;
    }
    return g;
}

void liveness(abstract_asm *aac)
{
    abstract_asm *code;
    g_graph *fg;
    bool has_changed;
    g_node_list *list;


    code = aac;
    while (code != NULL)
    {
        code->def = bit_init();
        code->use = bit_init();
        code->live_in = bit_init();
        code->live_out = bit_init();
        liveness_set_use_def(code);
        code = code->next;
    }


    fg = liveness_create_flow_graph(aac);

    do
    {
        has_changed = false;
        list = g_get_nodes(fg);

        while (list != NULL)
        {
            abstract_asm *i = (abstract_asm *) g_node_info(list->head);
            g_node_list *succ;
            bit_array in = i->live_in;
            bit_array out = i->live_out;
            i->live_in = bit_union(i->use, bit_diff(i->live_out, i->def));
            succ = g_successor(list->head);
            i->live_out = bit_init();
            while (succ != NULL)
            {
                bit_set_union(i->live_out, ((abstract_asm *)
                        g_node_info(succ->head))->live_in);
                succ = succ->tail;
            }

            if (!bit_equal(in, i->live_in) || !bit_equal(out, i->live_out))
            {
                has_changed = true;
            }
            list = list->tail;
        }

    }
    while (has_changed);


    if (print_liveness)
    {
        code = aac;
        while (code != NULL)
        {
            char debug[2014];
            sprintf(debug, "Live-in: %s, live-out: %s",
                    bit_get_string(code->live_in),
                    bit_get_string(code->live_out));
            code->debug_code = cpy_str(debug);
            code = code->next;
        }
    }
}

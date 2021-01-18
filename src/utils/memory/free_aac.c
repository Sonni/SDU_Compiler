#include "free_aac.h"
#include <stdlib.h>

void free_operand(operand *opd)
{
    switch (opd->kind)
    {
        case OPD_HEAP:
            free_operand(opd->val.heap_offset_reg);
            break;
        case OPD_STACK:
            free(opd->val.stack_reg.stack_reg);
            break;
        case OPD_REGISTER:
        case OPD_TMP:
        case OPD_LABEL:
        case OPD_TMP_SPILL:
        case OPD_INT:
        case OPD_FLOAT:
        case OPD_STRING:
            break;
    }
}

void free_aac(abstract_asm *aac)
{
    while (aac->next != NULL)
    {
        aac = aac->next;
        free(aac->previous);
        if (aac->num_opds == 1)
        {
            free_operand(aac->val.unary.opd);
            free(aac->val.unary.opd);
            aac->val.unary.opd = NULL;
        }
        if (aac->num_opds == 2)
        {
            free_operand(aac->val.bin.left);
            free_operand(aac->val.bin.right);

            /*free(aac->val.bin.left);
            aac->val.bin.left = NULL;
            free(aac->val.bin.right);
            aac->val.bin.right = NULL;*/
        }
        free(aac->live_in);
        free(aac->live_out);
        free(aac->def);
        free(aac->use);
        //free(aac->debug_code);
    }
    free(aac);
}

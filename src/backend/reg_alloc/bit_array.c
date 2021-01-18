#include "bit_array.h"
#include "../../utils/memory/memory.h"
#include "../../utils/string_util.h"
#include <stdio.h>
#include <stdlib.h>

bit_array bit_init()
{
    unsigned int i;
    bit_array b = malloc(INT_SIZE);
    for (i = 0; i < BIT_ENTRIES; i++)
    {
        b[i] = 0;
    }
    return b;
}

void bit_set(bit_array a, unsigned int bit)
{
    unsigned int offset = bit / 8 / sizeof(unsigned int);
    bit -= offset * 8 * sizeof(unsigned int);
    a[offset] |= (1 << bit);
}

void bit_clear(bit_array a, unsigned int bit)
{
    unsigned int offset = bit / 8 / sizeof(unsigned int);
    bit -= offset * 8 * sizeof(unsigned int);
    a[offset] &= ~(1 << bit);
}

bool bit_test(bit_array a, unsigned int bit)
{
    unsigned int offset = bit / 8 / sizeof(unsigned int);
    bit -= offset * 8 * sizeof(unsigned int);
    return ((a[offset] & (1 << bit)) != 0);
}

bool bit_equal(bit_array a, bit_array b)
{
    for (unsigned  int i = 0; i < BIT_ENTRIES; i++)
    {
        if (a[i] != b[i])
        {
            return false;
        }
    }
    return true;
}

bit_array bit_union(bit_array a, bit_array b)
{
    bit_array res = bit_init();
    for (unsigned int i = 0; i < BIT_ENTRIES; i++)
    {
        res[i] = a[i] | b[i];   // Bitwise or every int in array
    }
    return res;
}

bit_array bit_diff(bit_array a, bit_array b)
{
    bit_array res = bit_init();
    for (unsigned int i = 0; i < BIT_ENTRIES; i++)
    {
        res[i] = a[i] & (~b[i]);    // Bitwise and and (not b)
    }
    return res;
}

bit_array bit_intersect(bit_array a, bit_array b)
{
    bit_array res = bit_init();
    for (unsigned int i = 0; i < BIT_ENTRIES; i++)
    {
        res[i] = a[i] & b[i];
    }
    return res;
}

void bit_set_union(bit_array a, bit_array b)
{
    for (unsigned int i = 0; i < BIT_ENTRIES; i++)
    {
        a[i] |= b[i];
    }
}

void bit_set_diff(bit_array a, bit_array b)
{
    for (unsigned int i = 0; i < BIT_ENTRIES; i++)
    {
        a[i] &= (~b[i]);
    }
}

void bit_set_intersect(bit_array a, bit_array b)
{
    for (unsigned int i = 0; i < BIT_ENTRIES; i++)
    {
        a[i] &= b[i];
    }
}

void bit_print(bit_array a)
{
    for (unsigned int i = 0; i < BIT_ENTRIES; i++)
    {
        if (bit_test(a, i))
        {
            printf("%i ", i);
        }
    }
    printf("\n");
}

char *bit_get_string(bit_array a)
{
    char buffer[1024] = "", *buf = buffer;
    for (unsigned int i = 0; i < BIT_ENTRIES; i++)
    {
        if (bit_test(a, i))
        {
            buf += sprintf(buf, "%i ", i);
        }
    }
    return cpy_str(buffer);
}

int bit_get_highest(bit_array a)
{
    for (unsigned int i = BIT_ENTRIES-1; i > 0; i--)
    {
        if (bit_test(a, i))
        {
            return i;
        }
    }
    return 0;
}

bool bit_empty(bit_array a)
{
    for (unsigned int i = 0; i < BIT_ENTRIES; i++)
    {
        if (a[i] != 0)
        {
            return false;
        }
    }
    return true;
}

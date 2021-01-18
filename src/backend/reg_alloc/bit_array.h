#ifndef COMPILER_BIT_ARRAY_H
#define COMPILER_BIT_ARRAY_H

#include "../../utils/util.h"

#define BIT_ARR_SIZE 4096
#define INT_SIZE (BIT_ARR_SIZE + 1) / 8
#define BIT_ENTRIES INT_SIZE / sizeof(unsigned int)

typedef unsigned int *bit_array;

bit_array bit_init();

void bit_clear(bit_array a,  unsigned int bit);
void bit_set(bit_array a, unsigned int bit);

bool bit_test(bit_array a, unsigned int bit);
bool bit_equal(bit_array a, bit_array b);

bit_array bit_union(bit_array a, bit_array b);
bit_array bit_diff(bit_array a, bit_array b);
bit_array bit_intersect(bit_array a, bit_array b);

void bit_set_union(bit_array a, bit_array b);
void bit_set_diff(bit_array a, bit_array b);
void bit_set_intersect(bit_array a, bit_array b);

void bit_print(bit_array a);
char *bit_get_string(bit_array a);

int bit_get_highest(bit_array a);

bool bit_empty(bit_array a);

#endif //COMPILER_BIT_ARRAY_H

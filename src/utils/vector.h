#ifndef COMPILER_VECTOR_H
#define COMPILER_VECTOR_H

/*
 * Implementation of the vector data structure, using doubling method.
 */

#define START_CAPACITY 5
#include <stddef.h>

/*
 * The capacity of the vector is cut in half when possible and doubled
 * when more space is needed
 */

typedef struct vector {
    void **elements;
    int capacity;
    int size;
} vector;

vector *vector_init();

void vector_delete(vector *v);

void vector_push(vector *v, void *value);

void *vector_pop(vector *v);

size_t vector_size(const vector *v);

size_t vector_capacity(const vector *v);

void *vector_get_element(const vector *v, size_t index);

void *vector_remove_element(vector *v, size_t index);

void **vector_get_array(const vector *v);

void vector_free_array(vector *v);

void vector_print(const vector *v);

void vector_resize(vector *v, int capacity);

#endif

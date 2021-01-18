#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memory/memory.h"
#include "vector.h"

void vector_resize(vector *v, int capacity)
{
    if (capacity == 0)      // Should work with 0??
	{
		capacity = 1;
	}
	v->capacity = capacity;
	v->elements = realloc(v->elements, sizeof(void *)* capacity);
}

vector *vector_init()
{
	vector *v = NEW(vector);
    v->capacity = START_CAPACITY;
    v->elements = malloc(sizeof(void *)* v->capacity);
    v->size = 0;

    return v;
}

void vector_delete(vector *v)
{
	free(v->elements);
}

void vector_push(vector *v, void *value)
{
	if (v->capacity == v->size)				// Double the capacity if necessary
	{
        vector_resize(v, v->capacity * 2);
	}

	v->elements[v->size] = value;
	v->size++;
}

void *vector_pop(vector *v) {
    void *element = malloc(sizeof(void *));
    memcpy(element, v->elements[v->size - 1], sizeof(void *));
    v->size--;

    if (v->size % v->capacity == 0) // Cut the capacity in half if possible
    {
        vector_resize(v, v->size);
    }

	return element;
}

size_t vector_size(const vector *v)
{
	return v->size;
}

size_t vector_capacity(const vector *v)
{
	return v->capacity;
}

void *vector_get_element(const vector *v, size_t index)
{
	return v->elements[index];
}

void *vector_remove_element(vector *v, size_t index)
{
    void *removed_element = v->elements[0];
    for (int i = index; i < (int) v->size - 1; i++)
    {
        removed_element = v->elements[i];
        v->elements[i] = v->elements[i + 1];
    }
    v->elements[v->size - 1] = removed_element;
    return vector_pop(v);
}

void **vector_get_array(const vector *v)
{
	return v->elements;
}

void vector_print(const vector *v)
{
	unsigned int i;
	for (i = 0; i < (unsigned int) vector_size(v); i++)
	{
		printf("%s\n", (char*) vector_get_element(v, i));
	}
}

void vector_free_array(vector *v)
{
	unsigned int i;
	for (i = 0; i < (unsigned  int) vector_size(v); i++)
	{
		free(v->elements[i]);
	}
}

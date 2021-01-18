#ifndef COMPILER_MEMORY_H
#define COMPILER_MEMORY_H

#define NEW(type) (type *)mem_alloc(sizeof(type))

void *mem_alloc(unsigned n);

#endif

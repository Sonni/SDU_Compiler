#ifndef COMPILER_SYMBOL_H
#define COMPILER_SYMBOL_H

#define HASH_SIZE 317

#include "../parser/tree.h"

typedef struct symbol {
    char *name;
    int value;
    bool know_value;
    int num_assign;
    struct symbol *next;
    struct symbol_type *sym_type;
    struct operand *asm_operand;
    int offset;
    struct symbol_table *st;
    char *new_name;
    char *string;   // Only set if symbol is a const string
    bool is_func;
    int access;
    bool is_const;
    bool is_templated;
} symbol;

typedef struct symbol_table {
    symbol *table[HASH_SIZE];
    struct symbol_table *class_scope;
    struct symbol_table *next;
} symbol_table;


/*
 * Returns a hash value of the string 'str' by (str[i] << 1) + str[i+1]
 * This hash value modulus HashSize is the index to the hash table
 */
unsigned int hash(char *str);

symbol_table *init_symbol_table();

/*
 * Creates and return a new table with its next variable pointing to 't'
 */
symbol_table *scope_symbol_table(symbol_table *t);

/*
 * Put a symbol with 'name' and 'value' in the hash table, handling collision
 * with chaining each symbol contains a pointer to the next symbol.
 */
symbol *put_symbol(symbol_table *t, char *name, struct symbol_type *type,
                   int value);

/*
 * Returns the symbol with name = 'name' closest to 't' or NULL if
 * it is not found
 */
symbol *get_symbol(symbol_table *t, char *name);

/*
 * Return the size of the table
 */
int get_table_size(symbol_table *t);

int get_class_size(symbol_table *t);

/*
 * Prints the hash table starting from 't' through t-next ending at root
 */
void dump_symbol_table(symbol_table *t);

int get_symbol_depth(symbol_table *st, char *name);

int symbol_max_depth(symbol_table *st);

symbol *get_local_symbol(symbol_table *t, char *name);

/*
 * Merge st1 with st2, rename all symbols in st2 with suffix, res in st2
 */
void merge_symbols(symbol_table *st1, symbol_table *st2, char *suffix);

symbol_table *merge_tables2(symbol_table *st1, symbol_table *res, int *offset);

symbol_table *merge_tables(symbol_table *st1);

void symbol_set_offset(symbol_table * st);

void free_symbol_table(symbol_table *t);

symbol_table *get_real_symbol_table(symbol_table *st, char *name);

#endif

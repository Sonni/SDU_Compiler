#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "symbol.h"
#include "../../utils/memory/memory.h"
#include "../../utils/string_util.h"

symbol *init_symbol(char *name, struct symbol_type *s_t, int value)
{
    symbol *s = NEW(symbol);
    s->next = NULL;
    s->value = value;
    s->name = name;
    s->num_assign = 0;
    s->sym_type = s_t;
    s->asm_operand = NULL;
    s->offset = 0;
    s->know_value = false;
    s->st = NULL;
    s->is_func = false;
    s->access = AM_PUBLIC;
    s->is_const = false;
    s->is_templated = false;
    return s;
}


unsigned int hash(char *str)
{
    unsigned int i;
    int result = str[0];

    for (i = 1; i < strlen(str); i++)
    {
        result = (result << 1) + str[i];
    }

    return result;
}

symbol_table *init_symbol_table()
{
    unsigned int i;
    symbol_table *new_table = NEW(symbol_table);

    for (i = 0; i < HASH_SIZE; i++)
    {
        new_table->table[i] = NULL;
    }
    new_table->next = NULL;
    new_table->class_scope = NULL;

    return new_table;
}

symbol_table *scope_symbol_table(symbol_table *t)
{
    symbol_table *new_table = init_symbol_table();
    new_table->next = t;
    return new_table;
}

symbol *put_symbol(symbol_table *t, char *name, struct symbol_type *s_t,
                   int value)
{
    int index = hash(name) % HASH_SIZE;

    if (t->table[index] == NULL)
    {
        t->table[index] = init_symbol(name, s_t, value);

        return t->table[index];
    }

    symbol *table_slot = t->table[index];
    if (strcmp(table_slot->name, name) == 0) { return NULL; }

    while (table_slot->next != NULL)
    {
        table_slot = table_slot->next;
        if (strcmp(table_slot->name, name) == 0) { return NULL; }
    }

    table_slot->next = init_symbol(name, s_t, value);

    return table_slot->next;
}

symbol *get_symbol(symbol_table *t, char *name)
{
    int index = hash(name) % HASH_SIZE;

    // Outer loop check all the tables, inner loop checks chain of each table
    while (t != NULL)
    {
        symbol *symbol_slot = t->table[index];
        while (symbol_slot != NULL)
        {
            if (strcmp(symbol_slot->name, name) == 0)
            {
                return symbol_slot;
            }
            symbol_slot = symbol_slot->next;
        }
        t = t->next;
    }

    return NULL;
}

/*
 * Prints the hash table starting from 't' through t-next ending at root
 */
void dump_symbol_table(symbol_table *t)
{
    if (t == NULL)
    {
        return;
    }
    unsigned int i;

    printf("Address of table is: %p pointing to %p\n",
           (void *) t, (void *) t->next);

    for (i = 0; i < HASH_SIZE; i++)
    {
        symbol *s = t->table[i];
        while (s != NULL)
        {
            printf("(%s: %d, %d) ", s->name, s->sym_type->kind, s->offset);
            s = s->next;
        }
    }
    printf("\n");

    dump_symbol_table(t->next);
}

symbol *get_local_symbol(symbol_table *t, char *name)
{
    int index = hash(name) % HASH_SIZE;
    symbol *p = t->table[index];

    while (p != NULL)
    {
        if (strcmp(p->name, name) == 0)
        {
            return p;
        }
        p = p->next;
    }
    return p;
}

int get_table_size(symbol_table *t)
{
    int size = 0;
    for (int i = 0; i < HASH_SIZE; i++)
    {
        symbol *s = t->table[i];
        while (s != NULL)
        {
            size++;
            s = s->next;
        }
    }

    return size;
}

int get_class_size(symbol_table *t)
{
    return get_table_size(merge_tables(t));
}

int get_symbol_depth(symbol_table *st, char *name)
{
    int depth = 0;
    symbol *p = get_local_symbol(st, name);
    while (p == NULL)
    {
        st = st->next;
        p = get_local_symbol(st, name);
        depth++;
    }
    return depth;
}

int symbol_max_depth(symbol_table *st)
{
    int depth = 0;
    while (st->next != NULL)
    {
        depth++;
        st = st->next;
    }
    return depth;
}


void merge_symbols(symbol_table *st1, symbol_table *st2, char *suffix)
{
    char *new_name;
    symbol *s;

    for (int i = 0; i < HASH_SIZE; i++)
    {
        symbol *sym = st2->table[i];
        while (sym != NULL)
        {
            new_name = str_concat(sym->name, suffix);
            s = put_symbol(st1, new_name, sym->sym_type, sym->value);
            if (s != NULL)
            {
                s->offset = sym->offset;
                s->access = sym->access;
                s->string = sym->string;
                s->is_const = sym->is_const;
            }
            sym = sym->next;
        }
    }
}

symbol_table *merge_tables(symbol_table *st1)
{
    symbol_table *res = init_symbol_table();
    symbol_table *next = st1;
    symbol *s;
    while (next != NULL)
    {
        for (int i = 0; i < HASH_SIZE; i++)
        {
            symbol *sym = next->table[i];
            while (sym != NULL)
            {
                s = put_symbol(res, sym->name, sym->sym_type, sym->value);
                if (s != NULL)
                {
                    s->is_func = sym->is_func;
                    s->offset = sym->offset;
                    s->access = sym->access;
                    s->string = sym->string;
                    s->is_const = sym->is_const;
                }
                sym = sym->next;

            }
        }
        next = next->next;
    }
    return res;
}

symbol_table *merge_tables2(symbol_table *st1, symbol_table *res, int *offset)
{
    if (st1 == NULL)
        return res;
    merge_tables2(st1->next, res, offset);

    int cur_offset = (*offset);
    int highest_offset = 0;
    symbol_table *next = st1;
    symbol *s;

        for (int i = 0; i < HASH_SIZE; i++)
        {
            symbol *sym = next->table[i];
            while (sym != NULL)
            {
                s = put_symbol(res, sym->name, sym->sym_type, sym->value);
                if (s != NULL)
                {
                    s->is_func = sym->is_func;
                    s->offset = sym->offset + cur_offset;
                    s->access = sym->access;
                    s->string = sym->string;
                    s->is_const = sym->is_const;
                    if (s->offset > highest_offset)
                        highest_offset = s->offset;
                }
                else
                {
                    s = get_symbol(res, sym->name);
                    symbol *tmp_s = get_symbol(st1, sym->name);
                    s = tmp_s;
                }
                sym = sym->next;

            }
        }

    (*offset) += highest_offset;

    return res;
}

void symbol_set_offset(symbol_table * st)
{
    int offset = 1;
    for (int i = 0; i < HASH_SIZE; i++)
    {
        symbol *sym = st->table[i];
        while (sym != NULL)
        {
            if (!sym->is_func)
            {
                sym->offset = offset;
                offset++;
            }
            sym = sym->next;
        }
    }
}

void free_symbol_table(symbol_table *t)
{
    for (int i = 0; i < HASH_SIZE; i++)
    {
        symbol *p = t->table[i];
        while (p != NULL)
        {
            symbol *next = p->next;
            if (p->sym_type)
                free(p->sym_type);
            if (p->asm_operand)
                free(p->asm_operand);
            free(p);
            p = next;
        }
        free(p);
    }
    free(t);
}

symbol_table *get_real_symbol_table(symbol_table *t, char *name)
{
    int index = hash(name) % HASH_SIZE;

    // Outer loop check all the tables, inner loop checks chain of each table
    while (t != NULL)
    {
        symbol *symbol_slot = t->table[index];
        while (symbol_slot != NULL)
        {
            if (strcmp(symbol_slot->name, name) == 0)
            {
                return t;
            }
            symbol_slot = symbol_slot->next;
        }
        t = t->next;
    }
    return NULL;
}

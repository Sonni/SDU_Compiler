#ifndef COMPILER_GRAPH_H
#define COMPILER_GRAPH_H

#include <stdio.h>
#include "../../utils/util.h"

// This header file has been taken from Andrew w. Appel,
// Modern Compiler implementation in C page 230

typedef struct g_graph
{
    int num_nodes;
    struct g_node_list *nodes, *last;
} g_graph;

typedef struct g_node
{
    g_graph *graph;
    int key;
    int num_succ, num_pred;
    struct g_node_list *successor;
    struct g_node_list *predecessor;
    void *info;
} g_node;

typedef struct g_node_list
{
    g_node *head;
    struct g_node_list *tail;
} g_node_list;

g_graph *g_init_graph();
g_node *g_init_node(g_graph *g, void *info);
g_node_list *g_init_node_list(g_node *head, g_node_list *tail);
g_node_list *g_get_nodes(g_graph *g);
bool g_in_node_list(g_node * a, g_node_list *l);
void g_add_edge(g_node *from, g_node *to);
void g_remove_edge(g_node *from, g_node *to);
void g_show(FILE *out, g_node_list *p, void show_info(void *));
g_node_list *g_successor(g_node *n);
g_node_list *g_predecessor(g_node *n);
g_node_list *g_adjance(g_node *n);
bool g_goes_to(g_node *a, g_node *b);
int g_degree(g_node *n);
void *g_node_info(g_node *n);
g_node_list *g_remove(g_node *n, g_node_list *list);

#endif

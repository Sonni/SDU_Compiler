#include "graph.h"
#include "../../utils/memory/memory.h"

// https://www.cs.princeton.edu/~appel/modern/c/chap10/graph.c

g_graph *g_init_graph()
{
    g_graph *g = NEW(g_graph);
    g->num_nodes = 0;
    g->nodes = NULL;
    g->last = NULL;
    return g;
}

g_node *g_init_node(g_graph *g, void *info)
{
    g_node *n = NEW(g_node);
    g_node_list *list = g_init_node_list(n, NULL);
    n->graph = g;
    n->key = g->num_nodes++;

    if (g->last == NULL)
    {
        g->nodes = g->last = list;
    }
    else
    {
        g->last = g->last->tail = list;
    }

    n->successor = NULL;
    n->predecessor = NULL;
    n->num_pred = 0;
    n->num_succ = 0;
    n->info = info;
    return n;
}

g_node_list *g_init_node_list(g_node *head, g_node_list *tail)
{
    g_node_list *list = NEW(g_node_list);
    list->head = head;
    list->tail = tail;
    return list;
}

g_node_list *g_get_nodes(g_graph *g)
{
    return g->nodes;
}

bool g_in_node_list(g_node *a, g_node_list *l)
{
    g_node_list *next = l;
    while (next != NULL)
    {
        if (a == next->head)
        {
            return true;
        }
        next = next->tail;
    }

    return false;
}

void g_add_edge(g_node *from, g_node *to)
{
    if (g_goes_to(from, to))
    {
        return;
    }
    from->num_succ++;
    to->num_pred++;
    to->predecessor = g_init_node_list(from, to->predecessor);
    from->successor = g_init_node_list(to, from->successor);
}

g_node_list *g_remove(g_node *n, g_node_list *list)
{
    if (n == list->head)
    {
        return list->tail;
    }
    else
    {
        return g_init_node_list(list->head,
                                g_remove(list->head, g_remove(n, list->tail)));
    }
}

void g_remove_edge(g_node *from, g_node *to)
{
    from->num_succ--;
    to->num_pred--;
    to->predecessor = g_remove(from, to->predecessor);
    from->successor = g_remove(to, from->successor);
}

void g_show(FILE *out, g_node_list *p, void show_info(void *))
{
    for (; p != NULL; p = p->tail)
    {
        g_node *n = p->head;
        g_node_list *q;
        if (show_info)
        {
            show_info(n->info);
        }
        fprintf(out, " (%d): ", n->key);
        for (q = g_successor(n); q != NULL; q = q->tail)
        {
            fprintf(out, "%d ", q->head->key);
            fprintf(out, "\n");
        }
    }
}

g_node_list *g_successor(g_node *n)
{
    return n->successor;
}

g_node_list *g_predecessor(g_node *n)
{
    return n->predecessor;
}

g_node_list *g_concat(g_node_list *succ, g_node_list *pred)
{
    if (succ == NULL)
    {
        return pred;
    }
    else
    {
        return g_init_node_list(succ->head, g_concat(succ->tail, pred));
    }
}

g_node_list *g_adjance(g_node *n)
{
    return g_concat(g_successor(n), g_predecessor(n));
}

bool g_goes_to(g_node *a, g_node *b)
{
    return g_in_node_list(b, g_successor(a));
}

int g_degree(g_node *n)
{
    int degree = 0;
    g_node_list *list = g_predecessor(n);
    while (list != NULL)
    {
        degree++;
        list = list->tail;
    }
    return degree;
}

void *g_node_info(g_node *n)
{
    return n->info;
}

/*

list structures and routines
Copyright (C) 2013-2017 xinoir <xinoir@theta.black>
This file is part of beta.

*/

#include <stdlib.h>
#include <string.h>
#include "beta/error.h"
#include "beta/variant.h"
#include "beta/list.h"

struct list_node {
    struct variant *data;
    struct list_node *prev;
    struct list_node *next;
};

struct list {
    struct list_node *head;
    struct list_node *tail;
    int length;
};

static int init_node(void *object, size_t size, struct list_node **node)
{
    int s;

    if ((s = variant_create(object, size, &(*node)->data)))
        return s;

    return 0;
}

static int alloc_node(struct list_node **node)
{
    if (!(*node = malloc(sizeof(struct list_node))))
        return error("failed to allocate memory for a list node");

    return 0;
}

static int create_node(void *object, size_t size, struct list_node **node)
{
    int s;

    if ((s = alloc_node(node)))
        return s;
    if ((s = init_node(object, size, node)))
        return s;

    return 0;
}

static int init_list(struct list **list)
{
    (*list)->head = (*list)->tail = NULL;
    (*list)->length = 0;

    return 0;
}

static int alloc_list(struct list **list)
{
    if (!(*list = malloc(sizeof(struct list))))
        return error("failed to allocate memory for a list");

    return 0;
}

int list_length(const struct list *list)
{
    return list->length;
}

struct list_node *list_next(const struct list_node *node)
{
    return node->next;
}

struct list_node *list_head(const struct list *list)
{
    return list->head;
}

void *list_data(const struct list_node *node, size_t *size)
{
    if (size)
        *size = variant_size(node->data);

    return variant_object(node->data);
}

void list_delete(struct list *list, const struct list_node *node)
{
    if (node->prev)
        node->prev->next = node->next;
    else
        list->head = node->next;
    if (node->next)
        node->next->prev = node->prev;
    --list->length;
}

int list_prepend(struct list *list, void *object, size_t size, struct list_node **node)
{
    int s;
    struct list_node *ln = NULL;

    if ((s = create_node(object, size, &ln)))
        return s;
    ln->next = list->head;
    ln->prev = NULL;
    if (list->head)
        list->head->prev = ln;
    list->head = ln;
    ++list->length;

    if (node)
        *node = ln;

    return 0;
}

int list_append(struct list *list, void *object, size_t size, struct list_node **node)
{
    int s = 0;
    struct list_node *ln = NULL;

    if ((s = create_node(object, size, &ln)))
        return s;
    ln->next = NULL;
    ln->prev = list->tail;
    if (list->tail)
        list->tail->next = ln;
    list->tail = ln;
    if (!list->head)
        list->head = ln;
    ++list->length;

    if (node)
        *node = ln;

    return 0;
}

void list_destroy(struct list **list)
{
    struct list_node *this, *next;
    this = next = NULL;
    if (*list)
        for (this = (*list)->head; this; this = next) {
            next = this->next;
            /*
            variant_destroy(&this->data);
            */
            free(this);
        }
    free(*list);
    *list = NULL;
}

int list_create(struct list **list)
{
    int s;

    if ((s = alloc_list(list)))
        return s;
    if ((s = init_list(list)))
        return s;

    return 0;
}


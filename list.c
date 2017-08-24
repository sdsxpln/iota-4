/*

list structures and routines
Copyright (C) 2013-2017 xinoir <xinoir@theta.black>
This file is part of beta.

*/

#include <stdlib.h>
#include <string.h>
#include "beta/error.h"
#include "beta/list.h"

struct list_node {
    void *data;
    struct list_node *prev;
    struct list_node *next;
};

struct list {
    struct list_node *head;
    struct list_node *tail;
    int length;
};

static int create_node(void *data, struct list_node **node)
{
    if (!(*node = malloc(sizeof(struct list_node))))
        return error("failed to allocate memory for a list node");
    (*node)->data = data;

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

void *list_data(const struct list_node *node)
{
    return node->data;
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

int list_prepend(struct list *list, void *data, struct list_node **node)
{
    int s = 0;
    struct list_node *new_node = NULL;

    if ((s = create_node(data, &new_node)))
        return s;
    new_node->next = list->head;
    new_node->prev = NULL;
    if (list->head)
        list->head->prev = new_node;
    list->head = new_node;
    ++list->length;
    if (node)
        *node = new_node;

    return 0;
}

int list_append(struct list *list, void *data, struct list_node **node)
{
    int s = 0;
    struct list_node *new_node = NULL;

    if ((s = create_node(data, &new_node)))
        return s;
    new_node->next = NULL;
    new_node->prev = list->tail;
    if (list->tail)
        list->tail->next = new_node;
    list->tail = new_node;
    if (!list->head)
        list->head = new_node;
    ++list->length;
    if (node)
        *node = new_node;

    return 0;
}

void list_destroy(struct list **list)
{
    struct list_node *this, *next;
    this = next = NULL;
    if (*list)
        for (this = (*list)->head; this; this = next) {
            next = this->next;
            free(this);
        }
    free(*list);
    *list = NULL;
}

int list_create(struct list **list)
{
    if (!(*list = malloc(sizeof(struct list))))
        return error("failed to allocate memory for a list");
    (*list)->head = (*list)->tail = NULL;
    (*list)->length = 0;

    return 0;
}


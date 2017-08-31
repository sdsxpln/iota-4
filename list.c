/*

list structures and routines
Copyright (C) 2013-2017 xinoir <xinoir@theta.black>
This file is part of beta.

*/

#include <stdlib.h>
#include "beta/error.h"
#include "beta/list.h"

struct list_node {
    const void *key;
    const void *object;
    struct list_node *prev;
    struct list_node *next;
};

struct list {
    int length;
    struct list_node *head;
    struct list_node *tail;
};

static struct list_node *create_node(const void *key, const void *object)
{
    struct list_node *node;

    if (!(node = (struct list_node *)malloc(sizeof(struct list_node)))) {
        error("failed to allocate memory for a list node");
        return NULL;
    }
    node->key = key;
    node->object = object;

    return node;
}

int list_length(const struct list *list)
{
    return list->length;
}

struct list_node *list_next(const struct list_node *node)
{
    return node->next;
}

struct list_node *list_tail(const struct list *list)
{
    return list->tail;
}


struct list_node *list_head(const struct list *list)
{
    return list->head;
}

void *list_node_key(const struct list_node *node)
{
    return (void *)node->key;
}

void *list_node_object(const struct list_node *node)
{
    return (void *)node->object;
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

struct list_node *list_prepend(struct list *list, const void *key, const void *object)
{
    struct list_node *node;

    if (!(node = create_node(key, object))) {
        error("failed to create a list node");
        return NULL;
    }
    node->next = list->head;
    node->prev = NULL;
    if (list->head)
        list->head->prev = node;
    list->head = node;
    ++list->length;

    return node;
}

struct list_node *list_append(struct list *list, const void *key, const void *object)
{
    struct list_node *node;

    if (!(node = create_node(key, object))) {
        error("failed to create a list node");
        return NULL;
    }
    node->next = NULL;
    node->prev = list->tail;
    if (list->tail)
        list->tail->next = node;
    list->tail = node;
    if (!list->head)
        list->head = node;
    ++list->length;

    return node;
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

struct list *list_create()
{
    struct list *list;

    if (!(list = (struct list *)malloc(sizeof(struct list)))) {
        error("failed to allocate memory for a list");
        return NULL;
    }
    list->head = list->tail = NULL;
    list->length = 0;

    return list;
}


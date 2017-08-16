/*
 * a list structures and routines
 * Copyright (C) 2013-2017 xinoir <xinoir@theta.black>
 * This file is part of beta.
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

int list_length(const struct list *list)
{
    return list->length;
}

struct list_node *list_next_node(const struct list_node *node)
{
    return node->next;
}

struct list_node *list_head(const struct list *list)
{
    return list->head;
}

void *list_node_data(const struct list_node *node, size_t *size)
{
    if (size)
        *size = variant_size(node->data);

    return variant_object(node->data);
}

void list_destroy_node(struct list_node **node)
{
    variant_destroy(&(*node)->data);
    free(*node);
    *node = NULL;
}

void list_delete_node(struct list *list, const struct list_node *node)
{
    if (node->prev)
        node->prev->next = node->next;
    else
        list->head = node->next;
    if (node->next)
        node->next->prev = node->prev;
    --list->length;
}

static int list_alloc_node(struct list_node **node)
{
    if (!(*node = malloc(sizeof(struct list_node))))
        return error("failed to allocate memory for a list node");

    return 0;
}

int list_insert_node(struct list *list, void *object, size_t size, struct list_node **node)
{
    int status;

    if ((status = list_alloc_node(node)))
        return status;
    if ((status = variant_create(object, size, &(*node)->data)))
        return status;
    (*node)->next = list->head;
    (*node)->prev = NULL;
    if (list->head)
        list->head->prev = *node;
    list->head = *node;
    ++list->length;

    return 0;
}

void list_destroy(struct list **list)
{
    struct list_node *this, *next;
    this = next = NULL;

    if (*list)
        for (this = (*list)->head; this; this = next) {
            next = this->next;
            variant_destroy(&this->data);
            free(this);
        }

    free(*list);
    *list = NULL;
}

static void list_init(struct list *list)
{
    list->head = list->tail = NULL;
    list->length = 0;
}

static int list_alloc(struct list **list)
{
    if (!(*list = malloc(sizeof(struct list))))
        return error("failed to allocate memory for a list");

    return 0;
}

int list_create(struct list **list)
{
    int status;

    if ((status = list_alloc(list)))
        return status;
    list_init(*list);

    return 0;
}


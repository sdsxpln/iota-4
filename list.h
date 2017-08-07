/*
 * a list structures and routines
 * Copyright (C) 2013-2017 xinoir <xinoir@theta.black> 
 * This file is part of beta.
 */

#ifndef LIST_H
#define LIST_H

struct list_node;
struct list;

int list_length(const struct list *list);
struct list_node *list_next_node(const struct list_node *node);
void *list_node_data(const struct list_node *node, size_t *size);
void list_destroy_node(struct list_node **node);
void list_delete_node(struct list *list, const struct list_node *node);
int list_insert_node(struct list *list, void *object, size_t size, struct list_node **node);
void list_destroy(struct list **list);
int list_create(struct list **list);

#endif


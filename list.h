/*

list structures and routines
Copyright (C) 2013-2017 xinoir <xinoir@theta.black> 
This file is part of beta.

*/

#ifndef LIST_H
#define LIST_H

struct list_node;
struct list;

int list_length(const struct list *list);
struct list_node *list_next(const struct list_node *node);
struct list_node *list_tail(const struct list *list);
struct list_node *list_head(const struct list *list);
void *list_node_key(const struct list_node *node);
void *list_node_object(const struct list_node *node);
void list_delete(struct list *list, const struct list_node *node);
struct list_node *list_prepend(struct list *list, const void *key, const void *object);
struct list_node *list_append(struct list *list, const void *key, const void *object);
void list_destroy(struct list **list);
struct list *list_create();

#endif


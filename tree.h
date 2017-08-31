/*

tree structures and routines
Copyright (C) 2013-2017 Roman Fakhrazeyev <xinoir@theta.black>
This file is part of beta.

*/

#ifndef TREE_H
#define TREE_H

struct tree_node;
struct tree;

void tree_write(const struct tree *tree, int fd);
void *tree_object(const struct tree_node *node);
struct tree_node *tree_search(const struct tree *tree, const void *key);
struct tree_node *tree_add(struct tree *tree, const void *key, const void *object);
void tree_destroy(struct tree **tree);
struct tree *tree_create(int (* compare)(const void *, const void *), void (* write)(const void *object, int fd));

#endif


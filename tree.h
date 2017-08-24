/*

tree structures and routines
Copyright (C) 2013-2017 xinoir <xinoir@theta.black>
This file is part of beta.

*/

#ifndef TREE_H
#define TREE_H

#include <stdio.h>

struct tree_node;
struct tree;

void tree_print(FILE *stream, const struct tree *tree);
struct tree_node *tree_add(struct tree *tree, const void *object, size_t size);
void tree_free(struct tree **tree);
int tree_init(struct tree **tree, int (* compare)(const void *, const void *));

#endif


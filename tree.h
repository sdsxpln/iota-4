/*
 * a tree structures and routines
 * Copyright (C) 2013-2017 xinoir <xinoir@theta.black>
 * This file is part of beta.
 */

#ifndef TREE_H
#define TREE_H

#include <stdio.h>

struct tree_node {
    char *word;
    int count;
    struct tree_node *left;
    struct tree_node *right;
};

struct tree_node *tree_add_node(struct tree_node *tree_node, const char *word);
void tree_print(FILE *stream, struct tree_node *tree_node);

#endif


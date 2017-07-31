/*
 * a tree structures and routines
 * Copyright (C) 2013-2017 xinoir <xinoir@theta.black>
 * This file is part of beta.
 */

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include "beta/error.h"
#include "beta/tree.h"

static struct tree_node *tree_node_create(const char *word)
{
    struct tree_node *tree_node = NULL;

    tree_node = (struct tree_node *)malloc(sizeof(struct tree_node));
    if (!tree_node) {
        error("failed to allocate memory for a tree node");
        return NULL;
    }
    if (!(tree_node->word = (char *)malloc(strlen(word) + 1))) {
        error("failed to allocate memory for a tree node word");
        return NULL;
    }
    if (tree_node->word)
        strcpy(tree_node->word, word);
    tree_node->left = tree_node->right = NULL;
    tree_node->count = 1;

    return tree_node;
}

struct tree_node *tree_add_node(struct tree_node *tree_node, const char *word)
{
    int comparison;

    if (!tree_node)
        tree_node = tree_node_create(word);
    else if ((comparison = strcmp(word, tree_node->word)) == 0)
        tree_node->count++;
    else if (comparison < 0)
        tree_node->left = tree_add_node(tree_node->left, word);
    else
        tree_node->right = tree_add_node(tree_node->right, word);

    return tree_node;
}

void tree_print(FILE *stream, struct tree_node *tree_node)
{
    if (tree_node != NULL) {
        tree_print(stream, tree_node->left);
        fprintf(stream, "%4d %s\n", tree_node->count, tree_node->word);
        tree_print(stream, tree_node->right);
    }
}


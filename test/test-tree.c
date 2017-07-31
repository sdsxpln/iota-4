/*
 * tests for tree structures and routines
 * Copyright (C) 2013-2017 xinoir <xinoir@theta.black>
 * This file is part of beta.
 */

#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include "beta/tree.h"

static int test_tree_0002()
{
    struct tree_node *tree;

    tree = tree_add_node(NULL, "a");
    tree_add_node(tree, "b");
    tree_add_node(tree, "c");

    assert(tree->count == 1);
    assert(strcmp(tree->word, "a") == 0);
    assert(tree->right->count == 1);
    assert(strcmp(tree->right->word, "b") == 0);
    assert(tree->right->right->count == 1);
    assert(strcmp(tree->right->right->word, "c") == 0);

    return 0;
}


static int test_tree_0001()
{
    const char *tree_node_word = "word";
    struct tree_node *tree_node = NULL;
    

    tree_node = tree_add_node(NULL, tree_node_word);
    assert(tree_node);
    assert(strcmp(tree_node->word, tree_node_word) == 0);
    assert(tree_node->count == 1);
    assert(!tree_node->left);
    assert(!tree_node->right);

    return 0;
}

int main(void)
{
    if (test_tree_0001()) assert(0);
    if (test_tree_0002()) assert(0);

    exit(EXIT_SUCCESS);
}


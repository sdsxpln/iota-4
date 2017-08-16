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

struct tree_node {
    void *object;
    int count;
    struct tree_node *left;
    struct tree_node *right;
};

struct tree {
    int (* compare)(const void *, const void *);
    struct tree_node *root;
};

static void tree_print_node(FILE *stream, const struct tree_node *node)
{
    if (node) {
        tree_print_node(stream, node->left);
        fprintf(stream, "%4d %s\n", node->count, node->object);
        tree_print_node(stream, node->right);
    }
}

void tree_print(FILE *stream, const struct tree *tree)
{
    tree_print_node(stream, tree->root);
}

static struct tree_node *tree_create_node(const void *object, size_t size)
{
    struct tree_node *node = NULL;

    node = (struct tree_node *)malloc(sizeof(struct tree_node));
    if (!node) {
        error("failed to allocate memory for a tree node");
        return NULL;
    }
    if (!(node->object = malloc(size))) {
        error("failed to allocate memory for a tree node object");
        return NULL;
    }
    memcpy(node->object, object, size);
    node->left = node->right = NULL;
    node->count = 1;

    return node;
}

static struct tree_node *tree_add_node(struct tree *tree, struct tree_node *node,
        const void *object, size_t size)
{
    int cond;

    if (!node)
        node = tree_create_node(object, size);
    else if ((cond = tree->compare(object, node->object)) == 0)
        node->count++;
    else if (cond < 0)
        node->left = tree_add_node(tree, node->left, object, size);
    else
        node->right = tree_add_node(tree, node->right, object, size);

    return node;
}

struct tree_node *tree_add(struct tree *tree, const void *object, size_t size)
{
    struct tree_node *node = NULL;
    
    node = tree_add_node(tree, tree->root, object, size);
    if (tree->root == NULL)
        tree->root = node;

    return node;
}

static void tree_free_node(struct tree_node *node)
{
    if (node) {
        tree_free_node(node->left);
        free(node->object);
        tree_free_node(node->right);
        free(node);
        node = NULL;
    }
}

void tree_free(struct tree **tree)
{
    tree_free_node((*tree)->root);
    free(*tree);
    *tree = NULL;
}

int tree_init(struct tree **tree, int (* compare)(const void *, const void *))
{
    *tree = (struct tree *)malloc(sizeof(struct tree));
    (*tree)->compare = compare;
    (*tree)->root = NULL;

    return 0;
}


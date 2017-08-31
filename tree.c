/*

tree structures and routines
Copyright (C) 2013-2017 Roman Fakhrazeyev <xinoir@theta.black>
This file is part of beta.

*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "beta/error.h"
#include "beta/tree.h"

struct tree_node {
    const void *key;
    const void *object;
    struct tree_node *left;
    struct tree_node *right;
};

struct tree {
    int (* compare)(const void *, const void *);
    void (* write)(const void *, int fd);
    struct tree_node *root;
};

static void write_tree(const struct tree *tree, const struct tree_node *node, int fd)
{
    if (node) {
        write_tree(tree, node->left, fd);
        tree->write(node->object, fd);
        write_tree(tree, node->right, fd);
    }
}

static struct tree_node* create_node(const void *key, const void *object)
{
    struct tree_node *node;

    if (!(node = (struct tree_node *)malloc(sizeof(struct tree_node)))) {
        error("failed to allocate memory for a tree node");
        return NULL;
    }
    node->key = key;
    node->object = object;
    node->left = node->right = NULL;

    return node;
}

static struct tree_node *search_node(struct tree *tree, struct tree_node *node, const void *key)
{
    int cond;

    if (!node) {
        return NULL;
    } else if ((cond = tree->compare(node->key, key)) == 0) {
        return node;
    } else if (cond < 0) {
        return search_node(tree, node->left, key);
    } else {
        return search_node(tree, node->right, key);
    }

    return NULL;
}

static struct tree_node *add_node(struct tree *tree, struct tree_node *node, const void* key, const void *object)
{
    int cond;
    struct tree_node *postfix;

    if (!node) {
        if (!(node = create_node(key, object)))
            error("failed to create a tree node");
    } else if ((cond = tree->compare(node->key, key)) == 0) {
        assert(0);
    } else if (cond < 0) {
        postfix = add_node(tree, node->left, key, object);
        if (!node->left) {
            node->left = postfix;
            node = postfix;
        }
    } else {
        postfix = add_node(tree, node->right, key, object);
        if (!node->right) {
            node->right = postfix;
            node = postfix;
        }
    }

    return node;
}

static void destroy_tree(struct tree_node *node)
{
    if (node) {
        destroy_tree(node->left);
        destroy_tree(node->right);
        free(node);
    }
}

void tree_write(const struct tree *tree, int fd)
{
    write_tree(tree, tree->root, fd);
}

void *tree_object(const struct tree_node *node)
{
    return node->object;
}

struct tree_node *tree_search(const struct tree *tree, const void *key)
{
    return search_node(tree, tree->root, key);
}

struct tree_node *tree_add(struct tree *tree, const void *key, const void *object)
{
    struct tree_node *node;

    if (!(node = add_node(tree, tree->root, key, object))) {
        error("failed to add a tree node");
        return NULL;
    }
    if (tree->root == NULL)
        tree->root = node;

    return node;
}

void tree_destroy(struct tree **tree)
{
    destroy_tree((*tree)->root);
    free(*tree);
    *tree = NULL;
}

struct tree *tree_create(int (* compare)(const void *, const void *), void (* write)(const void *object, int fd))
{
    struct tree *tree;

    if (!(tree = (struct tree *)malloc(sizeof(struct tree)))) {
        error("failed to allocate memory for a tree");
        return NULL;
    }
    tree->compare = compare;
    tree->write = write;
    tree->root = NULL;

    return tree;
}


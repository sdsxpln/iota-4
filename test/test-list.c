/*
 * tests for list tructures and routines
 * Copyright (C) 2013-2017 xinoir <xinoir@theta.black>
 * This file is part of beta.
 */

#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include "beta/list.h"

const char *module;

static int test_list_0001()
{
    int status = 0;
    struct list *list = NULL;
    struct list_node *node = NULL;
    void *node_object = NULL;
    size_t node_object_size = 0;
    const char object[] = "list node data";

    status = list_create(&list);
    assert(!status);
    assert(list);

    status = list_insert_node(list, (void *)object, (size_t)(strlen(object) + 1), &node);
    assert(!status);
    assert(node);
    assert(list_length(list) == 1);
    assert(!list_next_node(node));

    node_object = list_node_data(node, &node_object_size);
    assert(node_object);
    assert(strcmp(node_object, object) == 0);
    assert(node_object_size == strlen(object) + 1);

    list_delete_node(list, node);
    assert(list_length(list) == 0);

    list_destroy_node(&node);
    assert(!node);

    list_destroy(&list);
    assert(!list);

    return 0;
}

int main(int argc, const char **argv)
{
    assert(argc != 0);
    module = argv[0];

    log_init(module);

    if (test_list_0001()) assert(0);

    log_deinit(module);

    exit(EXIT_SUCCESS);
}


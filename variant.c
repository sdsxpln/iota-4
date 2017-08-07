/*
 * a variant structures and routines
 * Copyright (C) 2013-2017 xinoir <xinoir@theta.black>
 * This file is part of beta.
 */

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "beta/error.h"
#include "beta/variant.h"

struct variant {
    void *object;
    size_t size;
};

size_t variant_size(const struct variant *variant)
{
    return variant->size;
}

void *variant_object(const struct variant *variant)
{
    return variant->object;
}

void variant_destroy(struct variant **variant)
{
    free((*variant)->object);
    free(*variant);
    *variant = NULL;
}

int variant_create(const void *object, size_t size, struct variant **variant)
{
    *variant = (struct variant *)malloc(sizeof(struct variant));
    if (!*variant)
        return error("failed to allocate memory for a variant");

    (*variant)->object= malloc(size);
    if (!(*variant)->object)
        return error("failed to allocate memory for a variant object");

    memmove((*variant)->object, object, size);
    (*variant)->size = size;

    return 0;
}


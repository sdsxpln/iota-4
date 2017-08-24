/*

variant structures and routines
Copyright (C) 2013-2017 xinoir <xinoir@theta.black>
This file is part of beta.

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

int variant_init(const void *object, size_t size, struct variant *variant)
{
    variant->object = object;
    variant->size = size;

    return 0;
}


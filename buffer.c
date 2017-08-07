/*
 * a buffer structures and routines
 * Copyright (C) 2013-2017 xinoir <xinoir@theta.black>
 * This file is part of beta.
 */

#include <stdlib.h>
#include <string.h>
#include "beta/error.h"
#include "beta/buffer.h"

struct buffer {
    void *base;
    size_t size;
    int length;
};

size_t buffer_size(const struct buffer *buffer)
{
    return buffer->size;
}

int buffer_length(const struct buffer *buffer)
{
    return buffer->length;
}

int buffer_append(struct buffer *buffer, void *object, size_t size)
{
    memmove(buffer->base + buffer->length, object, size);
    buffer->length += size;

    return 0;
}

void buffer_destroy(struct buffer **buffer)
{
    free(*buffer);
    *buffer = NULL;
}

static void buffer_init(struct buffer *buffer, size_t size)
{
    buffer->size = size;
    buffer->length = 0;
}

static int buffer_alloc(struct buffer **buffer, size_t size)
{
    if (!(*buffer = malloc(sizeof(struct buffer))))
        return error("failed to allocate memory for a buffer");
    if (!((*buffer)->base = malloc(sizeof(char) * size)))
        return error("failed to allocate memory for a buffer");

    return 0;
}

int buffer_create(struct buffer **buffer, size_t size)
{
    int status;

    if ((status = buffer_alloc(buffer, size)))
        return status;
    buffer_init(*buffer, size);

    return 0;
}


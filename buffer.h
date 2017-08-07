/*
 * a buffer structures and routines
 * Copyright (C) 2013-2017 xinoir <xinoir@theta.black> 
 * This file is part of beta.
 */

#ifndef BUFFER_H
#define BUFFER_H

struct buffer;

size_t buffer_size(const struct buffer *buffer);
int buffer_length(const struct buffer *buffer);
int buffer_append(struct buffer *buffer, void *object, size_t size);
void buffer_destroy(struct buffer **buffer);
int buffer_create(struct buffer **buffer, size_t size);

#endif


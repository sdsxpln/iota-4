/*
 * a variant structures and routines
 * Copyright (C) 2013-2017 xinoir <xinoir@theta.black> 
 * This file is part of beta.
 */

#ifndef VARIANT_H
#define VARIANT_H

struct variant;

size_t variant_size(const struct variant *variant);
void *variant_object(const struct variant *variant);
void variant_destroy(struct variant **variant);
int variant_create(const void *object, size_t size, struct variant **variant);

#endif


/*

map routines
Copyright (C) 2013-2017 Roman Fakhrazeyev <xinoir@theta.black> 
This file is part of beta.

*/

#ifndef MAP_H
#define MAP_H

struct map;
typedef unsigned (* hash)(const void *, int);
typedef int (* compare)(const void *, const void *);

void **map_to_array(const struct map *, size_t *length);
int map_contains(const struct map *, const void *key);
void *map_lookup(const struct map *, const void *key);
int map_install(struct map *, const void *key, const void *object);
void map_destroy(struct map **);
struct map *map_create(int size, hash, compare);

#endif


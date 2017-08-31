/*

map routines
Copyright (C) 2013-2017 Roman Fakhrazeyev <xinoir@theta.black> 
This file is part of beta.

*/

#ifndef MAP_H
#define MAP_H

struct map;

void *map_lookup(const struct map *, const void *key);
int map_install(struct map *, const void *key, const void *object);
struct map *map_create(int size, unsigned (* hash)(const void *, int),
        int (* compare)(const void *, const void *));

#endif


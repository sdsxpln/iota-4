/*

map routines
Copyright (C) 2013-2017 Roman Fakhrazeyev <xinoir@theta.black> 
This file is part of beta.

*/

#include <stdlib.h>
#include <string.h>
#include "beta/error.h"
#include "beta/list.h"
#include "beta/map.h"

struct map {
    int size;
    unsigned (* hash)(const void *, int);
    int (* compare)(const void*, const void *);
    struct list **buckets;
};

void *map_lookup(const struct map *map, const void *key)
{
    struct list *list;
    struct list_node *node;

    if ((list = *(map->buckets + map->hash(key, map->size))))
        for (node = list_head(list); node; node = list_next(node))
            if (map->compare(key, list_node_key(node)) == 0)
                return list_node_object(node);

    return NULL;
}

int map_install(struct map *map, const void *key, const void *object)
{
    struct list **slot;

    slot = map->buckets + map->hash(key, map->size);
    if (!*slot)
        *slot = list_create();
    list_prepend(*slot, key, object);

    return 0;
}

struct map *map_create(int size, unsigned (* hash)(const void *, int),
        int (* compare)(const void *, const void *))
{
    struct map *map;

    if (!(map = (struct map *)malloc(sizeof(struct map)))) {
        error("failed to allocate memory for a map");
        return NULL;
    }
    map->size = size;
    map->hash = hash;
    map->compare = compare;
    map->buckets = (struct list **)malloc(sizeof(struct list *) * map->size);
    memset(map->buckets, 0, sizeof(struct list *) * map->size);

    return map;
}


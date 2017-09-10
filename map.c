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
    struct list **buckets;
    unsigned (* hash)(const void *, int);
    int (* compare)(const void*, const void *);
};

static int map_num_of_objects(const struct map *map)
{
    int n = 0;
    struct list **bucket = map->buckets;

    for (int i = 0; i < map->size; i++, bucket++)
        if (*bucket)
            n += list_length(*bucket);

    return n;
}

void **map_to_array(const struct map *map, size_t *length)
{
    struct list **bucket = map->buckets;
    void *object;
    void **objects;
    struct list_node *node;

    *length = map_num_of_objects(map);
    if (!(objects = malloc(sizeof(void *) * *length))) return NULL;
    for (int i = 0; i < map->size; i++, bucket++)
        if (*bucket)
            for (node = list_head(*bucket); node; node = list_next(node))
                *objects++ = list_node_object(node);

    return objects - *length;
}

int map_contains(const struct map *map, const void *key)
{
    struct list *list = NULL;
    struct list_node *node = NULL;

    if ((list = *(map->buckets + map->hash(key, map->size))))
        for (node = list_head(list); node; node = list_next(node))
            if (map->compare(key, list_node_key(node)) == 0)
                return 1;

    return 0;
}

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
    struct list *bucket = NULL;

    if (!(bucket = *(map->buckets + map->hash(key, map->size))))
        bucket = list_create();
    list_prepend(bucket, key, object);

    return 0;
}

void map_destroy(struct map **map)
{
    struct list *bucket;

    for (int i = 0; i < (*map)->size; i++)
        if ((bucket = *((*map)->buckets + i)))
            list_destroy(&bucket);
    free(*map); 
    *map = NULL;
}

struct map *map_create(int size, hash hash, compare compare)
{
    struct map *map;

    if (!(map = (struct map *)malloc(sizeof(struct map))))
        return NULL;
    map->size = size;
    map->hash = hash;
    map->compare = compare;
    map->buckets = (struct list **)malloc(sizeof(struct list *) * map->size);
    memset(map->buckets, 0, sizeof(struct list *) * map->size);

    return map;
}


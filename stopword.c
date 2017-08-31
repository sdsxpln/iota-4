/*

stopword routines
Copyright (C) 2013-2017 Roman Fakhrazeyev <xinoir@theta.black> 
This file is part of beta.

*/

#include <stdlib.h>
#include <string.h>
#include "beta/error.h"
#include "beta/stream.h"
#include "beta/string.h"
#include "beta/hash.h"
#include "beta/map.h"

#define WORD_SIZE 128
#define STOPWORD_SIZE 128

struct stopword {
    struct map *words;
};

static void destroy_stopword(struct stopword *sw)
{
    if (!sw)
        return;
    free(sw->words);
    free(sw);
}

static int init_stopword(struct stopword *sw, int fd)
{
    char w[WORD_SIZE] = { 0 };
    char *word;
    int iseof;

    if (!(sw->words = map_create(STOPWORD_SIZE, &hash_string, &strcmp)))
        return error("failed to allocate memory for a stopword internal objects");
    for (; !stream_read_word(fd, w, WORD_SIZE, &iseof) && !iseof; ) {
        if (!(word = string_copy(w, WORD_SIZE)))
            return error("failed to copy a word");
        if (map_install(sw->words, word, word))
            return error("failed to initialize a stopword");
    }

    return 0;
}

int stopword_contains(const struct stopword *stopword, const char *word)
{
    return map_lookup(stopword->words, word) ? 1 : 0;
}

struct stopword *stopword_create(int fd)
{
    struct stopword *sw;

    if (!(sw = (struct stopword *)malloc(sizeof(struct stopword)))) {
        error("failed to allocate memory for a stopword");
        return NULL;
    }
    if (init_stopword(sw, fd))
        destroy_stopword(sw);

    return sw;
}


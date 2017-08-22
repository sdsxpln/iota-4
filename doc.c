/*

doc structures and routines
Copyright (C) 2013-2017 xinoir <xinoir@theta.black>
This file is part of Beta.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <unistd.h>
#include <regex.h>
#include "beta/error.h"
#include "beta/stream.h"
#include "beta/list.h"
#include "beta/doc.h"

#define DOC_LINE_MAX 4096

enum doc_term_type {
    UNKNOWN,
    EXTERNAL_SCENE,
    INTERNAL_SCENE
};

struct doc_line;

struct doc_term {
    enum doc_term_type type;
    struct doc_line *line;
};

static struct doc_term_def {
    enum doc_term_type type;
    const char *pattern;
    regex_t regex;
} doc_term_defs[] = {
    { EXTERNAL_SCENE, "^[[:blank:]]{2,}EXT.[[:blank:]]{1,}" },
    { INTERNAL_SCENE, "^[[:blank:]]{2,}INT.[[:blank:]]{1,}" }
};

struct doc_line {
    char *text;
    int size;
};

struct doc {
    struct list **terms;
    struct doc_line **lines;
    int length;
};

static int regex_error(int error, const regex_t *regex)
{
    size_t ree = 0;
    char reeb[256];

    ree = regerror(e, re, reeb, 256);
    return error("error: [%d], message: [%s]\n", ree, reeb);
}

static int alloc_line(struct doc_line **l, size_t nb)
{
    if (!(*l = malloc(sizeof(struct doc_line))))
        return error("failed to allocate memory for a doc line");

    if (nb > 1)
        if (!((*l)->text = malloc(nb)))
            return error("failed to allocate memory for a doc line text");

    return 0;
}

static int create_line(struct doc_line **l, size_t nb, const char *b)
{
    int s;

    if ((s = alloc_line(l, nb)))
        return s;

    if (nb > 1) {
        memcpy((*l)->text, b, nb);
        (*l)->size = nb;
    }
    else {
        (*l)->text = NULL;
        (*l)->size = 0;
    }

    return 0;
}

static int alloc_doc(struct doc **doc)
{
    if (!(*doc = malloc(sizeof(struct doc))))
        return error("failed to allocate memory for a doc");

    return 0;
}

int doc_length(const struct doc *doc)
{
    return doc->length;
}

int doc_parse(struct doc *doc)
{
    int s = 0;
    struct doc_line **line;
    regex_t re;

    if (regcomp(&re, "^[[:blank:]]{2,}EXT.[[:blank:]]{1,}", REG_EXTENDED | REG_NOSUB))
        return error("failed to compile a regex");

    for (line = doc->lines + 1; *line; line++) {
        if ((*line)->text && (*line)->size > 1) {
            if ((s = regexec(&re, (*line)->text, 0, NULL, 0)) == 0) {
                if ((*(line-1))->size == 0 && (*(line+1))->size == 0)
                    ;
            }
        }
    }

    regfree(&re);

    return 0;
}

int doc_read(int fd, struct doc *doc)
{
    int s = 0;
    int i = 0;
    int iseof = 0;
    char b[DOC_LINE_MAX] = {0};
    int bs = 0;
    struct list *list = NULL;
    struct list_node *node = NULL;
    struct doc_line *line = NULL;
    
    if ((s = list_create(&list)))
        return s;

    while (!stream_read_line(fd, b, DOC_LINE_MAX, &iseof) && !iseof) {
        bs = strlen(b) + 1;
        if ((s = create_line(&line, bs, b)))
            return s;
        if ((s = list_append(list, line, bs, NULL)))
            return s;
    }

    doc->length = list_length(list);
    doc->lines = (struct doc_line **)malloc((doc->length + 2) * sizeof(struct doc_line *));
    doc->lines[0] = NULL;
    doc->lines[doc->length + 1] = NULL;
    for (i = 1, node = list_head(list); node; node = list_next(node), i++)
        doc->lines[i] = (struct doc_line *)list_data(node, NULL);
    list_destroy(&list);

    return 0;
}

void doc_destroy(struct doc **doc)
{
    struct doc_line *line = NULL;

    for (line = *(*doc)->lines; line; line++)
        free(line->text);
    free(*doc);
    *doc = NULL;
}

int doc_create(struct doc **doc)
{
    int s;

    if ((s = alloc_doc(doc)))
        return s;

    entity[0].comp_regex = (regex_t *)malloc(sizeof(regex_t));
    if ((s = regcomp(entity[0].comp_regex, entity[0].regex_str, REG_EXTENDED)))
        return regex_error(s, entity[0].comp_regex);

    return 0;
}


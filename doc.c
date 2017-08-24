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
#define ERROR_MESSAGE_MAX 256

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

struct doc_line {
    struct list *terms;
    char *text;
    int size;
    int index;
};

struct doc {
    struct list *terms;
    struct doc_line **lines;
    int length;
};

typedef int (* match_term_t)(struct doc_line **, struct doc_term **);
static int regex_error(int en, const regex_t *regex);

static int match_internal_scene(struct doc_line **l, struct doc_term **t)
{
    int s = 0;
    static const enum doc_term_type type = INTERNAL_SCENE;
    static const char pattern[] = "^[[:blank:]]{2,}INT.[[:blank:]]{1,}";
    static int regex_flag = 0;
    static regex_t regex;
    *t = NULL;

    if (!regex_flag) {
        if ((s = regcomp(&regex, pattern, REG_EXTENDED)) != 0)
            return regex_error(s, &regex);
        regex_flag = 1;
    }

    if (!regexec(&regex, (*l)->text, 0, NULL, 0)) {
        if ((*(l-1))->size == 0 && (*(l+1))->size == 0) {
            *t = (struct doc_term *)malloc(sizeof(struct doc_term));
            (*t)->type = type;
            (*t)->line = *l;
        }
    }

    return s;
}

static int match_external_scene(struct doc_line **line, struct doc_term **term)
{
    int s = 0;
    static const enum doc_term_type term_type = EXTERNAL_SCENE;
    static const char pattern[] = "^[[:blank:]]{2,}EXT.[[:blank:]]{1,}";
    static int regex_flag = 0;
    static regex_t regex;
    *term = NULL;

    if (!regex_flag) {
        if ((s = regcomp(&regex, pattern, REG_EXTENDED)) != 0)
            return regex_error(s, &regex);
        regex_flag = 1;
    }

    if (!regexec(&regex, (*line)->text, 0, NULL, 0)) {
        if ((*(line-1))->size == 0 && (*(line+1))->size == 0) {
            *term = (struct doc_term *)malloc(sizeof(struct doc_term));
            (*term)->type = term_type;
            (*term)->line = *line;
        }
    }

    return s;
}

static int regex_error(int en, const regex_t *regex)
{
    size_t regex_en = 0;
    char regex_em[ERROR_MESSAGE_MAX] = {0};

    regex_en = regerror(en, regex, regex_em, sizeof regex_em);
    return error("ERROR: number: [%d], message: [%s]\n", regex_en, regex_em);
}

static int doc_line_create(struct doc_line **line, size_t nb, const char *b, int index)
{
    if (!(*line = malloc(sizeof(struct doc_line))))
        return error("failed to allocate memory for a doc line");
    if (nb > 1) {
        if (!((*line)->text = malloc(nb)))
            return error("failed to allocate memory for a doc line text");
        memcpy((*line)->text, b, nb);
        (*line)->size = nb;
    }
    else {
        (*line)->text = NULL;
        (*line)->size = 0;
    }
    (*line)->terms = NULL;
    (*line)->index = index;

    return 0;
}

static int doc_line_append_term(struct doc_line *line, struct doc_term *term)
{
    int s = 0;

    if (!line->terms)
        if ((s = list_create(&line->terms)))
            return s;
    if ((s = list_append(line->terms, term, NULL)))
        return s;

    return 0;
}

int doc_length(const struct doc *doc)
{
    return doc->length;
}

int doc_terms(const struct doc *doc)
{
    int c = 0;
    struct list_node *n = NULL;
    struct doc_term *t = NULL;

    for (n = list_head(doc->terms); n; n = list_next(n)) {
        t = (struct doc_term *)list_data(n);
        if (t->type == EXTERNAL_SCENE)
            ++c;
    }

    return c;
}

int doc_parse(struct doc *doc)
{
    int s = 0;
    struct doc_line **line = NULL;
    struct doc_term *term = NULL;

    if (!doc->terms)
        list_create(&doc->terms);

    match_term_t match_terms[] = {
        &match_external_scene,
        &match_internal_scene,
        NULL
    };

    match_term_t *match_term;

    for (line = doc->lines + 1; *line; line++)
        if ((*line)->size > 1)
            for (match_term = match_terms; *match_term; match_term++)
                if (!(*match_term)(line, &term) && term) {
                    list_append(doc->terms, term, NULL); 
                    if ((s = doc_line_append_term(*line, term)))
                        return s;
                    break;
                }

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
    for (i = 1; !stream_read_line(fd, b, DOC_LINE_MAX, &iseof) && !iseof; i++) {
        bs = strlen(b) + 1;
        if ((s = doc_line_create(&line, bs, b, i)))
            return s;
        if ((s = list_append(list, line, NULL)))
            return s;
    }

    doc->length = list_length(list);
    doc->lines = (struct doc_line **)malloc((doc->length + 2) * sizeof(struct doc_line *));
    doc->lines[0] = NULL;
    doc->lines[doc->length + 1] = NULL;
    for (i = 1, node = list_head(list); node; node = list_next(node), i++)
        doc->lines[i] = (struct doc_line *)list_data(node);
    list_destroy(&list);

    return 0;
}

void doc_destroy(struct doc **doc)
{
    struct doc_line *line = NULL;

    for (line = *(*doc)->lines; line; line++) {
        free(line->text);
        free(line);
    }
    free(*doc);
    *doc = NULL;
}

int doc_create(struct doc **doc)
{
    if (!(*doc = malloc(sizeof(struct doc))))
        return error("failed to allocate memory for a doc");
    (*doc)->terms = NULL;

    return 0;
}


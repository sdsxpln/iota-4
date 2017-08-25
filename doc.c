/*

doc structures and routines
Copyright (C) 2013-2017 xinoir <xinoir@theta.black>
This file is part of Beta.

*/

#include <stdlib.h>
#include "beta/error.h"
#include "beta/stream.h"
#include "beta/list.h"
#include "beta/regex.h"
#include "beta/doc.h"

#define DOC_LINE_MAX 4096

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

int doc_analyze(int fdo, const struct doc *doc)
{
    return 0;
}

int doc_parse(struct doc *doc, match_term_t **match_terms)
{
    int s = 0;
    struct doc_line **line = NULL;
    struct doc_term *term = NULL;
    match_term_t *match_term = NULL;

    if (!doc->terms)
        list_create(&doc->terms);

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

int doc_read(int fdi, struct doc *doc)
{
    int s, i, iseof;
    char b[DOC_LINE_MAX] = {0};
    struct list *list = NULL;
    struct list_node *node = NULL;
    struct doc_line *line = NULL;
    
    if ((s = list_create(&list)))
        return s;
    for (i = 1; !stream_read_line(fdi, b, DOC_LINE_MAX, &iseof) && !iseof; i++) {
        if ((s = doc_line_create(&line, strlen(b) + 1, b, i)))
            return s;
        if ((s = list_append(list, line, NULL)))
            return s;
    }

    doc->length = list_length(list);
    if (!(doc->lines = (struct doc_line **)malloc((doc->length + 2) * sizeof(struct doc_line *))))
        return error("failed to allocate memory for a doc lines");
    doc->lines[0] = doc->lines[doc->length + 1] = NULL;
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


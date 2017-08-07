/*
 * a doc structures and routines
 * Copyright (C) 2013-2017 xinoir <xinoir@theta.black>
 * This file is part of Beta.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <unistd.h>
#include "beta/error.h"
#include "beta/buffer.h"
#include "beta/doc.h"

#define DOC_LINE_MAX 4096
#define DOC_TEXT_MAX_SIZE 4096
#define DOC_TEXT_INITIAL_SIZE 4096

struct doc {
    struct buffer *text;
};

static int doc_line_max()
{
    int line_max;

    if (LINE_MAX >= DOC_LINE_MAX) {
        line_max = DOC_LINE_MAX;
    } else {
        long sc_line_max = sysconf(_SC_LINE_MAX);
        line_max = (sc_line_max < 0 || sc_line_max > DOC_LINE_MAX) ? DOC_LINE_MAX : (int)sc_line_max;
    }

    return line_max;
}

static int doc_alloc(struct doc **doc, size_t size)
{
    int status;

    if (!(*doc = malloc(sizeof(struct doc))))
        return error("failed to allocate memory for a doc");
    if ((status = buffer_create(&(*doc)->text, size)))
        return status;

    return 0;
}

int doc_create(FILE *stream, struct doc **doc)
{
    int status;
    int line_max;
    char *line;

    if ((status = doc_alloc(doc, DOC_TEXT_INITIAL_SIZE)))
        return status;

    line_max = doc_line_max();
    if (!(line = malloc(line_max + 1)))
        return error("failed to allocate memory for a doc line");

    while (!fgets(line, line_max + 1, stream))
        buffer_append((*doc)->text, line, strlen(line) + 1);

    if (errno != 0)
        return error("unknown error [%d] [%s]", errno, strerror(errno));


    return 0;
}


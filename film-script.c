/*

film script doc structures and routines
Copyright (C) 2013-2017 xinoir <xinoir@theta.black>
This file is part of Beta.

*/

#include <stdlib.h>
#include <regex.h>
#include "beta/error.h"
#include "beta/regex.h"
#include "beta/doc.h"
#include "beta/film-script.h"

enum term_type {
    UNKNOWN,
    EXTERNAL_SCENE,
    INTERNAL_SCENE
};

struct term_attr {
    char *pattern;
    regex_t regex;
    match_term matcher;
};

static int match_internal_scene(struct doc_line **, struct doc_term **);
static int match_external_scene(struct doc_line **, struct doc_term **);

struct term_attr *term_attrs[] = {
    [UNKNOWN] = NULL,
    [EXTERNAL_SCENE] = &(struct term_attr){
        .pattern = "^[[:blank:]]{2,}EXT.[[:blank:]]{1,}",
        .matcher = &match_external_scene
    },
    [INTERNAL_SCENE] = &(struct term_attr){
        .pattern = "^[[:blank:]]{2,}INT.[[:blank:]]{1,}",
        .matcher = &match_internal_scene
    },
    NULL
};

static int match_internal_scene(struct doc_line **line, struct doc_term **term)
{
    static enum term_type type = INTERNAL_SCENE;
    struct term_attr *attr = term_attrs[type];

    if (!regexec(&attr->regex, (*line)->text, 0, NULL, 0)) {
        if ((*(line-1))->size == 0 && (*(line+1))->size == 0) {
            *term = (struct doc_term *)malloc(sizeof(struct doc_term));
            (*term)->type = type;
            (*term)->line = *line;
        }
    }

    return 0;
}

static int match_external_scene(struct doc_line **line, struct doc_term **term)
{
    static enum term_type type = EXTERNAL_SCENE;
    struct term_attr *attr = term_attrs[type];

    if (!regexec(&attr->regex, (*line)->text, 0, NULL, 0)) {
        if ((*(line-1))->size == 0 && (*(line+1))->size == 0) {
            *term = (struct doc_term *)malloc(sizeof(struct doc_term));
            (*term)->type = type;
            (*term)->line = *line;
        }
    }

    return 0;
}

int film_script_init_term_matchers(match_term **term_matchers)
{
    int s;
    struct term_attr *attr;
    static match_term matchers[] = {
        &match_external_scene,
        &match_internal_scene,
        NULL
    };

    attr = term_attrs[EXTERNAL_SCENE];
    if ((s = regcomp(&attr->regex, attr->pattern, REG_EXTENDED)))
        return regex_error(s, &attr->regex);
    *term_matchers = matchers;

    return 0;
}


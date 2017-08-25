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

enum film_script_term_type {
    UNKNOWN,
    EXTERNAL_SCENE,
    INTERNAL_SCENE
};

match_term_t match_terms[] = {
    &match_external_scene,
    &match_internal_scene,
    NULL
};

match_term_t *match_term;

static int match_internal_scene(struct doc_line **line, struct doc_term **term)
{
    int s = 0;
    static const enum film_script_term_type type = INTERNAL_SCENE;
    static const char pattern[] = "^[[:blank:]]{2,}INT.[[:blank:]]{1,}";
    static int regex_flag = 0;
    static regex_t regex;
    *term = NULL;

    if (!regex_flag) {
        if ((s = regcomp(&regex, pattern, REG_EXTENDED)))
            return regex_error(s, &regex);
        regex_flag = 1;
    }

    if (!regexec(&regex, (*line)->text, 0, NULL, 0)) {
        if ((*(line-1))->size == 0 && (*(line+1))->size == 0) {
            *term = (struct doc_term *)malloc(sizeof(struct doc_term));
            (*term)->type = type;
            (*term)->line = *line;
        }
    }

    return s;
}

static int match_external_scene(struct doc_line **line, struct doc_term **term)
{
    int s = 0;
    static const enum film_script_term_type type = EXTERNAL_SCENE;
    static const char pattern[] = "^[[:blank:]]{2,}EXT.[[:blank:]]{1,}";
    static int regex_flag = 0;
    static regex_t regex;
    *term = NULL;

    if (!regex_flag) {
        if ((s = regcomp(&regex, pattern, REG_EXTENDED)))
            return regex_error(s, &regex);
        regex_flag = 1;
    }

    if (!regexec(&regex, (*line)->text, 0, NULL, 0)) {
        if ((*(line-1))->size == 0 && (*(line+1))->size == 0) {
            *term = (struct doc_term *)malloc(sizeof(struct doc_term));
            (*term)->type = type;
            (*term)->line = *line;
        }
    }

    return s;
}


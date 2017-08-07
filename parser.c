/*
 * script parser routines
 * Copyright (C) 2013-2017 xinoir <xinoir@theta.black>
 * This file is part of Beta.
 */

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <regex.h>
#include "beta/notused.h"
#include "beta/error.h"
#include "beta/script-parser.h"

#define REGEX_ERR_BUF_SZ 100
#define LINE_BUF_SZ 200

static struct entity {
    int type_id;
    char *type_name;
    char *regex_str;
    regex_t *comp_regex;
} entity[] = {
    { 1, "EXTERIOR-SCENE", "[[:digit:]]*[[:blank:]]+EXT[.]", NULL }
};

static int
script_parser_regex_err(int err, const char *func, int line, regex_t *regex)
{
    size_t regex_err = 0;
    char regex_errbuf[REGEX_ERR_BUF_SZ];

    regex_err = regerror(err, regex, regex_errbuf, REGEX_ERR_BUF_SZ);
    return error(
        "ERROR [%lu] [%s] [%d] [%s]:[%s]:[%d]\n",
        regex_err, regex_errbuf, err, __FILE__, func, line
    );
}

int
script_parser_parse(FILE *stream, struct script *script)
{
    int status = 0;
    char linebuf[LINE_BUF_SZ] = { 0 };
    int i;

    while (fgets(linebuf, sizeof linebuf + 1, stream) != NULL) {
        for (i = 0; i < (int)(sizeof(entity)/sizeof(struct entity)); i++) { 
            if (
                (status = regexec(entity[i].comp_regex, linebuf, 0, NULL,
                    REG_NOTBOL | REG_NOTEOL)) == 0
            ) {
                script->lines[0].type_id = entity[i].type_id;
                if (memmove(script->lines[0].text, linebuf, SCRIPT_LINE_MAX) == NULL)
                    return error("memmove");
                script->line_count++;
                continue;
            } else if (status != REG_NOMATCH)
                return script_parser_regex_err(status, __func__, __LINE__,
                    entity[i].comp_regex);
        }
    }

    if (errno != 0)
        return error("unknown error [%d] [%s]", errno, strerror(errno));

    return 0;
}

int
script_parser_init(void)
{
    int status = 0;

    entity[0].comp_regex = (regex_t *)malloc(sizeof(regex_t));

    if ((status = regcomp(
            entity[0].comp_regex, entity[0].regex_str, REG_EXTENDED)) != 0) {

        return script_parser_regex_err(
            status, __func__, __LINE__, entity[0].comp_regex);
    }

    return 0;
}


/*
 * tests for script parser routines
 * Copyright (C) 2013-2017 xinoir <xinoir@theta.black>
 * This file is part of Beta.
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include <sys/types.h>
#include <unistd.h>
#include "beta/log.h"
#include "beta/error.h"
#include "beta/trace.h"
#include "beta/script.h"
#include "beta/script-parser.h"

static const char *mod;

static void handle_exit(void)
{
    log_deinit(mod);
    trace("exited [%s]", mod);
}

static int test_script_parser_parse(void)
{
    size_t status;
    int pipefd[2];
    FILE *istream, *ostream;
    struct script *script = NULL;

    if (pipe(pipefd) == -1)
        return error("pipe");

    istream = ostream = NULL;

    if (!(istream = fdopen(pipefd[0], "r")))
        return error("fdopen");

    if (!(ostream = fdopen(pipefd[1], "w")))
        return error("fdopen");

    char text[] = " EXT. ";
    status = fwrite(text, sizeof(char), strlen(text), ostream);
    if (status == 0 || errno != 0)
        return error(
            "writing to [%d] failed with status [%lu] [%d] [%s] [%lu]",
            ostream, status, errno, strerror(errno));

    if (fclose(ostream))
        return error("fflush");

    script = (struct script *)malloc(sizeof(struct script));
    script->lines = (struct script_line *)malloc(sizeof(struct script_line) * 40000);
    script->lines[0].text = malloc(sizeof(char)*SCRIPT_LINE_MAX);
    script_parser_init();
    script_parser_parse(istream, script);
    assert(script->line_count == 1);
    assert(script->lines[0].type_id == 1);

    return 0;
}

static int test_script_parser_init(void)
{
    assert(script_parser_init() == 0);

    return 0;
}

int main(
    int argc,
    const char **argv
)
{

    assert(argc != 0);
    mod = argv[0];

    if (atexit(handle_exit))
        error("an exit handler could not be registered for [%s]", mod);

    log_init(mod);

    if (test_script_parser_init() != 0) assert(0);
    if (test_script_parser_parse() != 0) assert(0);

    exit(EXIT_SUCCESS);
}


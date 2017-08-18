/*
 * a token structures and routines
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
#include "beta/token.h"

#define TOKEN_MAX 128

struct token {
    char *string;
};

static int get_token_string(FILE *stream, char *string)
{
    char c;

    while ((c = fgetc(input)) != EOF && !isalpha(c))
        ;
    if (c == EOF)
        return EOF;
    for (*word = c; (c = fgetc(input)) && c != EOF; ) {
        if (!isalpha(c)) {
            ungetc(c, input);
            break;
        }
        *++word= c;
    }
    *++word = 0;

    return 0;
}


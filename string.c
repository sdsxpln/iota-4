/*
 * string library of routines
 * Copyright (C) 2013-2017 xinoir <xinoir@theta.black>
 * This file is part of beta.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "beta/error.h"
#include "beta/string.h"

int string_word(FILE *f, char *w, size_t nc)
{
    char c;

    while ((c = fgetc(f)) != EOF && !isalpha(c))
        ;
    if (c == EOF)
        return EOF;
    for (*w = c, --nc; nc > 0 && (c = fgetc(f)) && c != EOF; *++w = c, nc--) {
        if (!isalpha(c)) {
            ungetc(c, f);
            break;
        }
    }
    *++w = 0;

    return 0;
}

void string_reverse(char *string)
{
    int c, i, j;

    for (i = 0, j = strlen(string)-1; i < j; i++, j--)
        c = string[i], string[i] = string[j], string[j] = c;
}

void string_from_int(char *string, int integer)
{
    /* casting to an unsigned to handle a case for INT_MIN */
    unsigned u;
    int i, is_negative;

    u = (is_negative = integer < 0) ? -integer : integer;
    
    for (i = 0; (!i && !u) || (u); i++, u /= 10)
        string[i] = (u % 10) + '0';

    if (is_negative)
        string[i] = '-';

    string[++i] = '\0';
    string_reverse(string);
}

char *string_copy(const char *string, size_t size)
{
    char *b;
    int bs;
    int string_length;
    
    string_length = strlen(string);
    bs = string_length < size ? string_length + 1 : size - 1;
    if (!(b = (char *)malloc(sizeof(char) * bs))) {
        error("failed to allocate memory for a string copy");
        return NULL;
    }

    strncpy(b, string, bs);
    *(b + bs) = '\0';

    return b;
}

int string_append(char *buffer, size_t size, const char *string, ...)
{
    int status;
    va_list va;
    int length;

    length = strlen(buffer);
    va_start(va, string);

    if ((status = vsnprintf(buffer + length, size - length, string, va)) < 0)
        return error("formatting and copying a string failed");
    else if ((unsigned)status >= size - length)
        return error("truncated as the buffer was too small");

    va_end(va);

    return 0;
}


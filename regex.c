/*

regex structures and routines
Copyright (C) 2013-2017 xinoir <xinoir@theta.black>
This file is part of Beta.

*/

#include "beta/error.h"
#include "beta/regex.h"

#define REGEX_ERRMSG_MAX 256

int regex_error(int errnum, const regex_t *regex)
{
    size_t regex_errnum = 0;
    char regex_errmsg[REGEX_ERRMSG_MAX] = {0};

    regex_errnum = regerror(errnum, regex, regex_errmsg, sizeof regex_errmsg);
    return error("REGEX ERROR: num: [%d], msg: [%s]\n", regex_errnum, regex_errmsg);
}


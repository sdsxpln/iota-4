/*
 * fatal utility structures and routines
 * Copyright (C) 2013-2017 xinoir <xinoir@theta.black>
 * This file is part of beta.
 */

#include <stdlib.h>
#include <errno.h>
#include "beta/log.h"
#include "beta/fatal.h"

noreturn void fatal(const char *format, ...)
{
    va_list va;

    va_start(va, format);
    log_fatal(format, va, errno);
    va_end(va);

    exit(EXIT_FAILURE);
}


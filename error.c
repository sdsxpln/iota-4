/*
 * error utility structures and routines
 * Copyright (C) 2013-2017 xinoir <xinoir@theta.black>
 * This file is part of beta.
 */

#include <errno.h>
#include "beta/log.h"
#include "beta/error.h"

int error(const char *format, ...)
{
    const int status = errno;
    va_list va;

    va_start(va, format);
    log_error(format, va, status);
    va_end(va);

    errno = status;
    return 1;
}


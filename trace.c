/*

trace utility structures and routines
Copyright (C) 2013-2017 Roman Fakhrazeyev <xinoir@theta.black>
This file is part of beta.

*/

#include <errno.h>
#include "beta/log.h"
#include "beta/notused.h"
#include "beta/trace.h"

#ifdef TRACE
void trace(const char *format, ...)
{
    const int status = errno;
    va_list va;

    va_start(va, format);
    log_trace(format, va);
    va_end(va);
    errno = status;
}
#else
void trace(const char *format, ...) { notused(format); }
#endif


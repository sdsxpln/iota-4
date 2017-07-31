/*
 * tests for log utility routines
 * Copyright (C) 2013-2017 xinoir <xinoir@theta.black>
 * This file is part of beta.
 */

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include "beta/log.h"

static int test_log_0004()
{
    const char message[] = "test logging a debug message with a line truncated";
    char buffer[4096];

    memcpy(buffer, message, sizeof(message) - 1);
    memset(buffer + sizeof(message) - 1, '0', sizeof(buffer) - sizeof(message));
    buffer[sizeof(buffer) - 1] = 0;

    log_debug(buffer, NULL);

    return 0;
}

static int test_log_0003(int arg, ...)
{
    va_list va;

    errno = 1;
    va_start(va, arg);
    log_error("test logging an error message with arguments [%s] and [%d], and errno", va, errno);
    va_end(va);

    assert(errno == 1);

    errno = 0;
    return 0;
}

static int test_log_0002()
{
    log_debug("test logging a debug message with no args", NULL);

    return 0;
}

static int test_log_0001(int arg, ...)
{
    va_list va;

    va_start(va, arg);
    log_debug("test logging a debug message with arguments [%s]", va);
    va_end(va);

    return 0;
}

int main(int argc, const char **argv)
{
    assert(argc != 0);
    const char *module = argv[0];

    log_init(module);

    if (test_log_0001(1, "string")) assert(0);
    if (test_log_0002()) assert(0);
    if (test_log_0003(1, "string", 1)) assert(0);
    if (test_log_0004()) assert(0);

    log_deinit(module);

    exit(EXIT_SUCCESS);
}


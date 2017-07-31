/*
 * tests for error utility routines
 * Copyright (C) 2013-2017 xinoir <xinoir@theta.black>
 * This file is part of beta.
 */

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include "beta/log.h"
#include "beta/error.h"

static int test_error_0005()
{
    int status;
    const char message[] = "test an error with a line truncated";
    char buffer[4096];

    memcpy(buffer, message, sizeof(message) - 1);
    memset(buffer + sizeof(message) - 1, '0', sizeof(buffer) - sizeof(message));
    buffer[sizeof(buffer) - 1] = 0;

    errno = 1;
    status = error(buffer);

    assert(status == 1);
    assert(errno == 1);

    errno = 0;
    return 0;
}

static int test_error_0004()
{
    int status;

    errno = 0;
    status = error("test an error with no arguments and no errno");

    assert(status == 1);
    assert(errno == 0);

    errno = 0;
    return 0;
}

static int test_error_0003()
{
    int status;

    errno = 1;
    status = error("test an error with no arguments and with errno");

    assert(status == 1);
    assert(errno == 1);

    errno = 0;
    return 0;
}

static int test_error_0002()
{
    int status;

    errno = 0;
    status = error("test an error with arguments [%s] and [%d], and no errno", "string", 1);

    assert(status == 1);
    assert(errno == 0);

    errno = 0;
    return 0;
}

static int test_error_0001()
{
    int status;

    errno = 1;
    status = error("test an error with arguments [%s] and [%d], and errno [%d]", "string", 1, errno);

    assert(status == 1);
    assert(errno == 1);

    errno = 0;
    return 0;
}

int main(int argc, const char **argv)
{
    assert(argc != 0);
    const char *module = argv[0];

    log_init(module);

    if (test_error_0001()) assert(0);
    if (test_error_0002()) assert(0);
    if (test_error_0003()) assert(0);
    if (test_error_0004()) assert(0);
    if (test_error_0005()) assert(0);

    log_deinit(module);

    exit(EXIT_SUCCESS);
}


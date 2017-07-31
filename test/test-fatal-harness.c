/*
 * a harness for fatal test routines
 * Copyright (C) 2013-2017 xinoir <xinoir@theta.black>
 * This file is part of beta.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "beta/log.h"
#include "beta/fatal.h"
#include "beta/error.h"
#include "beta/trace.h"
#include "beta/debug.h"

static void handle_exit()
{
    trace("a process exited");
}

int main(int argc, const char **argv)
{
    assert(argc == 1);
    const char *module = argv[0];

    log_init(module);

    if (atexit(handle_exit))
        error("an exit handler could not be registered, [%s]", module);

    errno = 1;
    fatal("test a fatal condition");
    
    assert(0);
}


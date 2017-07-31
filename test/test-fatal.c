/*
 * tests for fatal condition routines
 * Copyright (C) 2013-2017 xinoir <xinoir@theta.black>
 * This file is part of beta.
 */

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
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

static int test_fatal_0001()
{
    int status;
    const char *module = __func__;
    const char *program = "./test-fatal-harness";
    char sh_cmd[128] = { 0 };

    memmove(sh_cmd, program, strlen(program));
    debug("shell command: [%s]", sh_cmd);

    log_init(module);

    if (atexit(handle_exit))
        error("an exit handler could not be registered, [%s]", module);

    status = system(sh_cmd);
    assert(status == 256);

    log_deinit(module);

    return 0;
}

int main()
{
    if (test_fatal_0001()) assert(0);

    exit(EXIT_SUCCESS);
}


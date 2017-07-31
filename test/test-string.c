/*
 * tests for string library of routines
 * Copyright (C) 2013-2017 xinoir <xinoir@theta.black>
 * This file is part of beta.
 */

#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include "beta/string.h"

static int test_string_append()
{
    char buffer[128] = "a b c";
    
    string_append(buffer, sizeof(buffer), ", %s", "d e f");
    string_append(buffer, sizeof(buffer), ": [%d]", 256);
    assert(!strcmp(buffer, "a b c, d e f: [256]"));

    return 0;
}

static int test_string_reverse()
{
    char string[] = "abcdef";
    
    string_reverse(string);
    assert(!strcmp(string, "fedcba"));

    return 0;
}

static int test_string_from_int(void)
{
    char string[16] = { 0 };

    string_from_int(string, 0);
    assert(!strcmp(string, "0"));

    string_from_int(string, INT_MAX);
    assert(!strcmp(string, "2147483647"));

    string_from_int(string, INT_MIN);
    assert(!strcmp(string, "-2147483648"));

    return 0;
}

int main(void)
{
    if (test_string_reverse()) assert(0);
    if (test_string_from_int()) assert(0);
    if (test_string_append()) assert(0);

    exit(EXIT_SUCCESS);
}


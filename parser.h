/*
 * script parser structures and routines
 * Copyright (C) 2013-2017 xinoir <xinoir@theta.black>
 * This file is part of Beta.
 */

#ifndef SCRIPT_PARSER_H
#define SCRIPT_PARSER_H

#include <stdio.h>
#include "script.h"

int
script_parser_parse(
    FILE *stream,
    struct script *script
);

int
script_parser_init(
    void
);

#endif


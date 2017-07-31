/*
 * fatal utility structures and routines
 * Copyright (C) 2013-2017 xinoir <xinoir@theta.black>
 * This file is part of beta.
 */

#ifndef FATAL_H
#define FATAL_H

#include <stdarg.h>
#include "beta/noreturn.h"

noreturn void fatal(const char *format, ...);

#endif


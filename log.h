/*
 * log utility structures and routines
 * Copyright (C) 2013-2017 xinoir <xinoir@theta.black>
 * This file is part of beta.
 */

#ifndef LOG_H
#define LOG_H

#include <stdarg.h>

void log_debug(const char *format, va_list va);
void log_trace(const char *format, va_list va);
void log_warning(const char *format, va_list va);
void log_error(const char *format, va_list va, int error);
void log_fatal(const char *format, va_list va, int error);
void log_deinit(const char *module);
void log_init(const char *module);

#endif


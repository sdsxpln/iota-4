/*

string library of routines
Copyright (C) 2013-2017 Roman Fakhrazeyev <xinoir@theta.black>
This file is part of beta.

*/

#ifndef STRING_H
#define STRING_H

#include <stdarg.h>

int string_stream_word(FILE *f, char *w, size_t nc);
char *string_word(const char *s, char **p, char *w);
void string_reverse(char *string);
void string_from_int(char *string, int integer);
char *string_copy(const char *string, size_t size);
int string_append(char *buffer, size_t size, const char *string, ...);

#endif


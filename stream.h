/*

stream structures and routines
Copyright (C) 2013-2017 Roman Fakhrazeyev <xinoir@theta.black>
This file is part of Beta.

*/

#ifndef STREAM_H
#define STREAM_H

#include <stdio.h>
#include <sys/types.h>

int stream_read_word(int fd, char *w, size_t nc, int *iseof);
int stream_read_line(int fd, char *l, size_t nc, int *iseof);
int stream_write(int fd, void *b, size_t nb, ssize_t *nbw, int *iseof);
int stream_read(int fd, void *b, size_t nb, ssize_t *nbr, int *iseof);

#endif


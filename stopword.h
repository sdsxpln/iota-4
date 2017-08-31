/*

stopword routines
Copyright (C) 2013-2017 Roman Fakhrazeyev <xinoir@theta.black> 
This file is part of beta.

*/

#ifndef STOPWORD_H
#define STOPWORD_H

struct stopword;

int stopword_contains(const struct stopword *stopword, const char *word);
struct stopword *stopword_create(int stream);

#endif


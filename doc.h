/*

a doc structures and routines
Copyright (C) 2013-2017 xinoir <xinoir@theta.black>
This file is part of Beta.

*/

#ifndef DOC_H
#define DOC_H

struct doc;

int doc_length(const struct doc *doc);
int doc_terms(const struct doc *doc);
int doc_parse(struct doc *doc);
int doc_read(int fd, struct doc *doc);
void doc_destroy(struct doc **doc);
int doc_create(struct doc **doc);

#endif


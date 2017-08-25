/*

doc structures and routines
Copyright (C) 2013-2017 xinoir <xinoir@theta.black>
This file is part of Beta.

*/

#ifndef DOC_H
#define DOC_H

struct doc;

typedef int (* match_term_t)(struct doc_line **, struct doc_term **);
int doc_length(const struct doc *doc);
int doc_analyze(int fdo, const struct doc *doc);
int doc_parse(struct doc *doc, match_term_t **match_terms);
int doc_read(int fdi, struct doc *doc);
void doc_destroy(struct doc **doc);
int doc_create(struct doc **doc);

#endif


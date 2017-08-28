/*

doc structures and routines
Copyright (C) 2013-2017 xinoir <xinoir@theta.black>
This file is part of Beta.

*/

#ifndef DOC_H
#define DOC_H

typedef int doc_term_type;
struct doc_term;
struct doc_line;
struct doc;

struct doc_term {
    doc_term_type type;
    struct doc_line *line;
};

struct doc_line {
    struct list *terms;
    char *text;
    int size;
    int index;
};

struct doc {
    struct list *terms;
    struct doc_line **lines;
    int length;
};

typedef int (* match_term)(struct doc_line **, struct doc_term **);
int doc_length(const struct doc *doc);
int doc_analyze(int fdo, const struct doc *doc);
int doc_parse(struct doc *doc, match_term *term_matchers);
int doc_read(int fdi, struct doc *doc);
void doc_destroy(struct doc **doc);
int doc_create(struct doc **doc);

#endif


/*
 * a token structures and routines
 * Copyright (C) 2013-2017 xinoir <xinoir@theta.black>
 * This file is part of Beta.
 */

#ifndef TOKEN_H
#define TOKEN_H

#define WORD_SIZE 128

struct token;
char *token_string(const struct token *token);
void token_destroy(struct token **token);
int token_create(FILE *stream, struct token **token);

#endif


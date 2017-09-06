# Makefile for beta
# Copyright (C) 2013-2017 xinoir <xinoir@theta.black>
# This file is part of beta.

CC = cc
CFLAGS = -g -O0
CFLAGS += -Wall -Wextra -lm
CFLAGS += -iquote..
SYM = .dSYM

QUIET_CC = @echo CC $@;

objects += log.o
objects += debug.o
objects += trace.o
objects += error.o
objects += stream.o
objects += string.o
objects += list.o
objects += tree.o
objects += hash.o
objects += map.o
objects += regex.o
objects += stopword.o

commands += word

export objects

all: word
word: word.c $(objects); $(QUIET_CC)$(CC) $(CFLAGS) $(objects) word.c -o word
scan: scan.c $(objects); $(QUIET_CC)$(CC) $(CFLAGS) $(objects) scan.c -o scan
objects: $(objects)
$(objects): %.o: %.c %.h; $(QUIET_CC)$(CC) $(CFLAGS) -o $@ -c $<
test: force $(objects)
	@$(MAKE) -s -C test;
clean: force
	@rm -rf $(objects) $(commands) $(commands:=$(SYM));
	@$(MAKE) -s -C test clean;
force:


# Makefile for beta
# Copyright (C) 2013-2017 xinoir <xinoir@theta.black>
# This file is part of beta.

CC = cc
CFLAGS = -g -O0
CFLAGS += -Wall -Wextra
CFLAGS += -iquote..

QUIET_CC = @echo CC $@;

objects += log.o
objects += fatal.o
objects += error.o
objects += warning.o
objects += trace.o
objects += debug.o
objects += string.o

export objects

objects: $(objects)
$(objects): %.o: %.c %.h; $(QUIET_CC)$(CC) $(CFLAGS) -o $@ -c $<
test: force $(objects)
	@$(MAKE) -s -C test;
clean: force
	@rm -rf $(objects);
	@$(MAKE) -s -C test clean;
force:


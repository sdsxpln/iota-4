# Makefile for beta
# Copyright (C) 2013-2017 xinoir <xinoir@theta.black>
# This file is part of beta.

CC = cc
CFLAGS = -g -O0
CFLAGS += -Wall -Wextra
CFLAGS += -iquote../..
SYM = .dSYM

QUIET_CC = @echo CC $@;

prefixed_objects = $(addprefix ../,$(objects))

tests += test-log
tests += test-fatal
tests += test-error
tests += test-string
tests += test-list

harnesses += test-fatal-harness

test: force tests; @(for test in $(tests); do ./test.sh $$test; done)
tests: $(tests) $(harnesses);
$(tests): %: %.c; $(QUIET_CC)$(CC) $(CFLAGS) $(prefixed_objects) -o $@ $<
$(harnesses): %: %.c; $(QUIET_CC)$(CC) $(CFLAGS) $(prefixed_objects) -o $@ $<
clean: force;
	@rm -rf \
        $(tests) $(tests:=.o) $(tests:=.dSYM) \
        $(harnesses) $(harnesses:=.o) $(harnesses:=.dSYM)
force:


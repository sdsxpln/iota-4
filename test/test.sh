#!/bin/sh

# test utility script for Makefile in Beta
# Copyright (C) 2013-2017 xinoir <xinoir@theta.black>
# This file is part of Beta.

$(./$@);
printf "%s %s\n" $@ $([ "$?" -eq "0" ] && echo "PASS" || echo "FAIL");


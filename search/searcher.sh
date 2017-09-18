#!/bin/sh

## search routines
## Copyright (C) 2013-2017 Roman Fakhrazeyev <xinoir@theta.black> 
## This file is part of iota.

umask 022;
set -o posix;
set -o errexit;
set -o notify;
set -o pipefail;
set -o nounset;
set -o errtrace;
set +o keyword;
set +o allexport;
set -o noclobber;
set +o hashall;

DEBUG=0;
relpath=$(dirname "$0");
path=$(realpath -m "$relpath");
pushd $relpath >/dev/null;

module_path="$path";
build_path="$module_path/.build";
apache_lucene="$build_path/lucene-6.6.0";
classpath="$apache_lucene/core/lucene-core-6.6.0.jar";
classpath="$classpath:$apache_lucene/queryparser/lucene-queryparser-6.6.0.jar";
classpath="$classpath:$apache_lucene/analysis/common/lucene-analyzers-common-6.6.0.jar";
classpath="$classpath:$build_path";

set +o nounset;
for ((index=1; index < $#; index+=2)); do
    [ "${!index}" = "-u" ] && { shift; idxuri=$1; shift; }
    [ "${!index}" = "-t" ] && { shift; idxtype=$1; shift; }
    [ "${!index}" = "-m" ] && { shift; maxhits=$1; shift; }
    [ "${!index}" = "-o" ] && { shift; offset=$1; shift; }
    [ "${!index}" = "-d" ] && { shift; debug=$1; shift; }
done;
[ -z "$idxuri" ] && { printf "an index uri is required\n"; exit 1; }
[ -z "$idxtype" ] && { printf "an index type is required\n"; exit 1; }
[ -z "$maxhits" ] && { printf "a max hits integer is required\n"; exit 1; }
[ -z "$offset" ] && { printf "an offset is required\n"; exit 1; }
[ -z "$debug" ] && { printf "a debug flag is required\n"; exit 1; }
set -o nounset;
declare -r idxuri=$idxuri;
declare -r idxtype=$idxtype;
declare -r maxhits=$maxhits;
declare -r offset=$offset;
declare -r debug=$debug;

java -cp $classpath black.theta.Searcher $idxuri $idxtype $maxhits $offset $debug;

popd >/dev/null;


#!/bin/sh

## index routines
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
source_path="$module_path/black/theta";
apache_lucene="$build_path/lucene-6.6.0";
classpath="$apache_lucene/core/lucene-core-6.6.0.jar";
classpath="$classpath:$apache_lucene/queryparser/lucene-queryparser-6.6.0.jar";
classpath="$classpath:$apache_lucene/analysis/common/lucene-analyzers-common-6.6.0.jar";
classpath="$classpath:$build_path";
export CLASSPATH="$classpath";

set +o nounset;
for ((index=1; index < $#; index+=2)); do
    [ "${!index}" = "-u" ] && { shift; idxuri=$1; shift; }
    [ "${!index}" = "-t" ] && { shift; idxtype=$1; shift; }
done;
[ -z "$idxuri" ] && { printf "an index uri is required\n"; exit 1; }
[ -z "$idxtype" ] && { printf "an index type is required\n"; exit 1; }
set -o nounset;
declare -r idxuri=$idxuri;
declare -r idxtype=$idxtype;

test_input=$(cat<<-@
The quick brown fox jumps over the lazy dog
@
);

install -d $source_path;
ln -sfr *.java $source_path;

[[ $DEBUG == 1 ]] && {  
    java \
        -agentlib:jdwp=transport=dt_socket,server=y,address=8000 \
        black.theta.Indexer $idxuri $idxtype;
    jdb \
        -sourcepath . \
        -connect com.sun.jdi.SocketAttach:hostname=localhost,port=8000;
} || {
    java black.theta.Indexer $idxuri $idxtype;
}

popd >/dev/null;


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
apache_lucene_version="6.6.0";
apache_lucene_path="$build_path/lucene-$apache_lucene_version";
apache_lucene_dist_url="http://www-us.apache.org/dist/lucene/java";

[ ! -d $build_path ] && {
    install -dv $build_path;
}

[ ! -d $apache_lucene_path ] && {

    pushd $build_path >/dev/null;

    wget $apache_lucene_dist_url/$apache_lucene_version/KEYS;
    wget $apache_lucene_dist_url/$apache_lucene_version/lucene-$apache_lucene_version.tgz.asc;
    wget $apache_lucene_dist_url/$apache_lucene_version/lucene-$apache_lucene_version.tgz.sha1;
    wget $apache_lucene_dist_url/$apache_lucene_version/lucene-$apache_lucene_version.tgz;

    gpg --import "KEYS";
    gpg --verify lucene-$apache_lucene_version.tgz.asc lucene-$apache_lucene_version.tgz;
    tar -xvf lucene-$apache_lucene_version.tgz;

    popd >/dev/null;

}

classpath="$apache_lucene_path/core/lucene-core-6.6.0.jar";
classpath="$classpath:$apache_lucene_path/queryparser/lucene-queryparser-6.6.0.jar";
classpath="$classpath:$apache_lucene_path/analysis/common/lucene-analyzers-common-6.6.0.jar";
classpath="$classpath:$apache_lucene_path/analysis/icu/lucene-analyzers-icu-6.6.0.jar";
javac -cp $classpath @OPTIONS @CLASSES;

popd >/dev/null;


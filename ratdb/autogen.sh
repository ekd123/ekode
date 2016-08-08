#!/bin/sh
set -x 
gtkdocize || exit 1 
autoreconf -i || exit 1
if [[ "$@" == "" ]]; then
    echo "I'm going to run configure without arguments."
fi
./configure $@ || exit 1


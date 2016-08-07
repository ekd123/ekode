#!/bin/sh
clang *.c `pkg-config --cflags --libs gobject-introspection-1.0 lua-5.1 glib-2.0 gobject-2.0` -o test
g-ir-scanner --namespace=T --nsversion=1.0 --include=GObject-2.0 --pkg=gobject-2.0 --program=./test test.c t-obj.c t-obj.h -o T-1.0.gir
g-ir-compiler T-1.0.gir -o T-1.0.typelib


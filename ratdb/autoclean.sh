#!/bin/sh

gmake distclean > /dev/null

(find . -name Makefile.in -exec rm {} \;)
(find . -name \#\* -exec rm {} \;)
(find . -name \*~ -exec rm {} \;)
(find . -name \*.core -exec rm {} \;)
(find . -name .clang-completion-error -exec rm {} \;)

(rm -rf autom4te.cache)
(rm -f aclocal.m4)
(rm -rf build-aux)
(rm -rf configure)
(rm -rf gtk-doc.make)
(cd m4; rm -f lt~obsolete.m4 ltsugar.m4 libtool.m4 ltoptions.m4 ltversion.m4 gtk-doc.m4 intltool.m4)

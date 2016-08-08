#!/bin/sh
rm -f ratdb-dbus-proxy.[ch]
gdbus-codegen --interface-prefix org.ratdb. \
    --generate-c-code ratdb-dbus-proxy \
    --c-namespace Ratdb \
    ratdb-dbus-interface.xml

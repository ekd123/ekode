#!/bin/sh
gdbus-codegen --interface-prefix org.home.hsession.Manager \
    --generate-c-code hs-dbus-manager \
    --c-namespace Hs \
    --c-generate-object-manager \
    hs-dbus-manager.xml


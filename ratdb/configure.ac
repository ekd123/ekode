AC_PREREQ([2.68])
AC_INIT(ratdb, 0.0, https://github.com/ratdb/ratdb/)
m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])
AC_CONFIG_SRCDIR([src/ratdb-main.c])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_HEADERS([config/config.h])

# initialize automake
AM_INIT_AUTOMAKE([-Wno-portability])
AM_MAINTAINER_MODE

# initialize i18n
IT_PROG_INTLTOOL([0.35.0])

GETTEXT_PACKAGE=[ratdb]
LOCALEDIR=[locale]
AC_SUBST(GETTEXT_PACKAGE)
AC_SUBST(LOCALEDIR)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE", [GETTEXT package name])

LIB_GETTEXT_PACKAGE=[libratdb]
LIB_LOCALEDIR=[locale]
AC_SUBST(LIB_GETTEXT_PACKAGE)
AC_SUBST(LIB_LOCALEDIR)

# libtool
AC_PROG_LIBTOOL
AM_PROG_LIBTOOL
LT_INIT([dlopen])

# pkg-config
PKG_CONFIG=[pkg-config]
PKG_PROG_PKG_CONFIG

# Checks for programs.
AM_PROG_CC_C_O
GLIB_GSETTINGS
GTK_DOC_CHECK([1.14],[--flavour no-tmpl])
GOBJECT_INTROSPECTION_CHECK([1.30.0])

# Checks for libraries.
PKG_CHECK_MODULES(RATDB, [
uuid \
gio-2.0 \
gthread-2.0 \
gmodule-2.0 \
glib-2.0 >= 2.32 \
gobject-2.0 \
gio-unix-2.0 >= 2.8
])
AC_SUBST(RATDB_CFLAGS)
AC_SUBST(RATDB_LIBS)

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([
Makefile

src/Makefile
src/ratdb.pc

libratdb/Makefile
libratdb/libratdb.pc

po/Makefile.in

docs/Makefile
docs/reference/Makefile
docs/reference/ratdb/Makefile])

AC_OUTPUT

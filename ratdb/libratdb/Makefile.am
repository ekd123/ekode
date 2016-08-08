### 
lib_LTLIBRARIES = liblibratdb.la
liblibratdbincludedir = $(includedir)/libratdb/libratdb

###
liblibratdb_la_SOURCES = \
	ratdb-client.c \
	ratdb-settings.c \
	ratdb-command.c 

liblibratdbdir = $(includedir)/libratdb/libratdb
liblibratdb_HEADERS = \
	ratdb-client.h \
	ratdb-settings.h \
	ratdb-command.h \
	ratdb.h

liblibratdb_la_LIBADD = $(RATDB_LIBS)

LIB_CFLAGS = $(RATDB_CFLAGS) -I$(top_srcdir) -DLOCALEDIR="\"$(LOCALEDIR)\"" \
	-DG_LOG_DOMAIN=\"RatDB\"
liblibratdb_la_CFLAGS = -Wall -O3 $(RATDB_CFLAGS) \
	$(LIB_CFLAGS) -std=c99 -pedantic

### pkg-config
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libratdb.pc
DISTCLEANFILES = libratdb.pc

### GObject Introspection
-include $(INTROSPECTION_MAKEFILE)
INTROSPECTION_GIRS = 
INTROSPECTION_SCANNER_ARGS = --add-include-path=$(srcdir)
INTROSPECTION_COMPILER_ARGS = --includedir=$(srcdir)

if HAVE_INTROSPECTION
Ratdb.gir: liblibratdb.la
Ratdb_gir_INCLUDES = GObject-2.0 GLib-2.0 Gio-2.0
Ratdb_gir_CFLAGS = $(LIB_CFLAGS)
Ratdb_gir_LIBS = liblibratdb.la
Ratdb_gir_FILES = $(liblibratdb_la_SOURCES) $(liblibratdb_HEADERS)
Ratdb_gir_SCANNERFLAGS = --namespace=Ratdb
Ratdb_gir_EXPORT_PACKAGE = ratdb
INTROSPECTION_GIRS += Ratdb.gir

girdir = $(datadir)/gir-1.0
gir_DATA = $(INTROSPECTION_GIRS)

typelibsdir = $(libdir)/girepository-1.0
typelibs_DATA = $(INTROSPECTION_GIRS:.gir=.typelib)

CLEANFILES = $(gir_DATA) $(typelibs_DATA)
endif

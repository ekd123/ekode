############# GSettings
gsettings_SCHEMAS = org.ratdb.gschema.xml
@GSETTINGS_RULES@

EXTRA_DIST = ${gsettings_SCHEMAS}

############# Fake Library, idea from RhythmCat2 project
libratdbd_sources = \
	ratdb-daemon.c \
	ratdb-server.c \
	ratdb-client.c \
	ratdb-dbus-proxy.c \
	ratdb-settings.c \
	ratdb-file.c \
	ratdb-engine.c \
	ratdb-options.c \
	ratdb-signal.c \
	ratdb-loader.c \
	ratdb-uuid.c

libratdbd_headers = \
	ratdb-engine.h \
	ratdb-enums.h \
	ratdb-file.h \
	ratdb-backend.h \
	ratdb-macros.h

libratdbd_noinst_headers = \
	ratdb-daemon.h \
	ratdb-server.h \
	ratdb-client.h \
	ratdb-dbus-proxy.h \
	ratdb-settings.h \
	ratdb-signal.h \
	ratdb-options.h \
	ratdb-loader.h \
	ratdb-uuid.h

libratdbd_la_SOURCES = $(libratdbd_sources)

NOINST_H_FILES = \
	$(libratdbd_noinst_headers)
noinst_LTLIBRARIES = libratdbd.la
libratdbdincludedir = $(includedir)/ratdb/ratdb
libratdbdinclude_HEADERS = $(libratdbd_headers)

############# pkg-config
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = ratdb.pc
DISTCLEANFILES = ratdb.pc

libratdbd_la_LDFLAGS = -export-dynamic -O2 -no-undefined -export-symbols-regex "^[[^_]].*"
libratdbd_la_LIBADD = @RATDB_LIBS@ @GLIB2_LIBS@

AM_CFLAGS =  -Wall -g $(RATDB_CFLAGS) -std=c99 \
	-I$(top_srcdir) -DLOCALEDIR="\"$(LOCALEDIR)\"" \
	-DG_LOG_DOMAIN="\"RatDB\"" \
	-DRATDB_ENGINES_DIR="\"${libdir}/ratdb/engines\"" \
	-DRATDB_FILEBACKENDS_DIR="\"${libdir}/ratdb/filebackends\""

libratdbd_la_CFLAGS = $(AM_CFLAGS)

############# Program 
bin_PROGRAMS = ratdbd

ratdbd_SOURCES = \
	ratdb-main.c 

ratdbd_CFLAGS = $(AM_CFLAGS)
ratdbd_DEPENDENCIES = @LIBOBJS@ libratdbd.la
ratdbd_LDADD = $(RATDB_LIBS) libratdbd.la

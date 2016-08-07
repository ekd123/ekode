CC=gcc

all:
	gdbus-codegen --interface-prefix org.ratdb.Engine.						\
              --generate-c-code ratdb-engine-dbus							\
              --c-namespace RatdbDbus										\
              --c-generate-object-manager									\
              ratdb-server.xml
	$(CC) server.c ratdb-engine-dbus.c -o server.elf `pkg-config --cflags --libs gio-2.0 gio-unix-2.0`
	$(CC) client.c ratdb-engine-dbus.c -o client.elf `pkg-config --cflags --libs gio-2.0 gio-unix-2.0`

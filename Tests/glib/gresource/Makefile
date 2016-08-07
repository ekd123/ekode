cc = gcc

all: a.out
a.out: a.c data.c data.h
	$(cc) a.c data.c `pkg-config --cflags --libs gio-2.0`
data.c:
	glib-compile-resources data.gresource.xml --generate-source --manual-register
data.h:
	glib-compile-resources data.gresource.xml --target data.h --generate-header --manual-register
clean:
	rm a.out data.[ch]

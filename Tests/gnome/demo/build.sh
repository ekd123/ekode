echo --- Compiling GResource ---
glib-compile-resources demoapp.gresource.xml --target=resources.c --generate-source

echo --- Compiling GSettings schema ---
glib-compile-schemas .

echo --- Building ---
cc *.c $(pkg-config --cflags --libs gtk+-3.0 gio-2.0) -o demo

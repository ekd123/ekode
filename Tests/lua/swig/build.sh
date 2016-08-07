swig2.0 -lua example.i
clang *.c `pkg-config --cflags --libs lua-5.1` -o test
./test a.lua
rm test example_wrap.c


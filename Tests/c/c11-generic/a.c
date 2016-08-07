/*
compile me with 'clang -std=c1x a.c'
currently gcc4.7.0 doesn't support generic yet.
*/
#include <stdio.h>

#define put(x) _Generic((x), char*:put_string, int: put_int)(x)

void put_string (char *string)
{
    puts (string);
}
void put_int (int x)
{
    printf ("%d\n", x);
}

int 
main (void)
{
    put (123);
    put ("hello world"+0);
    put ("a test for C11's _Generic"+0);
    return 0;
}


#include <stdio.h>

void foo (void)
{
    printf ("hello world\n");
}

int main (void)
{
    void (*ptr)(void) = foo;
    foo ();
    return 0;
}

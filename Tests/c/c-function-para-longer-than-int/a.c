#include <stdio.h>

/*
  on my 64-bit Linux box

  sizeof (long long) == 64
  sizeof (int) == 32
*/

long long
foo (long long a)
{
    return a;
}

int main (int argc,
          char *argv[])
{
    long long x = foo (123);

    return 0;
}

#include <stdio.h>
#include "sharedmem.h"

int main ()
{
    void *bytes = sm_alloc (100);
    printf ("ref: %d, length: %ld\n", sm_refcount (bytes), sm_length (bytes));
    return 0;
}


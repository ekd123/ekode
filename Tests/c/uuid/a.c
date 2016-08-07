/* clang a.c `pkg-config --cflags --libs uuid` */

#include <uuid/uuid.h>
#include <stdlib.h>
#include <stdio.h>

int main (void)
{
    uuid_t uuid;
    char out[100];
    uuid_generate (uuid);
    uuid_unparse (uuid, out);
    if (out)
        puts (out);
    return 0;
}

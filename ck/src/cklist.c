/* cklist uses darr - the typeless dynamic array library. */
#include "darr/darr.h"
#include "cklist.h"

static darr list;

void 
_ck_list_init (void)
{
    darr_new (&list, sizeof (pid_t));
}

unsigned int
_ck_list_get_length (void)
{
    return list.num;
}

int
_ck_list_find (pid_t pid)
{
    int i;
    pid_t t;

    for (i = 0; i<list.num; i++)
    {
        t = *(pid_t*)darr_a (&list, i);
        if (t == pid)
            return i;
    }

    return -1;
}

void
_ck_list_del (pid_t pid)
{
    int location;
    location = _ck_list_find (pid);
    if (location < 0)
        return;
    darr_del (&list, location);
}

pid_t *
_ck_list_get (void)
{
    return (pid_t *)list.mem;
}

void 
_ck_list_destroy (void)
{
    darr_free (&list);
}

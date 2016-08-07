/**
   for test darr.
   written by mike ma <zhtx10@gmail.com>.
 */

#include "../darr.h"
#include <stdio.h>

void pr_status(darr d)
{
    printf ("mem=0x%p\n"
            "len=%ld\n"
            "num=%ld\n"
            "len_of_item=%ld\n",
            d.mem, d.len, d.num, d.len_of_item);
}

void pr_arr (darr *pd)
{
    int i;
    for(i=0; i < pd->num; i++)
    {
        /* pr_status(*pd); */
        printf ("%ld\t", *(long *)darr_a(pd, i));
    }
    puts ("");
}

int main (int argc, char *argv[])
{
    darr d;         /* a dynamic array of long */
    darr_new (&d, sizeof (long));

#define ACCESS(d,n) *(long *)darr_a(& d,n)

    /* add 10 items */
    int i;
    darr_add_n (&d, 10);
    for (i=0; i<d.num; i++)
        ACCESS(d,i) = i;

    /* print */
    pr_arr (&d);/* d.num=10 */

    /* delete all the items */

    int t = d.num;
    for (i = 0; i < t; i++)
    {
        printf ("loop %d\n", i);
        darr_del (&d, 0);/* this will change d.num. */
    }

    /* print... again... shouldn't print anything */
    printf ("TTT: ");
    pr_arr (&d);

    /* end */
    printf ("done.\n");

    /* free */
    darr_free (&d);

    return 0;
}



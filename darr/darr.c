/**
 * darr.c
 * created on 2011/7/16.
 * written by mike ma<zhtx10@gmail.com>
 *
 * this file provides dynamic arrays (not list).
 * see the example below.
 */

#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include "darr.h"

/******a simple example:

  darr d; //a dynamic array of int.
  darr_new (&d, sizeof (int));
  darr_add (&d);
  darr_add (&d);
  *(int*)darr_a (&d, 0) = 12;
  *(int*)darr_a (&d, 1) = 13;
  darr_del (&d, 0);
  darr_del (&d, 0);//you should know why.
  darr_free (&d);

  *******************************************
  if you'd like a completed example, see tests/test_darr.c
 */

/* new. */
void darr_new (darr *d, size_t len_of_item)
{
    d->mem = NULL;
    d->len = 0;
    d->num = 0;
    d->len_of_item = len_of_item;
}

/* add */
void darr_add (darr *d)
{
    d->len = d->len_of_item * (d->num + 1);
    d->mem = realloc (d->mem, d->len);
    d->num ++;
}

/* add n items */
void darr_add_n (darr *d, long n)
{
    long i;
    for (i = 0; i < n; i++)
        darr_add (d);
}

/* access it. */
/* get a pointer to the item */
void *darr_a (darr *d, long location)
{
    return (d->mem + (location * d->len_of_item));
}

/* delete */
void darr_del (darr *d, long location)
{
    darr_move (d, location + 1, d->num - location, location);
    d->len = d->len_of_item * (d->num - 1);
    d->mem = realloc (d->mem, d->len);
    d->num --;
}

/* move */
/*
  this function is kind of difficult.
  this is a example:
      darr_move(&rdarr, 1, 1, 2);
  will move the 2nd item to the third.
  this is an advanced example:
      darr_move(&rdarr, 1, 3, 2);
  just like this:
  ---------------------------
  start:  0 1 2 3 4 5 6 7 8 ...
  called: 0 ? 1 2 3 5 6 7 8 ...
  ---------------------------

  if the memory requested isn't allocated, that's undefined.
  remember 0.**        :-)


  you should not use it, because it
  doesn't modify num and len. you have 
  to modify it to let other functions work.
  good luck.
 */
void darr_move (darr *d, long lfrom, long number, long lto)
{
    d->mem = memmove (d->mem  +  (lto   * d->len_of_item), 
                      d->mem  +  (lfrom * d->len_of_item), 
                      number  *   d->len_of_item);
}

/* free it. */
void darr_free (darr *d)
{
    free (d->mem);
    darr_new (d, 0);
}



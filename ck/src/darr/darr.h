#ifndef DARR_H

#include <sys/types.h>

typedef struct
{
    /* Do NOT modify the data below!!!!!!!!!! */
    void *mem;
    long  len; /* len of mem.*/
    long  num; /* number of items.*/
    long  len_of_item; 
} darr;

void  darr_new   (darr *d, size_t len_of_item);
void  darr_add   (darr *d);
void  darr_add_n (darr *d, long n);
void *darr_a     (darr *d, long location);
void  darr_del   (darr *d, long location);
void  darr_move  (darr *d, long lfrom, long number, long lto);
void  darr_free  (darr *d);

#endif /*DARR_H*/

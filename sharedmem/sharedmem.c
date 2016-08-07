#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "sharedmem.h"

#define OFFSET(struct_type, member)\
   ((long) ((char*) &((struct_type*) 0)->member))

struct memchunk 
{
    size_t length;
    unsigned int refcount;
    bool freedp;
    void *bytes;
};

static void *_sm_to_ptr (void *ptr)
{
    return ((struct memchunk*)ptr)->bytes;
}

static struct memchunk *_ptr_to_sm (void *ptr)
{
    return (struct memchunk *)ptr-OFFSET(struct memchunk, bytes);
}

void *sm_alloc (size_t length)
{
    struct memchunk *chunk;
    chunk = malloc (sizeof (struct memchunk));
    chunk->length = length;
    chunk->refcount = 1;
    chunk->freedp = false;
    chunk->bytes = malloc (length);
    return _sm_to_ptr (chunk);
}

void *sm_realloc (void *ptr, size_t length)
{
    struct memchunk *chunk;
    chunk = (struct memchunk *)_ptr_to_sm (ptr);
    chunk->bytes = realloc (chunk->bytes, length);
    chunk->length = length;
    return _sm_to_ptr (ptr);
}

void sm_free (void *ptr)
{
    if (_ptr_to_sm (ptr)->freedp == true)
    {
        printf ("Pointer 0x%lX had been freed!", (unsigned long)ptr);
        return;
    }
    free (ptr);
    _ptr_to_sm (ptr)->freedp = true;
    free (_ptr_to_sm (ptr));
}

void *sm_ref (void *ptr)
{
    _ptr_to_sm (ptr)->refcount ++;
    return ptr;
}

void *sm_unref (void *ptr)
{
    _ptr_to_sm (ptr)->refcount --;
    if (_ptr_to_sm (ptr)->refcount <= 0)
    {
        sm_free (ptr);
        return NULL;
    }
    return ptr;
}

size_t sm_length (void *ptr)
{
    return _ptr_to_sm (ptr)->length;
}

unsigned int sm_refcount (void *ptr)
{
    return _ptr_to_sm (ptr)->refcount;
}


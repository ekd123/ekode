#ifndef SHAREDMEM_H
#define SHAREDMEM_H

void *sm_alloc (size_t length);
void *sm_realloc (void *ptr, size_t length);
void sm_free (void *ptr);
void *sm_ref (void *ptr);
void *sm_unref (void *ptr);
size_t sm_length (void *ptr);
unsigned int sm_refcount (void *ptr);

#endif

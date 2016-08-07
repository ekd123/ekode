#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "core.h"

struct WenObject * 
wen_create_symbol (const char *symbol_name)
{
    struct WenObject *object = malloc (sizeof (struct WenObject));

    object->original_type = WEN_ATOM;
    object->real_type = WEN_RAW;
    object->symbol_name = strdup (symbol_name);
    object->refcount = 1;

    return object;
}

struct WenObject * 
wen_create_string (const char *src)
{
    struct WenObject *object = malloc (sizeof (struct WenObject));

    object->original_type = WEN_ATOM;
    object->real_type = WEN_STRING;
    object->attached_data = (void *)strdup (src);
    object->refcount = 1;

    return object;
}

/**
 * wen_create_list:
 * @first: the first item
 * @next: the next item
 * 
 * Create a new list object.
 * 
 * Returns: an object.
 */ 
struct WenObject *
wen_create_list (struct WenObject *first,
                 struct WenObject *next)
{
    struct WenObject *object = malloc (sizeof (struct WenObject));

    object->original_type = WEN_LIST;
    object->real_type = WEN_RAW;
    object->first = first;
    object->next = next;
    object->attached_data = NULL;
    object->refcount = 1;

    return object;
}

void 
wen_dump_list (struct WenObject *list)
{
    list->refcount ++;

    printf ("(");
    printf ("%s . ", list->first->symbol_name);
    if (list->next)
        wen_dump_list (list->next);
    else 
        printf ("nil");
    printf (")");

    list->refcount --;
}

void 
wen_unref_object (struct WenObject *object)
{
    if (!object) return;
    object->refcount --;
    if (object->refcount <= 0)
    {
        if (object->original_type == WEN_ATOM) 
        {
            if (object->real_type == WEN_RAW)
                free (object->symbol_name);
            else if (object->real_type == WEN_STRING)
                free (object->attached_data);
        }
        else 
        {
            wen_unref_object (object->first); // unref 1st item
            wen_unref_object (object->next); // unref rest
        }
        free (object); // free the structure itself
    }
}

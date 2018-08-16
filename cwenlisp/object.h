#ifndef OBJECT_H
#define OBJECT_H

#include "type.h"

typedef struct _Object Object;

Object *object_new();
void object_ref(Object *obj);
void object_unref(Object *obj);

#endif /* OBJECT_H */

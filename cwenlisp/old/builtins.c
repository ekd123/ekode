#include "core.h"

/**
 * wen_buildtin_listp:
 * @object: an object
 * 
 * Check wether @object is a list.
 * 
 * NOTE: this function is mapped as "list?".
 * 
 * Returns: true if @object is a list.
 */
bool wen_buildtin_listp (struct WenObject *object)
{
    if (object->original_type == WEN_LIST)
        return true;
    return false;
}

/**
 * wen_buildtin_atomp:
 * @object: an object
 * 
 * Check whether @object is an atom.
 * 
 * NOTE: this function is mapped as "atom?".
 * 
 * Returns: true if @object is an atom.
 */
bool wen_buildtin_atomp (struct WenObject *object)
{
    return !wen_buildtin_listp (object);
}

bool wen_buildtin_stringp (struct WenObject *object)
{
    return (object->real_type==WEN_STRING)?true:false;
}

/**
 * wen_buildtin_first:
 * @object: found the first item
 * 
 * Returns the first item.
 * 
 * [example]
 * (first '(a b)) ; ==> a
 * [/example]
 * 
 * When @object is nil, it returns nil.
 * 
 * Returns: object->first
 */
struct WenObject *wen_buildtin_first (struct WenObject *object)
{
    return (object)?object->first:NULL;
}

/**
 * wen_buildtin_rest:
 * @object: found the rest items
 * 
 * Returns the rest items.
 * 
 * [example]
 * (first '(a b)) ; ==> (b)
 * [/example]
 * 
 * When @object is nil, it returns nil.
 * 
 * Returns: @object->next
 */
struct WenObject *wen_buildtin_rest (struct WenObject *object)
{
    return (object)?object->next:NULL;
}

/**
   object := src
*/
void wen_buildtin_set (struct WenObject *object,
                       struct WenObject *src)
{
}

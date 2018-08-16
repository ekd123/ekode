#include <stddef.h>
#include <stdbool.h>

typedef enum {
    WEN_ATOM,
    WEN_LIST
} WEN_OTYPE;

typedef enum {
    WEN_RAW, /* used for symbol and list */
    WEN_STRING
} WEN_RTYPE;

struct WenObject {
    WEN_OTYPE original_type;
    WEN_RTYPE real_type;
    unsigned long refcount;
    struct WenObject *first; // "this"
    void *attached_data;
    union {
        char *symbol_name;
        struct WenObject *next;
    };
};

struct WenObject * wen_create_symbol (const char       *symbol_name);
struct WenObject * wen_create_list   (struct WenObject *first,
                                      struct WenObject *next);
struct WenObject * wen_create_string (const char       *src);
void               wen_dump_list     (struct WenObject *list);
void               wen_unref_object  (struct WenObject *object);



#ifndef __T_OBJ_H__
#define __T_OBJ_H__

#include <glib-object.h>


G_BEGIN_DECLS

#define T_TYPE_OBJ (t_obj_get_type())
#define T_OBJ(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj),T_TYPE_OBJ,Obj))
#define T_OBJ_CLASS(klass)                                              \
   (G_TYPE_CHECK_CLASS_CAST ((klass),                                   \
                             T_TYPE_OBJ,                                \
                             TObjClass))
#define T_IS_OBJ(obj)                                                   \
   (G_TYPE_CHECK_INSTANCE_TYPE ((obj),                                  \
                                T_TYPE_OBJ))
#define T_IS_OBJ_CLASS(klass)                                           \
   (G_TYPE_CHECK_CLASS_TYPE ((klass),                                   \
                             T_TYPE_OBJ))
#define T_GET_OBJ_CLASS(obj)                                            \
   (G_TYPE_INSTANCE_GET_CLASS ((obj),                                   \
                               T_TYPE_OBJ,                              \
                               TObjClass))

typedef struct _TObj      TObj;
typedef struct _TObjClass TObjClass;

struct _TObjClass
{
    GObjectClass parent_class;
};

struct _TObj
{
    GObject parent;
};

GType  t_obj_get_type (void);
gchar *t_obj_plus     (const gchar *str,
                       gint         number);

G_END_DECLS

#endif /* __T_OBJ_H__ */

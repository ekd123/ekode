#include "t-obj.h"

/**
 * SECTION: t-obj
 * @Title: TObj
 * 
 * Something tricky
 */

static void t_obj_dispose (GObject *object);
static void t_obj_finalize (GObject *object);

G_DEFINE_TYPE (TObj, t_obj, G_TYPE_OBJECT)

static void
t_obj_class_init (TObjClass *klass)
{
    GObjectClass *gobject_class = (GObjectClass *)klass;

    gobject_class->dispose = t_obj_dispose;
    gobject_class->finalize = t_obj_finalize;
}

static void
t_obj_init (TObj *self)
{
}

static void
t_obj_dispose (GObject *object)
{
    TObj *self = (TObj *)object;

    G_OBJECT_CLASS (t_obj_parent_class)->dispose (object);
}

static void
t_obj_finalize (GObject *object)
{
    TObj *self = (TObj *)object;

    g_signal_handlers_destroy (object);
    G_OBJECT_CLASS (t_obj_parent_class)->finalize (object);
}

/**
 * t_object_plus:
 * @str: the str
 * @number: number
 *
 * plus number and str
 * 
 * Returns: (transfer full): the data
 */
gchar *
t_obj_plus (const gchar *str,
            gint         number)
{
    return g_strdup_printf ("%s: %d", str, number);
}

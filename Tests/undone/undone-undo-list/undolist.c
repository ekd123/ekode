#include <glib.h>
#include "undolist.h"

UndoList *
undo_list_new (void)
{
    UndoList *ptr;

    ptr = g_malloc (sizeof (UndoList));
    ptr->queue = g_queue_new ();
    ptr->position = -1;
    ptr->desc_list = g_slist_alloc ();

    return ptr;
}

static void
__list_slist_free_callback (UndoInfo * data)
{
    undo_info_free (data);
}
static void
__list_queue_free_callback (UndoItem * data,
                            gpointer user_data)
{
    undo_item_free (data);
}
void
undo_list_free (UndoList *list)
{
    g_slist_free_full (list->desc_list, __list_slist_free_callback);
    g_queue_foreach (list->queue, __list_queue_free_callback, NULL);
    g_queue_free (list->queue);
}

UndoItem *
undo_item_new (enum UndoBehavior code,
               gpointer data)
{
    UndoItem *item;

    item = g_malloc (sizeof (UndoItem));
    item->

    return item;
}

UndoInfo *
undo_info_new (enum UndoBehavior behavior,
               GFunc callback,
               const gchar *desc);
{
    UndoInfo *info;

    info = g_malloc (sizeof (UndoInfo));
    info->code = behavior;
    info->callback = callback;
    info->description = g_strdup (desc);

    return info;
}

void
undo_info_free (UndoInfo * info)
{
    g_free (info->description);
    g_free (info);
}

gboolean
undo_info_register (UndoList *list,
                    UndoInfo *info)
{
    
}


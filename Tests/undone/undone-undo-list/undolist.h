#ifndef __UNDO_LIST_H__
#define __UNDO_LIST_H__

enum UndoBehavior
{
    BEHAVIOR_INVALID = 0,
    BEHAVIOR_HIT_KEY = 1<<0
};


typedef struct
{
    enum UndoBehavior code;
    GFunc callback;
    gchar *description;
}  UndoInfo;

typedef struct
{
    UndoInfo info;
    gpointer data;
}  UndoItem;

typedef struct
{
    GQueue *queue;
    GSList *desc_list;
    signed int position;
} UndoList;

/* create an undo item */
UndoItem *undo_item_new (enum UndoBehavior code, 
                         gpointer data);
void undo_item_free (UndoItem*item);
/* create an undo list */
UndoList *undo_list_new (void);
void undo_list_free (UndoList *list);

/* push a thing  */
UndoItem *undo_list_push (UndoList *list, 
                          UndoItem *item);

/* get current pos */
UndoItem *undo_list_get_current (UndoList *list);

/* set pos */
UndoItem *undo_list_set_current (UndoList *list);

/* create an undo info */
UndoInfo *undo_info_new (enum UndoBehavior behavior,
                         GFunc callback,
                         const gchar *desc);
void undo_info_free (UndoInfo *info);

/* register an undo info */
gboolean undo_info_register (UndoList *list,
                             UndoInfo *info);

/* 
   undo
   pos --
*/
UndoItem *undo_list_undo (UndoList *list);

/* 
   redo 
   pos ++
*/
UndoItem *undo_list_redo (UndoList *list);


#endif /* __UNDO_LIST_H__ */

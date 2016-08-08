/* this file is part of RatDB */
/**
 * Copyright (c) 2012 Mike Manilone
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS `AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
#ifndef __RATDB_FILE_H__
#define __RATDB_FILE_H__

#include "ratdb-macros.h"

/* made with imacros.sh */
#include <glib-object.h>

G_BEGIN_DECLS

#define RATDB_TYPE_FILE             (ratdb_file_get_type ())
#define RATDB_FILE(o)               (G_TYPE_CHECK_INSTANCE_CAST ((o), RATDB_TYPE_FILE, RatdbFile))
#define RATDB_IS_FILE(o)            (G_TYPE_CHECK_INSTANCE_TYPE ((o), RATDB_TYPE_FILE))
#define RATDB_GET_FILE_INTERFACE(o) (G_TYPE_INSTANCE_GET_CLASS ((o), RATDB_TYPE_FILE, RatdbFileInterface))

typedef struct _RatdbFile          RatdbFile;
typedef struct _RatdbFileInterface RatdbFileInterface;

struct _RatdbFileInterface
{
    GTypeInterface parent_interface;

    RATDB_DEFINE_VARIABLE_IFACE_ITEM (gint16, check_magic, RatdbFile);

    RATDB_DEFINE_VARIABLE_IFACE_ITEM (gboolean, new_database, RatdbFile);
    RATDB_DEFINE_VARIABLE_IFACE_ITEM (goffset, new_item, RatdbFile);
    RATDB_DEFINE_VARIABLE_IFACE_ITEM (goffset, new_table, RatdbFile);

    RATDB_DEFINE_VARIABLE_IFACE_ITEM (gpointer, delete_item, RatdbFile);
    RATDB_DEFINE_VARIABLE_IFACE_ITEM (gpointer, delete_table, RatdbFile);

    RATDB_DEFINE_VARIABLE_IFACE_ITEM (gpointer, list_tables, RatdbFile);
    RATDB_DEFINE_VARIABLE_IFACE_ITEM (gpointer, select_table, RatdbFile);

    goffset  (*jump_to)(RatdbFile *self, guint64 pos, gpointer user_data);
    guint64  (*pos)(RatdbFile *self, gpointer user_data);
    goffset  (*next)(RatdbFile *self, gpointer user_data);
    goffset  (*prev)(RatdbFile *self, gpointer user_data);
    goffset  (*rewind)(RatdbFile *self, gpointer user_data);

    gpointer (*item_info)(RatdbFile *self, gpointer user_data);
    gpointer (*table_info)(RatdbFile *self, gpointer user_data);

    gpointer __padding[500-15];
};

GType    ratdb_file_get_type            (void);

RATDB_DEFINE_VARIABLE_FUNC_IFACE (gint16, ratdb_file_check_magic, RatdbFile)
RATDB_DEFINE_VARIABLE_FUNC_IFACE (goffset, ratdb_file_new_item, RatdbFile)
RATDB_DEFINE_VARIABLE_FUNC_IFACE (goffset, ratdb_file_new_table, RatdbFile)
RATDB_DEFINE_VARIABLE_FUNC_IFACE (gboolean, ratdb_file_new_database, RatdbFile)
RATDB_DEFINE_VARIABLE_FUNC_IFACE (gpointer, ratdb_file_delete_item, RatdbFile)
RATDB_DEFINE_VARIABLE_FUNC_IFACE (gpointer, ratdb_file_delete_table, RatdbFile)
RATDB_DEFINE_VARIABLE_FUNC_IFACE (gpointer, ratdb_file_list_tables, RatdbFile)
RATDB_DEFINE_VARIABLE_FUNC_IFACE (gpointer, ratdb_file_select_table, RatdbFile)

goffset  ratdb_file_jump_to    (RatdbFile *self,
                                guint64 pos,
                                gpointer user_data);
guint64  ratdb_file_pos        (RatdbFile *self,
                                gpointer user_data);
goffset  ratdb_file_next       (RatdbFile *self,
                                gpointer user_data);
goffset  ratdb_file_prev       (RatdbFile *self,
                                gpointer user_data);
goffset  ratdb_file_rewind     (RatdbFile *self,
                                gpointer user_data);
gpointer ratdb_file_item_info  (RatdbFile *self,
                                gpointer user_data);
gpointer ratdb_file_table_info (RatdbFile *self,
                                gpointer user_data);

G_END_DECLS

#endif /* __RATDB_FILE_H__ */


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

/**
 * SECTION: RatdbFile
 * @Short_description: Abstracted File Interface
 * @Title: RatdbFile
 * @Include: ratdbfile.h
 * 
 * This is an abstracted file interface, which tell a file format class what 
 * need to do.
 */

#include <glib.h>

#include "ratdb-file.h"

G_DEFINE_INTERFACE (RatdbFile, ratdb_file, G_TYPE_INVALID)

static void
ratdb_file_default_init (RatdbFileInterface *iface)
{
}

RATDB_DEFINE_VARIABLE_FUNC (gint16, ratdb_file_check_magic, RatdbFile)
/**
 * ratdb_file_check_magic_valist:
 * @self: an object which implemented #RatdbFile
 * @first_para: the first argument
 * @args: the other arguments
 * 
 * Check whether your database's magic number is correct.
 * For full details please refer to the class you use.
 * 
 * Returns: undefined.
 */
gint16 
ratdb_file_check_magic_valist (RatdbFile *self,
                               gpointer first_para,
                               va_list args)
{
    return RATDB_GET_FILE_INTERFACE (self)->check_magic (self, first_para, args);
}

RATDB_DEFINE_VARIABLE_FUNC (goffset, ratdb_file_new_item, RatdbFile)
/**
 * ratdb_file_new_item_valist:
 * @self: an object which implemented #RatdbFile
 * @first_para: the first argument
 * @args: the other arguments
 * 
 * Create a new item.
 * 
 * Returns: the position of the newly created item.
 * or -1 if @file is not an #RatdbFile-implemented object.
 */
goffset
ratdb_file_new_item_valist (RatdbFile *self,
                            gpointer   first_para,
                            va_list args)
{
    return RATDB_GET_FILE_INTERFACE (self)->new_item (self, first_para, args);
}

RATDB_DEFINE_VARIABLE_FUNC (goffset, ratdb_file_new_table, RatdbFile)
/**
 * ratdb_file_new_table_valist:
 * @self: an object which implemented #RatdbFile.
 * @first_para: the first argument
 * @args: the other arguments
 * 
 * Create a new table.
 * 
 * Returns: the position of the newly created table.
 */
goffset
ratdb_file_new_table_valist (RatdbFile *self,
                             gpointer first_para,
                             va_list args)
{
    return RATDB_GET_FILE_INTERFACE (self)->new_table (self, first_para, args);
}

RATDB_DEFINE_VARIABLE_FUNC (gboolean, ratdb_file_new_database, RatdbFile)
/**
 * ratdb_file_new_database_valist:
 * @file: an object which implemented #RatdbFile
 * @first_para: the first argument
 * @args: the other arguments
 * 
 * Create a new database.
 * 
 * Returns: %TRUE on success.
 */
gboolean
ratdb_file_new_database_valist (RatdbFile *file,
                                gpointer first_para,
                                va_list args)
{
    return RATDB_GET_FILE_INTERFACE (file)->new_database (file, first_para, args);
}

RATDB_DEFINE_VARIABLE_FUNC (gpointer, ratdb_file_delete_item, RatdbFile)
/**
 * ratdb_file_delete_item_valist:
 * @file: an object which implemented #RatdbFile
 * @first_para: the first argument
 * @args: the other arguments
 *
 * Delete an item.
 * 
 * Returns: undefined.
 */
gpointer
ratdb_file_delete_item_valist (RatdbFile *file,
                               gpointer first_para,
                               va_list args)
{
    return RATDB_GET_FILE_INTERFACE (file)->delete_item (file, first_para, args);
}

RATDB_DEFINE_VARIABLE_FUNC (gpointer, ratdb_file_delete_table, RatdbFile)
/**
 * ratdb_file_delete_table_valist:
 * @file: an object which implemented #RatdbFile
 * @first_para: the first argument
 * @args: the other arguments
 * 
 * Delete a table.
 * 
 * Returns: undefined.
 */
gpointer
ratdb_file_delete_table_valist (RatdbFile *file,
                                gpointer first_para,
                                va_list args)
{
    return RATDB_GET_FILE_INTERFACE (file)->delete_table (file, first_para, args);
}

RATDB_DEFINE_VARIABLE_FUNC (gpointer, ratdb_file_list_tables, RatdbFile)
/**
 * ratdb_file_list_tables_valist:
 * @file: an object which implemented #RatdbFile
 * @first_para: the first argument
 * @args: the other arguments
 * 
 * List tables.
 * 
 * Returns: undefined. suggest GList.
 */
gpointer
ratdb_file_list_tables_valist (RatdbFile *file,
                                gpointer first_para,
                                va_list args)
{
    return RATDB_GET_FILE_INTERFACE (file)->list_tables (file, first_para, args);
}

RATDB_DEFINE_VARIABLE_FUNC (gpointer, ratdb_file_select_table, RatdbFile)
/**
 * ratdb_file_select_table_valist:
 * @file: an object which implemented #RatdbFile
 * @first_para: the first argument
 * @args: the other arguments
 * 
 * Switch to another table.
 * 
 * Returns: undefined.
 */
gpointer
ratdb_file_select_table_valist (RatdbFile *file,
                                gpointer first_para,
                                va_list args)
{
    return RATDB_GET_FILE_INTERFACE (file)->select_table (file, first_para, args);
}

/**
 * ratdb_file_jump_to:
 * @file: an object which implemented #RatdbFile
 * @user_data: extra data
 * 
 * Jump to the item of the number.
 * 
 * Returns: the real position of the item in file.
 */
goffset
ratdb_file_jump_to (RatdbFile *file,
                    guint64 pos,
                    gpointer user_data)
{
    return RATDB_GET_FILE_INTERFACE (file)->jump_to (file, pos, user_data);
}

/**
 * ratdb_file_pos:
 * @file: an object which implemented #RatdbFile
 * @user_data: extra data
 * 
 * Returns: where the current item is. It doesn't get the real position but an order.
 */
guint64
ratdb_file_pos (RatdbFile *file,
                gpointer user_data)
{
    return RATDB_GET_FILE_INTERFACE (file)->pos (file, user_data);
}

/**
 * ratdb_file_next:
 * @file: an object which implemented #RatdbFile
 * @user_data: extra data will be passed to the implementation.
 * 
 * Go to the next item of the table.
 * 
 * Returns: the position of the item.
 */
gssize 
ratdb_file_next (RatdbFile *file,
                 gpointer user_data)
{
    return RATDB_GET_FILE_INTERFACE (file)->next (file, user_data);
}

/**
 * ratdb_file_prev:
 * @file: an object which implemented #RatdbFile
 * @user_data: extra data will be passed to the implementation.
 * 
 * Go to the previous item of the table.
 * 
 * Returns: the position of the item.
 */
gssize 
ratdb_file_prev (RatdbFile *file,
                 gpointer user_data)
{
    return RATDB_GET_FILE_INTERFACE (file)->prev (file, user_data);
}

/**
 * ratdb_file_rewind:
 * @file: an object which implemented #RatdbFile
 * @user_data: extra data will be passed to the implementation.
 * 
 * Go back to the first item of the table.
 * 
 * Returns: the position of the item.
 */
gssize 
ratdb_file_rewind (RatdbFile *file,
                   gpointer user_data)
{
    return RATDB_GET_FILE_INTERFACE (file)->rewind (file, user_data);
}

/**
 * ratdb_file_item_info:
 * @file: an object which implemented #RatdbFile
 * @user_data: extra data will be passed to the implementation.
 * 
 * Get details of an item.
 * 
 * Returns: undefined.
 */
gpointer 
ratdb_file_item_info (RatdbFile *file,
                      gpointer user_data)
{
    return RATDB_GET_FILE_INTERFACE (file)->item_info (file, user_data);
}

/**
 * ratdb_file_table_info:
 * @file: an object which implemented #RatdbFile
 * @user_data: extra data will be passed to the implementation.
 * 
 * Get details of an table.
 * 
 * Returns: undefined.
 */
gpointer 
ratdb_file_table_info (RatdbFile *file,
                      gpointer user_data)
{
    return RATDB_GET_FILE_INTERFACE (file)->table_info (file, user_data);
}


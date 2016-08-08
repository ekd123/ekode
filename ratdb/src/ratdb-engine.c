/* this file is part of RatDB */
/**
 * Copyright (c) 2012, 2013 Mike Manilone
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
 * SECTION: RatdbEngine
 * @Short_description: Abstracted Engine Interface
 * @Title: RatdbEngine
 * @Include: ratdbengine.h
 * 
 * This is an abstracted engine interface, which tell an engine class what 
 * need to do.
 */
#include <glib.h>

#include "ratdb-macros.h"
#include "ratdb-engine.h"

G_DEFINE_INTERFACE (RatdbEngine, ratdb_engine, G_TYPE_INVALID)

static void
ratdb_engine_default_init (RatdbEngineInterface *iface)
{
}

/**
 * ratdb_engine_new_item:
 * @engine: an object which implemented #RatdbEngine
 * @name: name of the new item
 * @schema: schema
 * @values: default values for it
 * @user_data: extra data that will be passed to the implementation
 * 
 * Create a new item.
 * 
 * Returns: %TRUE on success.
 */
gboolean 
ratdb_engine_new_item (RatdbEngine *engine,
                       const gchar *name,
                       const gchar *schema,
                       const gchar *values,
                       gpointer     user_data)
{
    return RATDB_GET_ENGINE_INTERFACE (engine)->new_item (engine, name, schema, values, user_data);
}

/**
 * ratdb_engine_new_table:
 * @engine: an object which implemented #RatdbEngine
 * @name: name of the table
 * @user_data: extra data will be passed to the implementation
 * 
 * Create a new table.
 * 
 * Returns: %TRUE on success.
 */
gboolean 
ratdb_engine_new_table (RatdbEngine *engine,
                        const gchar *name,
                        gpointer     user_data)
{
    return RATDB_GET_ENGINE_INTERFACE (engine)->new_table (engine, name, user_data);
}

/**
 * ratdb_engine_new_database:
 * @engine: an object which implemented #RatdbEngine
 * @flags: how you create the new database object
 * @name: name of database
 * @path: path of database
 * @version: version of database
 * @user_data: extra data will be passed to the implementation
 * 
 * Create a new database object.
 * 
 * Returns: %TRUE on success.
 */
gboolean 
ratdb_engine_new_database (RatdbEngine     *engine,
                           RatdbNewDBFlags  flags,
                           const gchar     *name,
                           const gchar     *path,
                           const gchar     *version,
                           gpointer         user_data)
{
    return RATDB_GET_ENGINE_INTERFACE (engine)->new_database (engine, flags, name, path, version, user_data);
}

/**
 * ratdb_engine_delete_item:
 * @engine: an object which implemented #RatdbEngine
 * @condition: matching condition
 * @result: result
 * @user_data: extra data that will be passed to the implementation
 * 
 * Delete an item.
 * 
 * Returns: %TRUE on success.
 */
gboolean 
ratdb_engine_delete_item (RatdbEngine *engine,
                          const gchar *condition,
                          RatdbMatch  *result,
                          gpointer     user_data)
{
    return RATDB_GET_ENGINE_INTERFACE (engine)->delete_item (engine, condition, result, user_data);
}

/**
 * ratdb_engine_delete_table:
 * @engine: an object which implemented #RatdbEngine
 * @condition: matching condition
 * @result: result
 * @user_data: extra data that will be passed to the implementation
 * 
 * Delete a table.
 * 
 * Returns: %TRUE on success.
 */
gboolean 
ratdb_engine_delete_table (RatdbEngine *engine,
                           const gchar *condition,
                           RatdbMatch  *result,
                           gpointer     user_data)
{
    return RATDB_GET_ENGINE_INTERFACE (engine)->delete_table (engine, condition, result, user_data);
}

/**
 * ratdb_engine_list_tables:
 * @engine: an object which implemented #RatdbEngine
 * @condition: condition
 * @result: result
 * 
 * List tables.
 * 
 * Returns: %TRUE on success
 */
gboolean 
ratdb_engine_list_tables (RatdbEngine  *engine,
                          const gchar  *condition,
                          gchar       **result,
                          gpointer      user_data)
{
    return RATDB_GET_ENGINE_INTERFACE (engine)->list_tables (engine, condition, result, user_data);
}

/**
 * ratdb_engine_select_table:
 * @engine: an object which implemented #RatdbEngine
 * @condition: condition
 * @user_data: extra data that will be passed to the implementation
 * 
 * Switch to another table.
 * 
 * Returns: result
 */
RatdbMatch 
ratdb_engine_select_table (RatdbEngine *engine,
                           const gchar *condition,
                           gpointer     user_data)
{
    return RATDB_GET_ENGINE_INTERFACE (engine)->select_table (engine, condition, user_data);
}

/**
 * ratdb_engine_next:
 * @engine: an object which implemented #RatdbEngine
 * @user_data: extra data will be passed to the implementation.
 * 
 * Go to the next item.
 * 
 * Returns: %TRUE on success.
 */
gboolean 
ratdb_engine_next (RatdbEngine *engine,
                   gpointer user_data)
{
    return RATDB_GET_ENGINE_INTERFACE (engine)->next (engine, user_data);
}

/**
 * ratdb_engine_prev:
 * @engine: an object which implemented #RatdbEngine
 * @user_data: extra data will be passed to the implementation.
 * 
 * Go to the previous item.
 * 
 * Returns: %TRUE on success.
 */
gboolean 
ratdb_engine_prev (RatdbEngine *engine,
                   gpointer user_data)
{
    return RATDB_GET_ENGINE_INTERFACE (engine)->prev (engine, user_data);
}

/**
 * ratdb_engine_rewind:
 * @engine: an object which implemented #RatdbEngine
 * @user_data: extra data will be passed to the implementation.
 * 
 * Go back to the first item.
 * 
 * Returns: %TRUE on success.
 */
gboolean 
ratdb_engine_rewind (RatdbEngine *engine,
                     gpointer user_data)
{
    return RATDB_GET_ENGINE_INTERFACE (engine)->rewind (engine, user_data);
}

/**
 * ratdb_engine_extra_data:
 * @engine: an object which implemented #RatdbEngine
 * @user_data: extra data will be passed to the implementation.
 * 
 * Get the extra data of the database.
 * 
 * Returns: the result
 */
gchar *
ratdb_engine_extra_data (RatdbEngine *engine,
                         gpointer user_data)
{
    return RATDB_GET_ENGINE_INTERFACE (engine)->extra_data (engine, user_data);
}

/**
 * ratdb_engine_search:
 * @engine: an object which implemented #RatdbEngine
 * @condition: condition
 * @user_data: extra data that will be passed to the implementation
 * 
 * Look for something you need in the database.
 * 
 * Returns: result
 */
gchar *
ratdb_engine_search (RatdbEngine *engine,
                     const gchar *condition,
                     gpointer     user_data)
{
    return RATDB_GET_ENGINE_INTERFACE (engine)->search (engine, condition, user_data);
}


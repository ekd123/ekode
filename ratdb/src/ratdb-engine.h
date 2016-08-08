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

#ifndef __RATDB_ENGINE_H__
#define __RATDB_ENGINE_H__

#include "ratdb-macros.h"
#include "ratdb-enums.h"
#include <glib-object.h>

G_BEGIN_DECLS

#define RATDB_TYPE_ENGINE             (ratdb_engine_get_type ())
#define RATDB_ENGINE(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), RATDB_TYPE_ENGINE, RatdbEngine))
#define RATDB_IS_ENGINE(o)            (G_TYPE_CHECK_INSTANCE_TYPE ((o), RATDB_TYPE_ENGINE))
#define RATDB_GET_ENGINE_INTERFACE(o) (G_TYPE_INSTANCE_GET_CLASS ((o), RATDB_TYPE_ENGINE, RatdbEngineInterface))

typedef struct _RatdbEngine          RatdbEngine;
typedef struct _RatdbEngineInterface RatdbEngineInterface;

struct _RatdbEngineInterface
{
    GTypeInterface parent_interface;

    /* TODO: FIXME: need user ID checking */
    gboolean (*new_database)(RatdbEngine *engine, RatdbNewDBFlags flags, const gchar *name, const gchar *path, const gchar *version, gpointer user_data);
    gboolean (*new_table)(RatdbEngine *engine, const gchar *name, gpointer user_data);
    gboolean (*new_item)(RatdbEngine *engine, const gchar *name, const gchar *schema, const gchar *values, gpointer user_data);

    gboolean (*delete_item)(RatdbEngine *engine, const gchar *condition, RatdbMatch *result, gpointer user_data);
    gboolean (*delete_table)(RatdbEngine *engine, const gchar *condition, RatdbMatch *result, gpointer user_data);

    gboolean (*list_tables)(RatdbEngine *engine, const gchar *condition, gchar **result, gpointer user_data);
    RatdbMatch (*select_table)(RatdbEngine *engine, const gchar *condition, gpointer user_data);

    gboolean (*next)(RatdbEngine *engine, gpointer user_data);
    gboolean (*prev)(RatdbEngine *engine, gpointer user_data);
    gboolean (*rewind)(RatdbEngine *engine, gpointer user_data);
    gchar *(*extra_data)(RatdbEngine *engine, gpointer user_data);

    gchar *(*search)(RatdbEngine *engine, const gchar *condition, gpointer user_data);

    gpointer __padding[500-12];
};

GType      ratdb_engine_get_type     (void);
gboolean   ratdb_engine_new_database (RatdbEngine    *engine,
                                      RatdbNewDBFlags flags,
                                      const gchar    *name,
                                      const gchar    *path,
                                      const gchar    *version,
                                      gpointer        user_data);
gboolean   ratdb_engine_new_table    (RatdbEngine    *engine,
                                      const gchar    *name,
                                      gpointer        user_data);
gboolean   ratdb_engine_new_item     (RatdbEngine    *engine,
                                      const gchar    *name,
                                      const gchar    *schema,
                                      const gchar    *values,
                                      gpointer        user_data);

gboolean   ratdb_engine_delete_item  (RatdbEngine    *engine,
                                      const gchar    *condition,
                                      RatdbMatch     *result,
                                      gpointer        user_data);
gboolean   ratdb_engine_delete_table (RatdbEngine    *engine,
                                      const gchar    *condition,
                                      RatdbMatch     *result,
                                      gpointer        user_data);

gboolean   ratdb_engine_list_tables  (RatdbEngine    *engine,
                                      const gchar    *condition,
                                      gchar         **result,
                                      gpointer        user_data);
RatdbMatch ratdb_engine_select_table (RatdbEngine    *engine,
                                      const gchar    *condition,
                                      gpointer        user_data);

gboolean   ratdb_engine_next         (RatdbEngine    *engine,
                                      gpointer        user_data);
gboolean   ratdb_engine_prev         (RatdbEngine    *engine,
                                      gpointer        user_data);
gboolean   ratdb_engine_rewind       (RatdbEngine    *engine,
                                      gpointer        user_data);
gchar     *ratdb_engine_extra_data   (RatdbEngine    *engine,
                                      gpointer        user_data);
gchar     *ratdb_engine_search       (RatdbEngine    *engine,
                                      const gchar    *condition,
                                      gpointer        user_data);


G_END_DECLS

#endif /* __RATDB_ENGINE_H__ */

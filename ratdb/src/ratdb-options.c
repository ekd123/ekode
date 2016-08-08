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
 * SECTION: ratdboptions
 * @Title: RatdbOptions
 * @Short_description: Parse the Command Options
 * @See_also: #RatdbServer
 * 
 * A command options parser for RatDB. Only #RatdbServer uses it.
 *
 * Currently it contaions nothing.
 */

#include <glib.h>

#include "ratdb-options.h"

#define RATDB_GET_OPTIONS_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), RATDB_TYPE_OPTIONS, RatdbOptionsPrivate))
typedef struct _RatdbOptionsPrivate RatdbOptionsPrivate;
struct _RatdbOptionsPrivate
{
    GHashTable *table;
};

G_DEFINE_TYPE(RatdbOptions, ratdb_options, G_TYPE_OBJECT)

static void ratdb_options_dispose  ();
static void ratdb_options_finalize ();

static void
ratdb_options_init (RatdbOptions *options)
{
    RatdbOptionsPrivate *priv = RATDB_GET_OPTIONS_PRIVATE (options);

    priv->table = g_hash_table_new (g_direct_hash, g_direct_equal);
}

static void
ratdb_options_class_init (RatdbOptionsClass *klass)
{
    G_OBJECT_CLASS (klass)->dispose = ratdb_options_dispose;
    G_OBJECT_CLASS (klass)->finalize = ratdb_options_finalize;
}

static void
ratdb_options_dispose (GObject *object)
{
    RatdbOptionsPrivate *priv = RATDB_GET_OPTIONS_PRIVATE (object);

    if (priv->table)
    {
        g_hash_table_unref (priv->table);
        priv->table = NULL;
    }

    G_OBJECT_CLASS (ratdb_options_parent_class)->dispose (object);
}

static void
ratdb_options_finalize (GObject *object)
{
    G_OBJECT_CLASS (ratdb_options_parent_class)->finalize (object);
}

/**
 * ratdb_options_set:
 * @options: a #RatdbOptions.
 * @key: key
 * @value: the value to set.
 *
 * This functions save the key to the internal hash table.
 */
void
ratdb_options_set (RatdbOptions *options,
                   gpointer      key,
                   gpointer      value)
{
    RatdbOptionsPrivate *priv = RATDB_GET_OPTIONS_PRIVATE (options);

    g_hash_table_insert (priv->table, key, value);
}

/**
 * ratdb_options_get:
 * @options: a #RatdbOptions
 * 
 * This function gets the value of the key you give.
 * 
 * Return Value: the value of the key.
 */
gpointer
ratdb_options_get (RatdbOptions *options,
                   gconstpointer key)
{
    RatdbOptionsPrivate *priv = RATDB_GET_OPTIONS_PRIVATE (options);
    return g_hash_table_lookup (priv->table, key);
}


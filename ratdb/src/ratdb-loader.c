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
 * SECTION: RatdbLoader
 * @Title: RatdbLoader
 * @Short_description: Dynamically load and register backends
 * @Include: ratdb-loader.h
 * 
 * #RatdbLoader is a dynamic loader for loading and registering backends.
 * 
 * For some notes please see %RatdbFuncManager.
 */
#include <glib/gi18n.h>
#include <glib.h>
#include <gmodule.h>

#include "ratdb-file.h"
#include "ratdb-engine.h"
#include "ratdb-backend.h"
#include "ratdb-loader.h"

static void ratdb_loader_dispose (GObject *object);
static void ratdb_loader_finalize (GObject *object);

struct _RatdbLoaderPrivate
{
    GHashTable *loaded;
};

G_DEFINE_TYPE (RatdbLoader, ratdb_loader, G_TYPE_OBJECT)

static GObject *
ratdb_loader_constructor (GType type,
                          guint n_construct_params,
                          GObjectConstructParam *construct_params)
{
    static GObject *_main_instance = NULL;
    if (_main_instance != NULL)
        return _main_instance;
    _main_instance = G_OBJECT_CLASS (ratdb_loader_parent_class)->constructor (type, n_construct_params, construct_params);
    g_object_add_weak_pointer (_main_instance, (gpointer)&_main_instance);
    return _main_instance;
}

static void
ratdb_loader_class_init (RatdbLoaderClass *klass)
{
    GObjectClass *gobject_class = (GObjectClass *)klass;

    g_type_class_add_private (klass, sizeof (RatdbLoaderPrivate));

    gobject_class->dispose = ratdb_loader_dispose;
    gobject_class->finalize = ratdb_loader_finalize;
    gobject_class->constructor = ratdb_loader_constructor;
}

static void
ratdb_loader_init (RatdbLoader *self)
{
    self->priv = G_TYPE_INSTANCE_GET_PRIVATE (self,
                                              RATDB_TYPE_LOADER,
                                              RatdbLoaderPrivate);
    self->priv->loaded = g_hash_table_new_full (g_str_hash, g_str_equal,
                                                g_free, g_free);
}

static void
ratdb_loader_dispose (GObject *object)
{
    G_OBJECT_CLASS (ratdb_loader_parent_class)->dispose (object);
}

static void
ratdb_loader_finalize (GObject *object)
{
    RatdbLoader *self = (RatdbLoader *)object;

    if (self->priv->loaded)
    {
        g_hash_table_unref (self->priv->loaded);
        self->priv->loaded = NULL;
    }

    G_OBJECT_CLASS (ratdb_loader_parent_class)->finalize (object);
}

static gboolean 
_ratdb_loader_load_dir (RatdbLoader *loader,
                        const gchar *sym,
                        const gchar *dirname)
{
    GDir *dir = NULL;
    GError *error = NULL;
    const gchar *filename = NULL;

    g_debug ("Loading %s", dirname);

    dir = g_dir_open (dirname, 0, &error);
    if (!dir)
    {
        g_warning ("Error opening directory: %s", error->message);
        g_error_free (error);
        return FALSE;
    }

    while ((filename = g_dir_read_name (dir)) != NULL)
    {
        if (g_str_has_suffix (filename, G_MODULE_SUFFIX))
        {
            RatdbRegisterFunc func = NULL;
            GModule *module = NULL;
            char *path = g_strdup_printf ("%s/%s", dirname, filename);
            module = g_module_open (path, G_MODULE_BIND_LAZY);
            g_free (path);
            g_module_symbol (module, sym, (gpointer)&func);
            if (func)
            {
                gchar *module_name = RATDB_FIND_REAL_FILENAME(filename);
                gpointer value = g_malloc0 (sizeof(guint64));
                *(guint64*)value = (guint64)func;
                g_debug ("Module '%s' loaded with initializier at 0x%lX", module_name, *(guint64*)value);

                g_hash_table_insert (loader->priv->loaded, 
                                     module_name, 
                                     value);
            }
            else
            {
                g_critical (_("%s can't be loaded. Check whether the '%s' function works."), filename, sym);
                g_module_close (module);
            }
        }
    }
    g_dir_close (dir);
    return TRUE;
}

/**
 * ratdb_loader_new:
 * 
 * Get the default instance of #RatdbLoader
 * 
 * Returns: (transfer none): #RatdbLoader
 */
RatdbLoader *
ratdb_loader_new (void)
{
    return g_object_new (RATDB_TYPE_LOADER, NULL);
}

/**
 * ratdb_loader_load_engines:
 * 
 * Read the file list of RATDB_ENGINES_DIR, and call the 'register' of all
 * shared objects which the filenames are ended up with 'so'.
 * 
 * Returns: FALSE if any crtical errors.
 */
gboolean 
ratdb_loader_load_engines (RatdbLoader *loader)
{
    return _ratdb_loader_load_dir (loader, RATDB_REGISTER_SYMBOL, RATDB_ENGINES_DIR);
}

/**
 * ratdb_loader_load_engines:
 * 
 * Read the file list of RATDB_BACKENDS_DIR, and call the 'register' of all
 * shared objects which the filenames are ended up with 'so'.
 * 
 * Returns: FALSE if any crtical errors.
 */
gboolean 
ratdb_loader_load_filebackends (RatdbLoader *loader)
{
    return _ratdb_loader_load_dir (loader, RATDB_REGISTER_SYMBOL, RATDB_FILEBACKENDS_DIR);
}

/**
 * ratdb_loader_create_instance:
 * @type: type of engine
 * 
 * Create an instance of type %type.
 * 
 * Returns: (transfer full): the new object
 */
gpointer 
ratdb_loader_create_instance (const gchar *type)
{
    RatdbLoader *loader = ratdb_loader_new ();
    RatdbRegisterFunc* func = (RatdbRegisterFunc*)g_hash_table_lookup (loader->priv->loaded, type);
    if (!func)
    {
        g_debug ("No such type %s", type);
        return NULL;
    }

    g_debug ("Creating instance of type %s", type);

    return (*func)();
}


/**
 * Copyright 2012 Mike Manilone. All rights reserved.
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
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
#include <glib/gi18n.h>
#include <glib.h>
#include "hs-macros.h"
#include "hs-misc.h"
#include "hs-ext.h"
#include "hs-ext-manager.h"

/**
 * SECTION: hs-ext-manager
 * @Title: HsExtManager
 * @Short_description: The extension manager
 * @Include: hs-ext-manager.h
 * @See_also: hs-ext
 * 
 * #HsExtManager is the extension manager of HShell. It loads extensions
 * automatically.
 * 
 * All the extensions are located in datadir/hshell/extensions and 
 * XDG_DATA_HOME/hshell/extensions.
 * 
 * HShell doesn't use a normal configuration file, but an extension called 
 * "main". This extension will be used for configurations.
 */

struct _HsExtManagerPrivate
{
    GPtrArray *metadata;
};

static void hs_ext_manager_dispose  (GObject *object);
static void hs_ext_manager_finalize (GObject *object);
static void _metadata_free          (gpointer data);

G_DEFINE_TYPE (HsExtManager, hs_ext_manager, G_TYPE_OBJECT)
G_LOCK_DEFINE_STATIC (metadata);

static void
hs_ext_manager_class_init (HsExtManagerClass *klass)
{
    GObjectClass *gobject_class = (GObjectClass *)klass;

    g_type_class_add_private (gobject_class, sizeof (HsExtManagerPrivate));

    gobject_class->dispose = hs_ext_manager_dispose;
    gobject_class->finalize = hs_ext_manager_finalize;
}

static void
hs_ext_manager_init (HsExtManager *self)
{
    GString *tmp;
    const gchar *user_datadir;
    const gchar *const *sys_datadir;

    self->priv = G_TYPE_INSTANCE_GET_PRIVATE (self,
                                              HS_TYPE_EXT_MANAGER,
                                              HsExtManagerPrivate);

    g_debug (_("HShell Extension Manager is starting."));

    /* initialize metadata array */
    self->priv->metadata = g_ptr_array_new_with_free_func (_metadata_free);

    /* get directories */
    user_datadir = g_get_user_data_dir ();
    sys_datadir = g_get_system_data_dirs ();

    /* load system settings (default) */
    tmp = g_string_new ("");
    g_strv_iforeach (sys_datadir)
    {
        g_string_printf (tmp, "%s/hshell/extensions/main/", sys_datadir[i]);
        g_debug (_("ExtManager: Loading %s"), tmp->str);
        hs_ext_manager_load_ext (self, tmp->str);
    }
    g_string_free (tmp, TRUE);

    /* load user settings */
    tmp = g_string_new ("");
    g_string_append_printf (tmp, "%s/hshell/extensions/main/", user_datadir);
    g_debug (_("ExtManager: Loading %s"), tmp->str);
    hs_ext_manager_load_ext (self, tmp->str);
    g_string_free (tmp, TRUE);
}

static void
hs_ext_manager_dispose (GObject *object)
{
    HsExtManager *self = HS_EXT_MANAGER (object);

    if (self->priv->metadata)
    {
        g_ptr_array_unref (self->priv->metadata);
    }

    G_OBJECT_CLASS (hs_ext_manager_parent_class)->dispose (object);
}

static void
hs_ext_manager_finalize (GObject *object)
{
    g_signal_handlers_destroy (object);
    G_OBJECT_CLASS (hs_ext_manager_parent_class)->finalize (object);
}

static 
void _metadata_free (gpointer data)
{
    g_key_file_unref ((GKeyFile *)data);
}

HsExtManager *
hs_ext_manager_new (void)
{
    return g_object_new (HS_TYPE_EXT_MANAGER, NULL);
}

/**
 * hs_ext_manager_load_exts:
 * @manager: a #HsExtManager
 * 
 * Load the installed extensions, except extensions which are disabled
 * in the configuration.
 * 
 * If you want to load only one extension, you may need 
 * hs_ext_manager_load_ext().
 */
void 
hs_ext_manager_load_exts (HsExtManager *manager)
{
    HsExt *ext;
    GString *tmp;
    GDir *dir;
    GError *error;
    gchar *Cblocked_extensions, **be;
    const gchar *user_datadir, *diritem;
    const gchar *const *sys_datadir;

    ext = hs_ext_new ();
    Cblocked_extensions = hs_ext_global_get_str ("Blocked_Extensions");
    be = g_strsplit (Cblocked_extensions, ",", -1);
    g_free (Cblocked_extensions);

    user_datadir = g_get_user_data_dir ();
    sys_datadir = g_get_system_data_dirs ();

    /* auto-load sys exts */
    g_strv_iforeach (sys_datadir)
    {
        error = NULL;
        tmp = g_string_new ("");
        g_string_printf (tmp, "%s/hshell/extensions/", sys_datadir[i]);
        dir = g_dir_open (tmp->str, 0, &error);
        if (error)
        {
            g_debug (_("%s is not readable, skipped: %s"), tmp->str, error->message);
            g_error_free (error);
            goto lbl_load_user_ext;
        }
        while ((diritem = g_dir_read_name (dir)))
        {
            /* main is already loaded */
            if (g_strcmp0 ("main", tmp->str) || hs_misc_in_strv (be, diritem))
                continue;
            g_string_printf (tmp, "%s/hshell/extensions/%s/", sys_datadir[i], diritem);
            hs_ext_manager_load_ext (manager, tmp->str);
        }
        g_dir_close (dir);
        g_string_free (tmp, TRUE);
    }

  lbl_load_user_ext:
    /* auto-load user exts */
    tmp = g_string_new ("");
    g_string_printf (tmp, "%s/hshell/extensions/", user_datadir);
    error = NULL;
    dir = g_dir_open (tmp->str, 0, &error);
    if (error)
    {
        g_warning (_("%s is not readable: %s"), tmp->str, error->message);
        g_error_free (error);
    }
    if (!error)
    {
        while ((diritem = g_dir_read_name (dir)))
        {
            if (g_strcmp0 (diritem, "main") || hs_misc_in_strv (be, diritem))
                continue;
            g_string_printf (tmp, "%s/hshell/extensions/%s/", user_datadir, diritem);
            hs_ext_manager_load_ext (manager, tmp->str);
        }
        g_dir_close (dir);
    }
    g_string_free (tmp, TRUE);
    g_strfreev (be);
}

/**
 * hs_ext_manager_load_ext:
 * @manager: #HsExtManager
 * @path: path to the extension
 * 
 * Load the extension. Be sure your path is ended with '/'.
 * 
 * Returns: %TRUE on success
 */
gboolean 
hs_ext_manager_load_ext (HsExtManager *manager,
                         const gchar  *path)
{
    HsExt *ext;
    GKeyFile *metadata;
    gchar *mainfile;
    GString *tmp;
    GError *error;
    gboolean r;

    /* ensure that path is ended with '/' */
    g_return_val_if_fail (g_str_has_suffix (path, "/"), FALSE);

    /* load metadata (readonly) */
    metadata = g_key_file_new ();
    tmp = g_string_new (path);
    g_string_append_printf (tmp, "metadata.ini");
    r = g_key_file_load_from_file(metadata, tmp->str, G_KEY_FILE_NONE, NULL);
    g_string_free (tmp, TRUE);
    if (!r)
    {
        return FALSE;
    }
    G_LOCK (metadata);
    g_ptr_array_add (manager->priv->metadata, metadata);
    G_UNLOCK (metadata);

    /* load the lua file */
    error = NULL;
    mainfile = g_key_file_get_string (metadata,
                                      "Config",
                                      "Main",
                                      &error);
    if (!mainfile || error)
    {
        g_warning (_("Could not load the extension because %s"), error->message);
        g_error_free (error);
        return FALSE;
    }
    tmp = g_string_new (path);
    g_string_append (tmp, mainfile);
    ext = hs_ext_new ();
    hs_ext_run_file (tmp->str); /* finally it's running */
    g_string_free (tmp, TRUE);
    g_free (mainfile);

    return TRUE;
}

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
#include "hs-macros.h"
#include "hs-desktop-proc.h"

struct _HsDesktopProcPrivate
{
    /* stores GDesktopAppInfo */
    GList *data;
};

static void hs_desktop_proc_dispose (GObject *object);
static void hs_desktop_proc_finalize (GObject *object);

G_DEFINE_TYPE (HsDesktopProc, hs_desktop_proc, G_TYPE_OBJECT)
G_LOCK_DEFINE_STATIC (appinfo_data);

static void
hs_desktop_proc_class_init (HsDesktopProcClass *klass)
{
    GObjectClass *gobject_class = (GObjectClass *)klass;

    g_type_class_add_private (klass, sizeof (HsDesktopProcPrivate));

    gobject_class->dispose = hs_desktop_proc_dispose;
    gobject_class->finalize = hs_desktop_proc_finalize;

    /* initialize GDesktopAppInfo, and, our desktop
       environment's name is "HOME" */
    g_desktop_app_info_set_desktop_env ("HOME");
}

static void
hs_desktop_proc_init (HsDesktopProc *self)
{
    const gchar *const *sys_datadir;
    const gchar *user_datadir, *diritem;
    GString *tmp;
    GDir *dir;

    self->priv = G_TYPE_INSTANCE_GET_PRIVATE (self,
                                              HS_TYPE_DESKTOP_PROC,
                                              HsDesktopProcPrivate);

    sys_datadir = g_get_system_data_dirs ();
    user_datadir = g_get_user_data_dir ();

    /* load sys .desktops */
    g_strv_iforeach (sys_datadir)
    {
        tmp = g_string_new (sys_datadir[i]);
        g_string_append (tmp, "/applications/");
        dir = g_dir_open (tmp->str, 0, NULL);
        if (dir)
        {
            while ((diritem = g_dir_read_name (dir)))
            {
                GString *fname;
                GDesktopAppInfo *appinfo;
                fname = g_string_new (diritem);
                g_string_prepend (fname, tmp->str);
                if (g_str_has_suffix (diritem, ".desktop"))
                {
                    appinfo = g_desktop_app_info_new_from_filename (fname->str);
                    hs_desktop_proc_add_appinfo (self, appinfo);
                }
                g_string_free (fname, TRUE);
            }
            g_dir_close (dir);
        }
        g_string_free (tmp, TRUE);
    }

    /* load user .desktops */
    tmp = g_string_new (user_datadir);
    g_string_append (tmp, "/applications/");
    dir = g_dir_open (tmp->str, 0, NULL);
    if (dir)
    {
        while ((diritem = g_dir_read_name (dir)))
        {
            GString *fname;
            GDesktopAppInfo *appinfo;
            fname = g_string_new (diritem);
            g_string_prepend (fname, tmp->str);
            if (g_str_has_suffix (diritem, ".desktop"))
            {
                appinfo = g_desktop_app_info_new_from_filename (fname->str);
                hs_desktop_proc_add_appinfo (self, appinfo);
            }
            g_string_free (fname, TRUE);
        }
    }
    g_string_free (tmp, TRUE);
}

static void
hs_desktop_proc_dispose (GObject *object)
{
    G_OBJECT_CLASS (hs_desktop_proc_parent_class)->dispose (object);
}

static void
hs_desktop_proc_finalize (GObject *object)
{
    HsDesktopProc *self = (HsDesktopProc *)object;

    if (self->priv->data)
    {
        g_list_free_full (self->priv->data, g_object_unref);
        self->priv->data = NULL;
    }

    g_signal_handlers_destroy (object);
    G_OBJECT_CLASS (hs_desktop_proc_parent_class)->finalize (object);
}

/**
 * hs_desktop_proc_new:
 * 
 * Create a new #HsDesktopProc.
 *
 * Returns: a newly-created #HsDesktopProc.
 */
HsDesktopProc *
hs_desktop_proc_new (void)
{
    return g_object_new (HS_TYPE_DESKTOP_PROC, NULL);
}

/**
 * hs_desktop_proc_get_categories:
 * @appinfo: a #GDesktopAppInfo
 * 
 * A wrapper for g_destop_app_info_get_categories(). 
 * This function splits the string for you. But it won't
 * validate the value for you.
 * 
 * Actually it seems like a method of #GDesktopAppInfo.
 * 
 * Returns: (transfer full): a NULL-terminated array of strings.
 * Use g_strfreev() to free it if it's no longer used.
 */
gchar ** 
hs_desktop_proc_get_categories (GDesktopAppInfo *appinfo)
{
    const gchar *value;

    value = g_desktop_app_info_get_categories (appinfo);
    return g_strsplit (value, ";", -1);
}

/**
 * hs_desktop_proc_get_all:
 * @dp: a #HsDesktopProc
 * 
 * Get the list of all the .desktop files information.
 * 
 * Returns: (transfer none): the list
 */
GList *
hs_desktop_proc_get_all (HsDesktopProc *dp)
{
    return dp->priv->data;
}

/**
 * hs_desktop_proc_get_list:
 * @dp: a #HsDesktopProc
 * @list: a NULL-terminated array of strings,
 * which stores file names
 * 
 * Get a list of #GDesktopAppInfo, if any one of 
 * @list is in @dp.
 * 
 * Returns: a #GList
 */
GList * 
hs_desktop_proc_get_list (HsDesktopProc *dp,
                          gchar        **list)
{
    GList *retval;

    retval = NULL;
    g_list_iforeach (dp->priv->data)
    {
        g_strv_jforeach (list)
        {
            GDesktopAppInfo *info = G_DESKTOP_APP_INFO (g_list_nth_data (dp->priv->data, i));
            if (!g_strcmp0 (list[j], g_path_get_basename (g_desktop_app_info_get_filename (info))))
            {
                retval = g_list_append (retval, info);
            }
        }
    }

    return retval;
}

/**
 * hs_desktop_proc_add_appinfo:
 * @dp: a #HsDesktopProc
 * @appinfo: a #GDesktopAppInfo
 * 
 * Insert @appinfo into @dp.
 * 
 * This is rarely used outside the #HsDesktopProc itself.
 */ 
void 
hs_desktop_proc_add_appinfo (HsDesktopProc   *dp,
                             GDesktopAppInfo *appinfo)
{
    g_return_if_fail (G_IS_DESKTOP_APP_INFO (appinfo));

    G_LOCK (appinfo_data);
    dp->priv->data = g_list_append (dp->priv->data, appinfo);
    G_UNLOCK (appinfo_data);
}


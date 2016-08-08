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
#include <glib.h>
#include <glib/gi18n.h>
#include "hs-macros.h"
#include "hs-marshal.h"
#include "hs-message.h"
#include "hs-desktop-proc.h"
#include "hs-filter.h"
#include "hs-run.h"
#include "hs-app-chooser.h"

/**
 * SECTION: hs-app-chooser
 * @Title: HsAppChooser
 * @Short_description: The clickable application launcher widget
 * 
 * #HsAppChooser is a widget which shows icons of applications. You can 
 * click on an icon to run it.
 * 
 * #HsAppChooser also supports "filter". Current used filter names are 
 * "Category" and "NameInclude".
 * 
 * If there is a 'Category' filter, packages which are not in the category
 * will not be shown.
 * 
 * If there is a 'NameInclude' filter, packages, which names don't have the 
 * string described in the filter, will not be shown.
 */

static void        hs_app_chooser_dispose        (GObject          *object);
static void        hs_app_chooser_finalize       (GObject          *object);
static GdkPixbuf * _get_pixbuf_from_themed_icon  (GtkIconTheme     *icontheme,
                                                  GThemedIcon      *icon,
                                                  int               size);
static void        _signal_desktop_updated_handler
                                                 (HsAppChooser     *ac);
static void        _signal_filter_updated_hanlder (HsAppChooser     *ac);
static void        _signal_item_activated_handler (GtkIconView      *iconview,
                                                   GtkTreePath      *path,
                                                   gpointer          user_data);
static void        _hs_app_chooser_refresh_icons (HsAppChooser     *ac);
static gboolean    _check_category               (HsAppChooser     *ac,
                                                  GDesktopAppInfo  *info);
static gboolean    _check_name_include           (HsAppChooser     *ac,
                                                  GDesktopAppInfo  *info);
static void        _hs_app_chooser_monitor_worker (HsAppChooser     *ac);
static void        _monitor_changed_handler      (GFileMonitor     *monitor,
                                                  GFile            *file,
                                                  GFile            *other_file,
                                                  GFileMonitorEvent event_type,
                                                  gpointer          user_data);

struct _HsAppChooserPrivate
{
    HsDesktopProc *dp;
    HsFilter *filter;
    GtkListStore *store;
    GList *monitors;
};

enum 
{
    COL_ICON,
    COL_STR,
    COL_INFO,
    NUM_COLS
};

enum
{
    SIGNAL_0,
    SIGNAL_FILTER_UPDATED,
    SIGNAL_DESKTOP_UPDATED,
    SIGNAL_ACTED, 
    SIGNAL_LAST
};

static gint signals[SIGNAL_LAST] = {0};

G_LOCK_DEFINE_STATIC (monitors);
G_DEFINE_TYPE (HsAppChooser, hs_app_chooser, GTK_TYPE_ICON_VIEW)

static void
hs_app_chooser_class_init (HsAppChooserClass *klass)
{
    GObjectClass *gobject_class = (GObjectClass *)klass;

    g_type_class_add_private (klass, sizeof (HsAppChooserPrivate));

    gobject_class->dispose = hs_app_chooser_dispose;
    gobject_class->finalize = hs_app_chooser_finalize;

    /**
     * HsAppChooser::filter-updated:
     * 
     * Filter updated
     */
    signals[SIGNAL_FILTER_UPDATED] = g_signal_new ("filter-updated",
                                                   HS_TYPE_APP_CHOOSER, 
                                                   G_SIGNAL_ACTION,
                                                   G_STRUCT_OFFSET (HsAppChooserClass, filter_updated),
                                                   NULL, NULL,
                                                   hs_marshal_VOID__VOID,
                                                   G_TYPE_NONE, 0, NULL);

    /**
     * HsAppChooser::desktop-updated:
     * 
     * .desktop files updated
     */
    signals[SIGNAL_DESKTOP_UPDATED] = g_signal_new ("desktop-updated",
                                                    HS_TYPE_APP_CHOOSER, 
                                                    G_SIGNAL_ACTION,
                                                    G_STRUCT_OFFSET (HsAppChooserClass, desktop_updated),
                                                    NULL, NULL,
                                                    hs_marshal_VOID__VOID,
                                                    G_TYPE_NONE, 0, NULL);

    /**
     * HsAppChooser::acted:
     * 
     * Emitted when finished a job.
     */
    signals[SIGNAL_ACTED] = g_signal_new ("acted",
                                          HS_TYPE_APP_CHOOSER,
                                          G_SIGNAL_ACTION,
                                          G_STRUCT_OFFSET (HsAppChooserClass, acted),
                                          NULL, NULL,
                                          hs_marshal_VOID__VOID,
                                          G_TYPE_NONE, 0, NULL);
}

static void
hs_app_chooser_init (HsAppChooser *self)
{
    self->priv = G_TYPE_INSTANCE_GET_PRIVATE (self,
                                              HS_TYPE_APP_CHOOSER,
                                              HsAppChooserPrivate);

    /* desktop proc */
    self->priv->dp = NULL;

    /* init filter */
    self->priv->filter = hs_filter_new ();

    /* set model */
    self->priv->store = gtk_list_store_new (NUM_COLS, GDK_TYPE_PIXBUF, G_TYPE_STRING, G_TYPE_DESKTOP_APP_INFO);
    gtk_icon_view_set_model (GTK_ICON_VIEW (self), GTK_TREE_MODEL (self->priv->store));

    /* bind columns */
    gtk_icon_view_set_text_column (GTK_ICON_VIEW (self), COL_STR);
    gtk_icon_view_set_pixbuf_column (GTK_ICON_VIEW (self), COL_ICON);
    gtk_icon_view_set_selection_mode (GTK_ICON_VIEW (self),
                                      GTK_SELECTION_SINGLE);

    /* connect to signals */
    g_signal_connect (self, "item-activated", G_CALLBACK (_signal_item_activated_handler), NULL);
    g_signal_connect (self, "desktop-updated", G_CALLBACK (_signal_desktop_updated_handler), NULL);
    g_signal_connect (self, "filter-updated", G_CALLBACK (_signal_filter_updated_hanlder), NULL);

    /* update desktops */
    g_signal_emit (self, signals[SIGNAL_DESKTOP_UPDATED], 0, NULL);

    /* start monitoring */
    _hs_app_chooser_monitor_worker (self);
}

static void 
hs_app_chooser_dispose (GObject *object)
{
    G_OBJECT_CLASS (hs_app_chooser_parent_class)->dispose (object);
}

static void
hs_app_chooser_finalize (GObject *object)
{
    HsAppChooser *self = (HsAppChooser *)object;

    if (self->priv->filter)
    {
        hs_filter_unref (self->priv->filter);
        self->priv->filter = NULL;
    }
    if (self->priv->dp)
    {
        g_object_unref (self->priv->dp);
        self->priv->dp = NULL;
    }
    if (self->priv->store)
    {
        g_object_unref (self->priv->store);
        self->priv->store = NULL;
    }
    if (self->priv->monitors)
    {
        g_list_free_full (self->priv->monitors, g_object_unref);
        self->priv->monitors = NULL;
    }

    g_signal_handlers_destroy (object);
    G_OBJECT_CLASS (hs_app_chooser_parent_class)->finalize (object);
}

/**
   returns a GdkPixbuf from GThemedIcon (transfer full) 
*/
static GdkPixbuf *
_get_pixbuf_from_themed_icon (GtkIconTheme *icon_theme,
                              GThemedIcon *icon,
                              int size)
{
    char **icon_names;
    GtkIconInfo *info;
    GdkPixbuf *pixbuf;
    GError *error = NULL;

    if (!icon_theme || !icon || size <= 0)
        return NULL;

    g_object_get (icon, "names", &icon_names, NULL);
    if (!icon_names)
        return NULL;
    info = gtk_icon_theme_choose_icon (icon_theme, (const gchar **)icon_names, size, 0);
    pixbuf = gtk_icon_info_load_icon (info, &error);
    if (error)
    {
        g_warning (_("Could not load icon: %s\n"), error->message);
        g_error_free (error);
    }
    gtk_icon_info_free (info);
    g_strfreev (icon_names);

    return pixbuf;
}

/**
   append an icon into @store 
*/
static void 
_append_icon (GtkListStore          *store,
              const GdkPixbuf       *image,
              const gchar           *text,
              const GDesktopAppInfo *info)
{
    GtkTreeIter iter;
    gtk_list_store_append (store, &iter);
    gtk_list_store_set (store, &iter, COL_ICON, image, COL_STR, text, COL_INFO, info, -1);
}

/**
   refresh icons of @ac 
*/
static void 
_hs_app_chooser_refresh_icons (HsAppChooser *ac)
{
    gtk_list_store_clear (ac->priv->store);
    GList *dlist;
    dlist = hs_desktop_proc_get_all (ac->priv->dp);
    g_list_iforeach (dlist)
    {
        GDesktopAppInfo *info = (GDesktopAppInfo *)g_list_nth_data (dlist, i);
        if (!g_app_info_should_show (G_APP_INFO (info)))
            continue;
        if (!_check_category (ac, info)) /* filter: Category */
            continue;
        if (!_check_name_include (ac, info)) /* filter: NameInclude */
            continue;
        const gchar *text = g_app_info_get_display_name (G_APP_INFO (info));
        GIcon *icon = g_app_info_get_icon (G_APP_INFO (info));
        GdkPixbuf *pixbuf;
        /* load GdkPixbuf from GIcon */
        if (G_IS_THEMED_ICON (icon))
        {
            pixbuf = _get_pixbuf_from_themed_icon (gtk_icon_theme_get_default (), G_THEMED_ICON (icon), 64);
        }
        else if (G_IS_LOADABLE_ICON (icon))
        {
            GError *error = NULL;
            gchar *path = g_file_get_path (g_file_icon_get_file (G_FILE_ICON (icon)));
            pixbuf = gdk_pixbuf_new_from_file (path, &error);
            g_free (path);
            if (error)
            {
                g_warning (_("Could not load the icon: %s"), error->message);
                g_error_free (error);
            }
        }
        else 
        { 
            g_debug (_("Don't know what the icon is..."));
            pixbuf = NULL; 
        }
        /* the same size */
        if (pixbuf)
            gdk_pixbuf_scale_simple (pixbuf, 64, 64, GDK_INTERP_BILINEAR);
        _append_icon (ac->priv->store, pixbuf, text, info);
    }
}

static void 
_monitor_changed_handler (GFileMonitor     *monitor,
                          GFile            *file,
                          GFile            *other_file,
                          GFileMonitorEvent event_type,
                          gpointer          user_data)
{
    g_signal_emit (user_data, signals[SIGNAL_DESKTOP_UPDATED], 0);
}

/* @path without "/applications" */
static void 
_monitor_dir (HsAppChooser *ac, const gchar *path)
{
    GFileMonitor *m;
    GFile *f;
    gchar *t = g_strdup_printf("%s/applications", path);
    f = g_file_new_for_path (t);
    m = g_file_monitor_directory (f, G_FILE_MONITOR_NONE, NULL, NULL);
    g_signal_connect (m, "changed", G_CALLBACK (_monitor_changed_handler), ac);
    /* FIXME: Don't know if monitors will automatically be unref'd after the 
       files are destroyed */
    G_LOCK (monitors);
    ac->priv->monitors = g_list_append (ac->priv->monitors, f);
    G_UNLOCK (monitors);
}

static gpointer 
thr_worker (gpointer data)
{
    const gchar *const *sys_datadir = g_get_system_data_dirs ();
    const gchar *user_datadir = g_get_user_data_dir ();
    HsAppChooser *ac = (HsAppChooser *)data;
    GMainLoop *loop;
    _monitor_dir (ac, user_datadir);
    g_strv_iforeach (sys_datadir)
        _monitor_dir (ac, sys_datadir[i]);
    loop = g_main_loop_new (NULL, FALSE);
    g_main_loop_run (loop);
    return NULL;
}

static void 
_hs_app_chooser_monitor_worker (HsAppChooser *ac)
{

    void _monitor_dir (HsAppChooser *ac, const gchar *path);
    GThread *t;
    t = g_thread_new ("monitor_worker", thr_worker, ac);
}

/**
   when double-click on an icon, launch it with hs_run_do_run_async() 
*/
static void 
_signal_item_activated_handler (GtkIconView *iconview,
                                GtkTreePath *path,
                                gpointer     user_data)
{
    HsAppChooser *self = (HsAppChooser *)iconview;
    GtkTreeIter iter;
    GValue value = G_VALUE_INIT;
    GDesktopAppInfo *info;
    GError *error = NULL;
    if (!gtk_tree_model_get_iter (GTK_TREE_MODEL (self->priv->store), &iter, path))
    {
        hsm_warning (_("Something bad in the App Chooser?"), GTK_BUTTONS_OK, _("Sorry I could not know what are you doing..."));
        return;
    }
    gtk_tree_model_get_value (GTK_TREE_MODEL (self->priv->store), &iter, COL_INFO, &value);
    info = g_value_get_object (&value);
    g_value_unset (&value);
    hs_run_do_run_async (g_app_info_get_executable (G_APP_INFO (info)), &error);
    if (error)
    {
        hsm_warning (_("Could not run the program"), GTK_BUTTONS_OK, error->message);
        g_error_free (error);
        return;
    }
    g_signal_emit (self, signals[SIGNAL_ACTED], 0, NULL);
}

static void 
_signal_filter_updated_hanlder (HsAppChooser *ac)
{
    _hs_app_chooser_refresh_icons (ac);
}

static void 
_signal_desktop_updated_handler (HsAppChooser *ac)
{
    if (ac->priv->dp)
        g_object_unref (ac->priv->dp);
    ac->priv->dp = hs_desktop_proc_new ();

    _hs_app_chooser_refresh_icons (ac);
}

static gboolean 
_check_category (HsAppChooser    *ac,
                 GDesktopAppInfo *info)
{
    if (!hs_filter_has (ac->priv->filter, "Category"))
        return TRUE;
    gchar **values = hs_filter_get (ac->priv->filter, "Category");
    if (!values)
    {
        return FALSE;
    }
    gchar **categories = g_strsplit (g_desktop_app_info_get_categories (info), ";", -1);
    if (!categories)
    {
        g_strfreev (values);
        return FALSE;
    }
    g_strv_iforeach (values)
    {
        g_strv_jforeach (categories)
        {
            if (!g_strcmp0 (categories[j], values[i]))
                return TRUE;
        }
    }
    g_strfreev (values);
    g_strfreev (categories);
    return FALSE;
}

static gboolean 
_check_name_include (HsAppChooser    *ac,
                     GDesktopAppInfo *info)
{
    if (!hs_filter_has (ac->priv->filter, "NameInclude"))
        return TRUE;
    gchar **values = hs_filter_get (ac->priv->filter, "NameInclude");
    if (!values || !values[0])
        return TRUE;
    if (!strlen (values[0]))
        goto true;
    /* check for executable */
    if (strcasestr (g_app_info_get_executable (G_APP_INFO (info)), values[0]))
        goto true;
    /* check for display name */
    if (strcasestr (g_app_info_get_display_name (G_APP_INFO(info)), values[0]))
        goto true;
    goto false;
  true:
    g_strfreev (values);
    return TRUE;
  false:
    g_strfreev (values);
    return FALSE;
}

/**
 * hs_app_chooser_new:
 * 
 * Create a new #HsAppChooser
 * 
 * Returns: a newly-created #HsAppChooser
 */
GtkWidget *
hs_app_chooser_new (void)
{
    return (GtkWidget *)g_object_new (HS_TYPE_APP_CHOOSER, NULL);
}

/**
 * hs_app_chooser_add_filter_category:
 * @ac: a #HsAppChooser
 * @value: value will store in the default filter
 * 
 * Add a value to the "Category" filter.
 */
void
hs_app_chooser_add_filter_category (HsAppChooser *ac,
                                    const gchar *value)
{
    hs_filter_clear (ac->priv->filter, "Category");
    if (strlen (value))
        hs_filter_add (ac->priv->filter, "Category", value);
    g_signal_emit (ac, signals[SIGNAL_FILTER_UPDATED], 0, NULL);
}

/**
 * hs_app_chooser_add_filter_name_include:
 * ac: a #HsAppChooser
 * value: the value
 * 
 * Add @value to the "NameInclude" filter
 */
void 
hs_app_chooser_add_filter_name_include (HsAppChooser *ac,
                                        const gchar  *value)
{
    hs_filter_clear (ac->priv->filter, "NameInclude");
    if (strlen (value))
        hs_filter_add (ac->priv->filter, "NameInclude", value);
    g_signal_emit (ac, signals[SIGNAL_FILTER_UPDATED], 0, NULL);
}

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
#include "hs-macros.h"
#include "hs-marshal.h"
#include "hs-message.h"
#include "hs-app-chooser.h"
#include "hs-ext.h"
#include "hs-desktop-proc.h"
#include "hs-launcher-button.h"
#include "hs-dash.h"

struct _HsDashPrivate
{
    HsDesktopProc *dp;
    HsAppChooser *appchooser;
};

static void hs_dash_dispose      (GObject      *object);
static void hs_dash_finalize     (GObject      *object);
static void hs_dash_set_property (GObject      *object,
                                  guint         prop_id,
                                  const GValue *value,
                                  GParamSpec   *pspec);
static void hs_dash_get_property (GObject      *object,
                                  guint         prop_id,
                                  GValue       *value,
                                  GParamSpec   *pspec);

enum 
{
    PROP_0, 
    PROP_APPCHOOSER
};

enum
{
    SIGNAL_0,
    SIGNAL_ACTED,
    SIGNAL_LAST
};

static gint signals[SIGNAL_LAST] = { 0 };

G_DEFINE_TYPE (HsDash, hs_dash, GTK_TYPE_GRID)

static void 
hs_dash_class_init (HsDashClass *klass)
{
    GObjectClass *gobject_class = (GObjectClass *)klass;

    g_type_class_add_private (klass, sizeof (HsDashPrivate));

    gobject_class->dispose = hs_dash_dispose;
    gobject_class->finalize = hs_dash_finalize;
    gobject_class->get_property = hs_dash_get_property;
    gobject_class->set_property = hs_dash_set_property;

    /**
     * HsDash:appchooser:
     * 
     * the app chooser widget which to be used
     */
    g_object_class_install_property (gobject_class, PROP_APPCHOOSER,
                                     g_param_spec_object ("appchooser",
                                                          "App Chooser",
                                                          "The app chooser to be used",
                                                          HS_TYPE_APP_CHOOSER,
                                                          G_PARAM_READWRITE | G_PARAM_CONSTRUCT));

    /**
     * HsDash::acted:
     * 
     * Emitted when finished a job.
     */
    signals[SIGNAL_ACTED] = g_signal_new ("acted",
                                          HS_TYPE_DASH,
                                          G_SIGNAL_ACTION,
                                          G_STRUCT_OFFSET (HsDashClass, acted),
                                          NULL, NULL,
                                          hs_marshal_VOID__VOID,
                                          G_TYPE_NONE, 0, NULL);
}

static void 
hs_dash_init (HsDash *self)
{
    self->priv = G_TYPE_INSTANCE_GET_PRIVATE (self,
                                              HS_TYPE_DASH,
                                              HsDashPrivate);

    self->priv->dp = NULL;
    gtk_container_set_border_width (GTK_CONTAINER (self), 10);
    hs_dash_refresh_icons (self);
}

static void 
hs_dash_dispose (GObject *object)
{
    HsDash *self = (HsDash *)object;
    HS_FINAL_FREE_PRIVOBJ (appchooser);
    G_OBJECT_CLASS (hs_dash_parent_class)->dispose (object);
}

static void
hs_dash_finalize (GObject *object)
{
    g_signal_handlers_destroy (object);
    G_OBJECT_CLASS (hs_dash_parent_class)->finalize (object);
}

static void 
_signal_error_lauching_cb (HsLauncherButton *button,
                           const gchar *message)
{
    hsm_warning (_("Could not run the program"), GTK_BUTTONS_OK, message);
}

static void 
_signal_launched_cb (HsLauncherButton *btn, 
                     gpointer user_data)
{
    g_signal_emit (user_data, signals[SIGNAL_ACTED], 0);
}

static void 
hs_dash_set_property (GObject      *object,
                      guint         prop_id,
                      const GValue *value,
                      GParamSpec   *pspec)
{
    HsDash *self = (HsDash *)object;
    switch (prop_id)
    {
    case PROP_APPCHOOSER:
        if (self->priv->appchooser)
            g_object_unref (self->priv->appchooser);
        self->priv->appchooser = g_value_get_object (value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
        break;
    }
}

static void 
hs_dash_get_property (GObject      *object,
                      guint         prop_id,
                      GValue       *value,
                      GParamSpec   *pspec)
{
    HsDash *self = (HsDash *)object;
    switch (prop_id)
    {
    case PROP_APPCHOOSER:
        g_value_take_object (value, self->priv->appchooser);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
        break;
    }
}


/**
 * hs_dash_new:
 * @ac: which #HsAppChooser's data to be used
 * 
 * Create a new #HsDash.
 * 
 * Returns: a newly-created #HsDash.
 */
GtkWidget *
hs_dash_new (HsAppChooser *ac)
{
    return (GtkWidget *)g_object_new (HS_TYPE_DASH, "appchooser", ac, NULL);
}

/**
 * hs_dash_refresh_icons:
 * @dash: a #HsDash
 * 
 * Refresh the icons show in Dash.
 */
void 
hs_dash_refresh_icons (HsDash *dash)
{
    const GList *list;
    GList *grid_widgets;
    gchar *Cdash_favorites, **favlist;

    /* delete old widgets */
    grid_widgets = gtk_container_get_children (GTK_CONTAINER (dash));
    g_list_iforeach (grid_widgets)
    {
        GtkWidget *widget = g_list_nth_data (grid_widgets, i);

        gtk_container_remove (GTK_CONTAINER (dash), widget);
        gtk_widget_destroy (widget);
    }

    /* add new widgets */
    if (dash->priv->dp)
        g_object_unref (dash->priv->dp);
    dash->priv->dp = hs_desktop_proc_new ();
    Cdash_favorites = hs_ext_global_get_str ("Dash_Favorites");
    favlist = g_strsplit (Cdash_favorites, ",", -1);
    list = hs_desktop_proc_get_list (dash->priv->dp, favlist);
    g_strfreev (favlist);
    g_free (Cdash_favorites);
    g_list_iforeach (list)
    {
        GDesktopAppInfo *info;
        GtkWidget *image, *btn;
        info = g_list_nth_data ((GList *)list, i);
        image = gtk_image_new_from_gicon (g_app_info_get_icon (G_APP_INFO (info)), 
                                          GTK_ICON_SIZE_BUTTON);
        btn = hs_launcher_button_new_with_appinfo (info);
        gtk_widget_set_size_request (btn, 64, 64);
        gtk_widget_set_tooltip_text (btn, g_app_info_get_display_name (G_APP_INFO (info)));
        g_signal_connect (btn, "error-launching", 
                          G_CALLBACK (_signal_error_lauching_cb), NULL);
        g_signal_connect (btn, "launched",
                          G_CALLBACK (_signal_launched_cb), dash);
        gtk_container_add (GTK_CONTAINER (btn), image);
        gtk_grid_attach_next_to (GTK_GRID (dash), btn, NULL, GTK_POS_BOTTOM, 1, 1);
    }
}

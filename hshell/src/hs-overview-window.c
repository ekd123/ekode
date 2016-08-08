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
#include <time.h>
#include <glib/gi18n.h>
#include "hs-app-chooser.h"
#include "hs-dash.h"
#include "hs-overview-window.h"

/**
 * SECTION: hs-overview-window
 * @Title: HsOverviewWindow
 * @Short_description: The overview window
 * @Include: hs-overview-window.h
 * @See_also: hs-dash, hs-app-chooser
 * 
 * The overview mode. It features a Dash, an App Chooser and an App Viewer.
 */

struct _HsOverviewWindowPrivate
{
    GtkWidget *ac, *dash, *sw, *box, *vbox, *searchentry;
};

static void     hs_overview_window_dispose  (GObject     *object);
static void     hs_overview_window_finalize (GObject     *object);
static gboolean _signal_delete_callback     (GtkWidget   *widget,
                                             GdkEvent    *event,
                                             gpointer     user_data);
static void     _signal_show_callback       (GtkWidget   *widget,
                                             gpointer     user_data);
static void     _signal_search_changed_cb   (GtkEditable *editable,
                                             gpointer     user_data);

G_DEFINE_TYPE (HsOverviewWindow, hs_overview_window, GTK_TYPE_WINDOW)

static GObject *
hs_overview_window_constructor (GType type,
                                guint n_construct_params,
                                GObjectConstructParam *construct_params)
{
    static GObject *_main_instance = NULL;
    if (_main_instance != NULL)
        return _main_instance;
    _main_instance = G_OBJECT_CLASS (hs_overview_window_parent_class)->constructor (type, n_construct_params, construct_params);
    g_object_add_weak_pointer (_main_instance, (gpointer)&_main_instance);
    return _main_instance;
}

static void
hs_overview_window_class_init (HsOverviewWindowClass *klass)
{
    GObjectClass *gobject_class = (GObjectClass *)klass;

    g_type_class_add_private (klass, sizeof (HsOverviewWindowPrivate));

    gobject_class->constructor = hs_overview_window_constructor;
    gobject_class->dispose = hs_overview_window_dispose;
    gobject_class->finalize = hs_overview_window_finalize;
}

static void
hs_overview_window_init (HsOverviewWindow *self)
{
    GdkRGBA color;

    g_debug (_("HShell Overview Mode is staring."));

    self->priv = G_TYPE_INSTANCE_GET_PRIVATE (self,
                                              HS_TYPE_OVERVIEW_WINDOW,
                                              HsOverviewWindowPrivate);

    /* do not show in taskbar or pager */
    gtk_window_set_skip_taskbar_hint (GTK_WINDOW (self), TRUE);
    gtk_window_set_skip_pager_hint (GTK_WINDOW (self), TRUE);

    /* fullscreen */
    gtk_window_fullscreen (GTK_WINDOW (self));
    gtk_window_set_keep_above (GTK_WINDOW (self), TRUE);
    gtk_window_set_decorated (GTK_WINDOW (self), FALSE);
    gtk_window_set_has_resize_grip (GTK_WINDOW (self), FALSE);

    /* backgroud: gray */
    gdk_rgba_parse (&color, "rgb(192,192,192)");
    gtk_widget_override_background_color (GTK_WIDGET (self),
                                          GTK_STATE_FLAG_NORMAL,
                                          &color);

    /* add app chooser */
    self->priv->sw = gtk_scrolled_window_new (NULL, NULL);
    self->priv->ac = hs_app_chooser_new ();
    gtk_container_add (GTK_CONTAINER (self->priv->sw), self->priv->ac);

    /* add dash */
    self->priv->dash = hs_dash_new (HS_APP_CHOOSER (self->priv->ac));

    /* search entry */
    self->priv->searchentry = gtk_entry_new ();
    g_signal_connect (self->priv->searchentry, "changed", G_CALLBACK (_signal_search_changed_cb), self->priv->ac);

    /* layout */
    self->priv->box = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
    self->priv->vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add (GTK_CONTAINER (self), self->priv->box);
    gtk_box_pack_start (GTK_BOX (self->priv->box), self->priv->dash, FALSE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX (self->priv->box), self->priv->vbox, TRUE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX (self->priv->vbox), self->priv->searchentry, FALSE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX (self->priv->vbox), self->priv->sw, TRUE, TRUE, 0);

    /* hide if alt-f4 or job done */
    g_signal_connect (self, "delete-event", G_CALLBACK (_signal_delete_callback), self);
    g_signal_connect (self, "show", G_CALLBACK (_signal_show_callback), NULL);
    g_signal_connect (self->priv->ac, "acted", G_CALLBACK (_signal_delete_callback), NULL);
    g_signal_connect (self->priv->dash, "acted", G_CALLBACK (_signal_delete_callback), NULL);
}

static void
hs_overview_window_dispose (GObject *object)
{
    G_OBJECT_CLASS (hs_overview_window_parent_class)->dispose (object);
}

static void
hs_overview_window_finalize (GObject *object)
{
    g_signal_handlers_destroy (object);
    G_OBJECT_CLASS (hs_overview_window_parent_class)->finalize (object);
}

static void
_signal_search_changed_cb (GtkEditable *editable,
                           gpointer     user_data)
{
    const gchar *text = gtk_entry_get_text ((GtkEntry*)editable);
    hs_app_chooser_add_filter_name_include ((HsAppChooser*)user_data, text);
}

/**
    if alt-f4, don't delete, hide instead 
*/
static gboolean 
_signal_delete_callback (GtkWidget *widget,
                         GdkEvent  *event,
                         gpointer   user_data)
{
    g_debug (_("HShell Overview Mode is hiding."));
    hs_overview_window_hide ();

    return TRUE;
}

/**
   used for debugging
 */
static void
_signal_show_callback (GtkWidget *widget,
                       gpointer user_data)
{
    g_debug (_("HShell Overview Mode is showing."));
}


/**
 * hs_overview_window_new:
 * 
 * Get the default instance of #HsOverviewWindow
 *
 * Returns: the default instance of #HsOverviewWindow
 */
HsOverviewWindow *
hs_overview_window_new (void)
{
    return g_object_new (HS_TYPE_OVERVIEW_WINDOW, NULL);
}

/**
 * hs_overview_window_show:
 * 
 * Show the overview window.
 */
void
hs_overview_window_show (void)
{
    HsOverviewWindow *ow = hs_overview_window_new ();
    gtk_widget_show_all (GTK_WIDGET (ow));
}

/**
 * hs_overview_window_hide:
 * 
 * Hide the overview window.
 */
void
hs_overview_window_hide (void)
{
    HsOverviewWindow *ow = hs_overview_window_new ();
    gtk_widget_hide (GTK_WIDGET (ow));
}

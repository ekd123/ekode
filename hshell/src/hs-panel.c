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
#include "hs-x.h"
#include "hs-overview.h"
#include "hs-date.h"
#include "hs-user-button.h"
#include "hs-ext.h"
#include "hs-panel.h"
#include "eggtraymanager/eggtraymanager.h"

/**
 * SECTION: hs-panel
 * @Title: HsPanel
 * @Short_description: The top-level panel
 * @Include: hs-panel.h
 * 
 * #HsPanel is the panel of HShell.
 * 
 * By default, it adds "Overview", "Date", "User Menu" and 
 * "Notification Area" into itself.
 */

static void hs_panel_dispose       (GObject        *object);
static void hs_panel_finalize      (GObject        *object);
static void _hs_panel_add_internal (HsPanel        *panel,
                                    GtkWidget      *widget,
                                    GtkPositionType where);
static void _tray_added            (EggTrayManager *manager,
                                    GtkWidget      *icon,
                                    void           *data);

struct _HsPanelPrivate
{
    /* window rectangle */
    GdkRectangle rect;

    /* layout */
    GtkWidget *layout_grid;
    GtkWidget *layout_center_grid;

    /* widgets */
    GtkWidget *overview_button, *date, *userbutton;

    /* tray */
    EggTrayManager *traymanager;
    GtkWidget *traybox;
};
G_DEFINE_TYPE (HsPanel, hs_panel, GTK_TYPE_WINDOW)

static GObject *
hs_panel_constructor (GType type,
                      guint n_construct_params,
                      GObjectConstructParam *construct_params)
{
    static GObject *_main_instance = NULL;
    if (_main_instance != NULL)
        return _main_instance;
    _main_instance = G_OBJECT_CLASS (hs_panel_parent_class)->constructor (type, n_construct_params, construct_params);
    g_object_add_weak_pointer (_main_instance, (gpointer)&_main_instance);
    return _main_instance;
}

static void
hs_panel_class_init (HsPanelClass *klass)
{
    GObjectClass *gobject_class = (GObjectClass *)klass;

    g_type_class_add_private (klass, sizeof (HsPanelPrivate));

    gobject_class->constructor = hs_panel_constructor;
    gobject_class->dispose = hs_panel_dispose;
    gobject_class->finalize = hs_panel_finalize;
}

static void
hs_panel_init (HsPanel *self)
{
    GdkRectangle rect;
    HsExt *ext;
    gdouble Cscreen_reserve;

    g_debug (_("HShell Panel is starting."));

    self->priv = G_TYPE_INSTANCE_GET_PRIVATE (self,
                                              HS_TYPE_PANEL,
                                              HsPanelPrivate);

    /* get config */
    ext = hs_ext_new ();
    Cscreen_reserve = hs_ext_global_get_number ("Screen_Reserve");
    if (Cscreen_reserve == 0)
        g_error (_("Error loading configuration 'Screen_Reserve'"));

    /* no border */
    gtk_window_set_decorated (GTK_WINDOW (self), FALSE);

    /* no resize grip */
    gtk_window_set_has_resize_grip (GTK_WINDOW (self), FALSE);

    /* on all workspaces */
    gtk_window_stick (GTK_WINDOW (self));

    /* get geometry of the monitor */
    if (!hs_x_get_monitor_geometry (-1, &rect))
    {
        g_error (_("Could not get information of your monitor."));
    }

    self->priv->rect.x = 0;
    self->priv->rect.y = 0;
    self->priv->rect.width = rect.width;
    self->priv->rect.height = rect.height / Cscreen_reserve;

    /* be a panel (always on top & unresizable) */
    gtk_window_set_type_hint (GTK_WINDOW (self), GDK_WINDOW_TYPE_HINT_DOCK);
    gtk_window_set_keep_above (GTK_WINDOW (self), TRUE);

    /* do not show in taskbar and pager*/
    gtk_window_set_skip_taskbar_hint (GTK_WINDOW (self), TRUE);
    gtk_window_set_skip_pager_hint (GTK_WINDOW (self), TRUE);

    /* move */
    gtk_window_move (GTK_WINDOW (self), 0, 0);

    /* resize window */
    gtk_window_resize (GTK_WINDOW (self), self->priv->rect.width, self->priv->rect.height);

    /* do not delete by alt-f4 */
    g_signal_connect (self, "delete-event", G_CALLBACK (gtk_true), NULL);

    /* init layout */
    self->priv->layout_grid = gtk_grid_new ();
    self->priv->layout_center_grid = gtk_grid_new ();

    g_object_set (self->priv->layout_grid, "orientation", GTK_ORIENTATION_HORIZONTAL,
                  NULL);
    g_object_set (self->priv->layout_center_grid, "hexpand-set", TRUE, "hexpand",
                  TRUE, "orientation", GTK_ORIENTATION_HORIZONTAL, "halign",
                  GTK_ALIGN_CENTER, NULL);

    gtk_container_add (GTK_CONTAINER (self), self->priv->layout_grid);
    gtk_container_add (GTK_CONTAINER (self->priv->layout_grid),
                       self->priv->layout_center_grid);

    /* add overview button on left */
    self->priv->overview_button = hs_overview_new ();
    _hs_panel_add_internal (self, self->priv->overview_button, 
                            GTK_POS_LEFT);
    self->priv->date = hs_date_new ();
    _hs_panel_add_internal (self, self->priv->date, GTK_POS_TOP);
    self->priv->userbutton = hs_user_button_new ();
    _hs_panel_add_internal (self, self->priv->userbutton, GTK_POS_RIGHT);

    /* tray */
    self->priv->traymanager = egg_tray_manager_new ();
    self->priv->traybox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 1);
    gtk_container_set_border_width (GTK_CONTAINER (self->priv->traybox), 1);
    if (!egg_tray_manager_manage_screen (self->priv->traymanager, gtk_widget_get_screen (GTK_WIDGET (self))))
        g_warning (_("Can't manage the system tray"));
    _hs_panel_add_internal (self, GTK_WIDGET (self->priv->traybox), GTK_POS_RIGHT);
    g_signal_connect (self->priv->traymanager, "tray_icon_added", G_CALLBACK (_tray_added), self->priv->traybox);
}

static void
hs_panel_dispose (GObject *object)
{
    G_OBJECT_CLASS (hs_panel_parent_class)->dispose (object);
}

static void
hs_panel_finalize (GObject *object)
{
    HsPanel *self = (HsPanel*)object;
    if (self->priv->traymanager)
    {
        g_object_unref (self->priv->traymanager);
        self->priv->traymanager = NULL;
    }
    g_signal_handlers_destroy (object);
    G_OBJECT_CLASS (hs_panel_parent_class)->finalize (object);
}


static void
_tray_added (EggTrayManager *manager,
	     GtkWidget      *icon,
	     void           *data)
{
    GtkWidget *box = (GtkWidget*)data;
    gtk_box_pack_end (GTK_BOX (box), icon, FALSE, FALSE, 0);
    gtk_widget_show (icon);
}

/* 
   see hs_panel_add() 
*/
static void
_hs_panel_add_internal (HsPanel        *panel,
                        GtkWidget      *widget,
                        GtkPositionType where)
{
    g_object_set (widget, "vexpand", TRUE, "vexpand-set", TRUE, NULL);
    switch (where)
    {
    case GTK_POS_LEFT:
        g_object_set (widget, "halign", GTK_ALIGN_START, NULL);
        gtk_grid_insert_next_to (GTK_GRID (panel->priv->layout_grid),
                                 panel->priv->layout_center_grid, GTK_POS_LEFT);
        gtk_grid_attach_next_to (GTK_GRID (panel->priv->layout_grid), widget,
                                 panel->priv->layout_center_grid, GTK_POS_LEFT, 1, 1);
        break;
    /* BOTTOM may be changed later, use TOP */
    case GTK_POS_BOTTOM:
    case GTK_POS_TOP: 
        gtk_container_add (GTK_CONTAINER (panel->priv->layout_center_grid), widget);
        break;
    case GTK_POS_RIGHT:
        g_object_set (widget, "halign", GTK_ALIGN_END, NULL);
        gtk_grid_insert_next_to (GTK_GRID (panel->priv->layout_grid),
                                 panel->priv->layout_center_grid, GTK_POS_RIGHT);
        gtk_grid_attach_next_to (GTK_GRID (panel->priv->layout_grid), widget,
                                 panel->priv->layout_center_grid, GTK_POS_RIGHT, 1, 1);
        break;
    default:
        g_warning (_("You shouldn't reach here. You may find a new bug."));
        break;
    }
}

/**
 * hs_panel_new:
 * 
 * Create a new #HsPanel.
 * 
 * Returns: (transfer-full): a new #HsPanel.
 */
HsPanel *
hs_panel_new (void)
{
    return g_object_new (HS_TYPE_PANEL, NULL);
}

/**
 * hs_panel_show:
 * 
 * Let the panel show.
 */
void
hs_panel_show (void)
{
    gtk_widget_show_all (GTK_WIDGET (hs_panel_new ()));
}

/**
 * hs_panel_add:
 * @widget: a #GtkWidget
 * @where: where too add, can be HS_PANEL_ADD_LEFT, HS_PANEL_ADD_CENTER, HS_PANEL_RIGHT
 * 
 * Add %widget to panel's left, center or right.
 * This function is also available for extensions. [FIXME: TODO:]
 */
void
hs_panel_add (GtkWidget      *widget,
              GtkPositionType where)
{
    HsPanel *panel = hs_panel_new ();
    _hs_panel_add_internal (panel, widget, where);
}

/**
 * hs_panel_get_rectangle:
 * 
 * Get the rectangle of %panel.
 * 
 * Returns: (transfer-none) a #GdkRectangle which stores the information. Do NOT modify this.
 */
GdkRectangle 
hs_panel_get_rectangle (void)
{
    return (hs_panel_new ())->priv->rect;
}

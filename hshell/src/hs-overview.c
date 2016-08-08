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
#include "hs-overview-window.h"
#include "hs-overview.h"

/**
 * SECTION: hs-overview
 * @Title: HsOverview
 * @Short_description: The overview button
 * @Include: hs-overview.h
 * @See_also: hs-overview-window
 * 
 * This is just an "Overview Button" shows "Activities" like GNOME-Shell.
 * When you press, it'll show up the Overview Window.
 */

static void hs_overview_dispose      (GObject   *object);
static void hs_overview_finalize     (GObject   *object);
static void _signal_clicked_callback (GtkButton *button,
                                      gpointer   user_data);

G_DEFINE_TYPE (HsOverview, hs_overview, GTK_TYPE_BUTTON)

static void
hs_overview_class_init (HsOverviewClass *klass)
{
    GObjectClass *gobject_class = (GObjectClass *)klass;

    gobject_class->dispose = hs_overview_dispose;
    gobject_class->finalize = hs_overview_finalize;
}

static void
hs_overview_init (HsOverview *self)
{
    /* be non-3D by default */
    gtk_button_set_relief (GTK_BUTTON (self), GTK_RELIEF_NONE);

    /* FIXME: TODO: set image or configuration? */

    /* handle signals */
    g_signal_connect (self, "clicked", G_CALLBACK (_signal_clicked_callback), NULL);
}

static void
hs_overview_dispose (GObject *object)
{
    G_OBJECT_CLASS (hs_overview_parent_class)->dispose (object);
}

static void
hs_overview_finalize (GObject *object)
{
    g_signal_handlers_destroy (object);
    G_OBJECT_CLASS (hs_overview_parent_class)->finalize (object);
}

static void 
_signal_clicked_callback (GtkButton *button,
                          gpointer   user_data)
{
    hs_overview_window_show ();
}

GtkWidget *
hs_overview_new (void)
{
    return GTK_WIDGET (g_object_new (HS_TYPE_OVERVIEW, 
                                     "label", _("Activities"), NULL));
}


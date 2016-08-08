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
#include "hs-power.h"
#include "hs-user-button.h"

/**
 * SECTION: hs-user-button
 * @Title: HsUserButton
 * @Short_description: User's own button :-)
 * @Include: hs-user-button.h
 * 
 * This widget shows user's name and icon and a number of 
 * launchers users may be interested in.
 * 
 * Because GTK+ 3.6 is not released, here we just log out if you
 * press. Later we'll make it a #GMenuButton-based class.
 */

static void hs_user_button_dispose  (GObject   *object);
static void hs_user_button_finalize (GObject   *object);
static void _signal_clicked_hanlder (GtkButton *button,
                                     gpointer   user_data);

struct _HsUserButtonPrivate
{
    gpointer dummy;
};

G_DEFINE_TYPE (HsUserButton, hs_user_button, GTK_TYPE_BUTTON)

static void
hs_user_button_class_init (HsUserButtonClass *klass)
{
    GObjectClass *gobject_class = (GObjectClass *)klass;

    g_type_class_add_private (klass, sizeof (HsUserButtonPrivate));

    gobject_class->dispose = hs_user_button_dispose;
    gobject_class->finalize = hs_user_button_finalize;
}

static void
hs_user_button_init (HsUserButton *self)
{
    self->priv = G_TYPE_INSTANCE_GET_PRIVATE (self,
                                              HS_TYPE_USER_BUTTON,
                                              HsUserButtonPrivate);
    gtk_button_set_relief (GTK_BUTTON (self), GTK_RELIEF_NONE);

    /* FIXME: This behavior will be changed later */
    gtk_widget_set_tooltip_text (GTK_WIDGET (self), _("Click to log out"));
    g_signal_connect (self, "clicked", G_CALLBACK (_signal_clicked_hanlder), NULL);
}

static void
hs_user_button_dispose (GObject *object)
{
    G_OBJECT_CLASS (hs_user_button_parent_class)->dispose (object);
}

static void
hs_user_button_finalize (GObject *object)
{
    g_signal_handlers_destroy (object);
    G_OBJECT_CLASS (hs_user_button_parent_class)->finalize (object);
}

static void 
_signal_clicked_hanlder (GtkButton *button,
                         gpointer   user_data)
{
    hs_power_logout ();
}

GtkWidget *
hs_user_button_new (void)
{
    return GTK_WIDGET (g_object_new (HS_TYPE_USER_BUTTON, "label", g_get_real_name (), NULL));
}


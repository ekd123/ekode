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
#include "hs-run.h"

static void hs_run_dispose        (GObject   *object);
static void hs_run_finalize       (GObject   *object);
static void _hs_run_click_handler (GtkButton *button,
                                   gpointer   user_data);
static void _signal_show_cb       (GtkWidget *widget,
                                   gpointer   user_data);
static void _signal_delete_cb     (GtkWidget *widget,
                                   gpointer   user_data);

struct _HsRunPrivate
{
    GtkWidget *grid, *label, *input, *btnrun;
};

G_DEFINE_TYPE (HsRun, hs_run, GTK_TYPE_DIALOG)

static void
hs_run_class_init (HsRunClass *klass)
{
    GObjectClass *gobject_class = (GObjectClass *)klass;

    g_type_class_add_private (klass, sizeof (HsRunPrivate));

    gobject_class->dispose = hs_run_dispose;
    gobject_class->finalize = hs_run_finalize;
}

static void 
hs_run_init (HsRun *self)
{
    g_debug ("HShell 'Run' dialog is starting.");

    self->priv = G_TYPE_INSTANCE_GET_PRIVATE (self,
                                              HS_TYPE_RUN,
                                              HsRunPrivate);

    /* create the layout */
    self->priv->grid = gtk_grid_new ();
    self->priv->label = gtk_label_new (_("Write the command you wish to run here:"));
    self->priv->input = gtk_entry_new ();
    self->priv->btnrun = gtk_button_new_with_mnemonic (_("_Run"));

    gtk_grid_attach_next_to (GTK_GRID (self->priv->grid), self->priv->label, NULL, GTK_POS_BOTTOM, 1, 1);
    gtk_grid_attach_next_to (GTK_GRID (self->priv->grid), self->priv->input, NULL, GTK_POS_BOTTOM, 1, 1);
    gtk_box_pack_start (GTK_BOX (gtk_dialog_get_action_area (GTK_DIALOG (self))), self->priv->btnrun, TRUE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX (gtk_dialog_get_content_area (GTK_DIALOG (self))), self->priv->grid, TRUE, TRUE, 0);

    gtk_container_set_border_width (GTK_CONTAINER (self), 10);

    /* connect to the signals */
    g_signal_connect (self, "delete-event", G_CALLBACK (_signal_delete_cb), NULL);
    g_signal_connect (self, "show", G_CALLBACK (_signal_show_cb), NULL);
    g_signal_connect (self->priv->btnrun, "clicked", G_CALLBACK (_hs_run_click_handler), self->priv->input);
}

static void
hs_run_dispose (GObject *object)
{
    G_OBJECT_CLASS (hs_run_parent_class)->dispose (object);
}

static void
hs_run_finalize (GObject *object)
{
    HsRun *self = (HsRun *)object;

    gtk_widget_destroy (self->priv->grid);
    self->priv->grid = NULL;
    gtk_widget_destroy (self->priv->input);
    self->priv->input = NULL;
    gtk_widget_destroy (self->priv->label);
    self->priv->label = NULL;
    gtk_widget_destroy (self->priv->btnrun);
    self->priv->btnrun = NULL;

    g_signal_handlers_destroy (object);
    G_OBJECT_CLASS (hs_run_parent_class)->finalize (object);
}

static void 
_signal_show_cb (GtkWidget *widget,
                 gpointer user_data)
{
    g_debug (_("'Run' is showing."));
}

static void 
_signal_delete_cb (GtkWidget *widget,
                   gpointer user_data)
{
    g_debug (_("'Run' is hiding"));
    gtk_widget_hide (widget);
}

static void 
_hs_run_click_handler (GtkButton *button,
                       gpointer user_data)
{
    GError *error = NULL;

    if (!hs_run_do_run_async (gtk_entry_get_text (GTK_ENTRY (user_data)), &error))
    {
        /* FIXME: replace this with HsMessageDialog */
        g_message ("can't run");
    }
}

/**
 * hs_run_new:
 * 
 * Initialize the 'Run' dialog.
 * 
 * Returns: %NULL if failed
 */
HsRun *
hs_run_new (void)
{
    return g_object_new (HS_TYPE_RUN, NULL);
}

/**
 * hs_run_do_run_async:
 * @commandline: the command line you need to run.
 * @error: #GError for error reporting.
 * 
 * Run a command line asynchronous.
 * 
 * Returns: %TRUE on success
 */
gboolean
hs_run_do_run_async (const gchar *commandline,
                     GError     **error)
{
    if (!g_spawn_command_line_async (commandline, error))
    {
        return FALSE;
    }
    return TRUE;
}

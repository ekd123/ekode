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
#include <gio/gio.h>
#include <glib/gi18n.h>
#include <gtk/gtk.h>
#include "hs-ext.h"
#include "hs-ext-manager.h"
#include "hs-run.h"
#include "hs-panel.h"
#include "hs-x.h"
#include "hs-app.h"

static void     hs_app_dispose    (GObject      *object);
static void     hs_app_finalize   (GObject      *object);
static void     _activate_handler (GApplication *app,
                                   gpointer      user_data);
static gboolean _command_line     (GApplication *app,
                                   gchar      ***arguments,
                                   gint         *exit_status);
static void     _size_allocated   (GtkWidget    *widget,
                                   GdkRectangle *allocation,
                                   gpointer      user_data);

G_DEFINE_TYPE (HsApp, hs_app, GTK_TYPE_APPLICATION)

static GObject *
hs_app_constructor (GType type,
                    guint n_construct_params,
                    GObjectConstructParam *construct_params)
{
    static GObject *_main_instance = NULL;
    if (_main_instance != NULL)
        return _main_instance;
    _main_instance = G_OBJECT_CLASS (hs_app_parent_class)->constructor (type, n_construct_params, construct_params);
    g_object_add_weak_pointer (_main_instance, (gpointer)&_main_instance);
    return _main_instance;
}

static void
hs_app_class_init (HsAppClass *klass)
{
    GObjectClass *gobject_class = (GObjectClass *)klass;
    GApplicationClass *app_class = (GApplicationClass *)klass;

    gobject_class->dispose = hs_app_dispose;
    gobject_class->finalize = hs_app_finalize;
    gobject_class->constructor = hs_app_constructor;

    app_class->local_command_line = _command_line;
}

static void
hs_app_init (HsApp *self)
{
    g_debug (_("HShell is starting."));

#ifdef DEBUG
    g_print (_("You are using the debug version. You may also want to run"
               "hshell by 'G_MESSAGES_DEBUG=all hshell'"));
#endif

    g_signal_connect (self, "activate", G_CALLBACK (_activate_handler), NULL);
}

static void
hs_app_dispose (GObject *object)
{
    G_OBJECT_CLASS (hs_app_parent_class)->dispose (object);
}

static void
hs_app_finalize (GObject *object)
{
    g_signal_handlers_destroy (object);
    G_OBJECT_CLASS (hs_app_parent_class)->finalize (object);
}

static void 
_size_allocated (GtkWidget    *widget,
                 GdkRectangle *allocation,
                 gpointer      user_data)
{
    hs_x_set_window_strut (GTK_WINDOW (widget));
}

/**
   signal activate
 */
static void
_activate_handler (GApplication *app,
                   gpointer user_data)
{
    GtkWidget * dialog;
    dialog = gtk_message_dialog_new (GTK_WINDOW (hs_panel_new ()), 
                                     GTK_DIALOG_DESTROY_WITH_PARENT,
                                     GTK_MESSAGE_WARNING,
                                     GTK_BUTTONS_OK,
                                     _("Another instance is trying to start!"));
    gtk_message_dialog_format_secondary_text (GTK_MESSAGE_DIALOG (dialog), 
                                              _("Another instance is trying "
                                                "to start. \n This action "
                                                "has been rejected."
                                                "If you really want"
                                                " to start a new instance,"
                                                "run 'hshell --replace'. \n"
                                                "Note that this option would "
                                                "replace the current working"
                                                "instance.\n"));
    gtk_widget_show_all (dialog);
}

/**
   v-func local_command_line
 */
static gboolean
_command_line (GApplication *app,
               gchar      ***arguments,
               gint         *exit_status)
{
    HsExt *ext;
    HsExtManager *extmanager;
    HsPanel *panel;

    /* Initialize extension system */
    ext = hs_ext_new ();
    if (ext == NULL)
        g_error (_("Stop."));

    /* Initialize the extension manager */
    extmanager = hs_ext_manager_new ();

    /* Initialize the panel */
    panel = hs_panel_new ();
    hs_panel_show ();
    g_signal_connect (panel, "size-allocate", G_CALLBACK (_size_allocated), NULL);

    /* Now load extensions */
    hs_ext_manager_load_exts (extmanager);

    gtk_main ();

    *exit_status = 0;
    return TRUE;
}

/**
 * hs_app_new:
 * 
 * Get the default instance of #HsApp.
 * 
 * Returns: (transfer-none): the default instance of #HsApp.
 */
HsApp *
hs_app_new (void)
{
    return g_object_new (HS_TYPE_APP, 
                         "application-id", HS_APP_APPID,
                         "flags", G_APPLICATION_HANDLES_COMMAND_LINE 
                         | G_APPLICATION_SEND_ENVIRONMENT, 
                         NULL);
}


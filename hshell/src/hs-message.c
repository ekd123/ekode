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
#include "hs-message.h"

/**
 * SECTION: hs-message
 * @Title: HsMessage
 * @Short_description: Made message dialogs easier
 * @Include: hs-message.h
 * 
 * Some utilities for easier message dialogs.
 * 
 * These functions has 'hsm' prefix for easier inputting.
 */

gint 
hsm_warning (const gchar *text,
             GtkButtonsType buttons,
             const gchar *secondary_text)
{
    gint retval;
    GtkWidget *d;
    g_debug (_("HSM: Warning recieved: %s: %s"), text, secondary_text);
    d = gtk_message_dialog_new (NULL, GTK_DIALOG_DESTROY_WITH_PARENT, 
                                GTK_MESSAGE_WARNING, buttons, "%s", text);
    gtk_message_dialog_format_secondary_text (GTK_MESSAGE_DIALOG (d), "%s", secondary_text);
    gtk_window_set_title (GTK_WINDOW (d), _("Warning!"));
    retval = gtk_dialog_run (GTK_DIALOG (d));
    gtk_widget_destroy (d);
    return retval;
}

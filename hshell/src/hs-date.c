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
#include <stdlib.h>
#include "hs-macros.h"
#include "hs-ext.h"
#include "hs-date.h"

static void     hs_date_dispose   (GObject *object);
static void     hs_date_finalize  (GObject *object);
static gboolean _hs_date_set_date (HsDate  *dlabel);

struct _HsDatePrivate
{
    gchar *format;
};

G_DEFINE_TYPE (HsDate, hs_date, GTK_TYPE_LABEL)

static void
hs_date_class_init (HsDateClass *klass)
{
    GObjectClass *gobject_class = (GObjectClass *)klass;

    g_type_class_add_private (klass, sizeof (HsDate));

    gobject_class->dispose = hs_date_dispose;
    gobject_class->finalize = hs_date_finalize;
}

static void
hs_date_init (HsDate *self)
{
    self->priv = G_TYPE_INSTANCE_GET_PRIVATE (self,
                                              HS_TYPE_DATE,
                                              HsDatePrivate);
    self->priv->format = hs_ext_global_get_str ("Date_Format");

    /* refresh the date every 995ms, give some time to the OS :-) */
    _hs_date_set_date (self);
    g_timeout_add (995, (GSourceFunc)_hs_date_set_date, self);
}

static void
hs_date_dispose (GObject *object)
{
    G_OBJECT_CLASS (hs_date_parent_class)->dispose (object);
}

static void
hs_date_finalize (GObject *object)
{
    HsDate *self = (HsDate *)object;

    HS_FINAL_FREE_PRIVVAR (format);

    g_signal_handlers_destroy (object);
    G_OBJECT_CLASS (hs_date_parent_class)->finalize (object);
}

static gboolean 
_hs_date_set_date (HsDate  *dlabel)
{
    gchar *str;
    GDateTime *dt = g_date_time_new_now_local ();
    str = g_date_time_format (dt, dlabel->priv->format);
    gtk_label_set_text (GTK_LABEL (dlabel), str);
    g_free (str);
    return TRUE;
}

GtkWidget *
hs_date_new (void)
{
    return GTK_WIDGET (g_object_new (HS_TYPE_DATE, NULL));
}

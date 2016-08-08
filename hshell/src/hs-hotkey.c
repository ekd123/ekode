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
#include <glib.h>
#include <gtkhotkey.h>
#include "hs-macros.h"
#include "hs-hotkey.h"

#define HS_HOTKEY_ERROR (hs_hotkey_error_quark())
static void   hs_hotkey_dispose     (GObject *object);
static void   hs_hotkey_finalize    (GObject *object);
static GQuark hs_hotkey_error_quark (void);

struct _HsHotkeyPrivate
{
    GList *hotkey_list;
};

G_DEFINE_TYPE (HsHotkey, hs_hotkey, G_TYPE_OBJECT)
G_LOCK_DEFINE_STATIC (hotkey_list);

static GObject *
hs_hotkey_constructor (GType type,
                       guint n_construct_params,
                       GObjectConstructParam *construct_params)
{
    static GObject *_main_instance = NULL;
    if (_main_instance != NULL)
        return _main_instance;
    _main_instance = G_OBJECT_CLASS (hs_hotkey_parent_class)->constructor (type, n_construct_params, construct_params);
    g_object_add_weak_pointer (_main_instance, (gpointer)&_main_instance);
    return _main_instance;
}

static void
hs_hotkey_class_init (HsHotkeyClass *klass)
{
    GObjectClass *gobject_class = (GObjectClass *)klass;

    g_type_class_add_private (klass, sizeof (HsHotkeyPrivate));

    gobject_class->constructor = hs_hotkey_constructor;
    gobject_class->dispose = hs_hotkey_dispose;
    gobject_class->finalize = hs_hotkey_finalize;
}

static void
hs_hotkey_init (HsHotkey *self)
{
    self->priv = G_TYPE_INSTANCE_GET_PRIVATE (self,
                                              HS_TYPE_HOTKEY,
                                              HsHotkeyPrivate);
    self->priv->hotkey_list = NULL;
}

static void
hs_hotkey_dispose (GObject *object)
{
    G_OBJECT_CLASS (hs_hotkey_parent_class)->dispose (object);
}

static void
hs_hotkey_finalize (GObject *object)
{
    HsHotkey *self = (HsHotkey *)object;

    if (self->priv->hotkey_list)
    {
        g_list_free_full (self->priv->hotkey_list, g_object_unref);
        self->priv->hotkey_list = NULL;
    }

    g_signal_handlers_destroy (object);
    G_OBJECT_CLASS (hs_hotkey_parent_class)->finalize (object);
}

static GQuark 
hs_hotkey_error_quark (void)
{
    return g_quark_from_static_string ("hs-hotkey-error-quark");
}

HsHotkey *
hs_hotkey_new (void)
{
    return g_object_new (HS_TYPE_HOTKEY, NULL);
}

gboolean 
hs_hotkey_bind (const gchar *key_id,
                const gchar *key,
                GError     **error)
{
    HsHotkey *hk = hs_hotkey_new();
    GtkHotkeyInfo *keyinfo;

    keyinfo = gtk_hotkey_info_new (g_get_application_name (),
                                   key_id,
                                   key,
                                   NULL);
    if (!keyinfo)
    {
        g_set_error (error, HS_HOTKEY_ERROR, HS_HOTKEY_ERROR_OBJECT_FAILED,
                     _("Failed to create the backend object."));
        return FALSE;
    }
    if (gtk_hotkey_info_bind (keyinfo, error))
    {
        G_LOCK (hotkey_list);
        hk->priv->hotkey_list = g_list_append (hk->priv->hotkey_list, keyinfo);
        G_UNLOCK (hotkey_list);
        return TRUE;
    }
    return FALSE;
}

gboolean 
hs_hotkey_unbind (const gchar *key_id,
                  GError     **error)
{
    HsHotkey *hk = hs_hotkey_new ();
    g_list_iforeach (hk->priv->hotkey_list)
    {
        GtkHotkeyInfo *info = GTK_HOTKEY_INFO (g_list_nth_data (hk->priv->hotkey_list, i));
        if (!g_strcmp0 (gtk_hotkey_info_get_key_id (info), key_id))
        {
            if (gtk_hotkey_info_unbind (info, error))
            {
                return TRUE;
            }
            return FALSE;
        }
    }
    return FALSE;
}










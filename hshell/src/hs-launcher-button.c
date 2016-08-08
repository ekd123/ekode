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
#include <glib-object.h>
#include <glib/gi18n.h>
#include "hs-marshal.h"
#include "hs-desktop-proc.h"
#include "hs-launcher-button.h"

struct _HsLauncherButtonPrivate
{
    GDesktopAppInfo *appinfo;
    gboolean launch;
};

enum
{
    PROP_0,
    PROP_APPINFO,
    PROP_LAUNCH
};

enum
{
    SIGNAL_0,
    SIGNAL_ERROR_LAUNCHING,
    SIGNAL_LAUNCHED,
    SIGNAL_LAST
};

static gint signals[SIGNAL_LAST] = { 0 };

static void hs_launcher_button_dispose      (GObject      *object);
static void hs_launcher_button_finalize     (GObject      *object);
static void hs_launcher_button_set_property (GObject      *object,
                                             guint         prop_id,
                                             const GValue *value,
                                             GParamSpec   *pspec);
static void hs_launcher_button_get_property (GObject      *object,
                                             guint         prop_id,
                                             GValue       *value,
                                             GParamSpec   *pspec);
static void _signal_clicked_cb              (GtkButton    *button,
                                             gpointer      user_data);
G_DEFINE_TYPE (HsLauncherButton, hs_launcher_button, GTK_TYPE_BUTTON)

static void
hs_launcher_button_class_init (HsLauncherButtonClass *klass)
{
    GObjectClass *gobject_class = (GObjectClass *)klass;

    g_type_class_add_private (klass, sizeof (HsLauncherButtonPrivate));

    gobject_class->dispose = hs_launcher_button_dispose;
    gobject_class->finalize = hs_launcher_button_finalize;
    gobject_class->set_property = hs_launcher_button_set_property;
    gobject_class->get_property = hs_launcher_button_get_property;

    /**
     * HsLauncherButton:launch:
     * 
     * Whether the button should launch the application if button is pressed.
     */
    g_object_class_install_property (gobject_class, PROP_LAUNCH,
                                     g_param_spec_boolean ("launch",
                                                           "Launch",
                                                           "Whether the button should launch the application if button is pressed.", 
                                                           TRUE, 
                                                           G_PARAM_READWRITE));
    /**
     * HsLauncherButton:appinfo:
     * 
     * The #GDesktopAppInfo to be processed.
     */
    g_object_class_install_property (gobject_class, PROP_APPINFO,
                                     g_param_spec_object ("appinfo",
                                                          "App Info",
                                                          "The GDesktopAppInfo to be processed",
                                                          G_TYPE_DESKTOP_APP_INFO,
                                                          G_PARAM_READWRITE));

    /**
     * HsLauncherButton::error-launching:
     * 
     * it's emitted if any errors during launching
     */
    signals[SIGNAL_ERROR_LAUNCHING] = g_signal_new ("error-launching",
                                                    HS_TYPE_LAUNCHER_BUTTON,
                                                    G_SIGNAL_RUN_FIRST,
                                                    G_STRUCT_OFFSET(HsLauncherButtonClass, error_launching),
                                                    NULL, NULL,
                                                    hs_marshal_VOID__STRING,
                                                    G_TYPE_NONE, 1,
                                                    G_TYPE_STRING, NULL);

    /**
     * HsLauncherButton::launched:
     * 
     * Emitted when a program launched
     */
    signals[SIGNAL_LAUNCHED] = g_signal_new ("launched",
                                             HS_TYPE_LAUNCHER_BUTTON,
                                             G_SIGNAL_ACTION,
                                             G_STRUCT_OFFSET (HsLauncherButtonClass, launched),
                                             NULL, NULL, 
                                             hs_marshal_VOID__VOID,
                                             G_TYPE_NONE, 0, NULL);
}

static void
hs_launcher_button_init (HsLauncherButton *self)
{
    self->priv = G_TYPE_INSTANCE_GET_PRIVATE (self,
                                              HS_TYPE_LAUNCHER_BUTTON,
                                              HsLauncherButtonPrivate);
    g_signal_connect (self, "clicked", G_CALLBACK (_signal_clicked_cb), NULL);
}

static void 
hs_launcher_button_set_property (GObject      *object,
                                 guint         prop_id,
                                 const GValue *value,
                                 GParamSpec   *pspec)
{
    HsLauncherButton *self = HS_LAUNCHER_BUTTON (object);
    switch (prop_id)
    {
    case PROP_LAUNCH:
        hs_launcher_button_set_launch (self, g_value_get_boolean (value));
        break;
    case PROP_APPINFO:
        hs_launcher_button_set_appinfo (self, g_value_get_object (value));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
        break;
    }
}

static void
hs_launcher_button_get_property (GObject    *object,
                          guint       prop_id,
                          GValue     *value,
                          GParamSpec *pspec)
{
    HsLauncherButton *self = HS_LAUNCHER_BUTTON (object);
    switch (prop_id)
    {
    case PROP_LAUNCH:
        g_value_set_boolean (value, self->priv->launch);
        break;
    case PROP_APPINFO:
        g_value_set_object (value, hs_launcher_button_get_appinfo (self));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
        break;
    }
}

static void
hs_launcher_button_dispose (GObject *object)
{
    HsLauncherButton *self = (HsLauncherButton *)object;

    if (self->priv->appinfo)
    {
        g_object_unref (self->priv->appinfo);
    }

    G_OBJECT_CLASS (hs_launcher_button_parent_class)->dispose (object);
}

static void
hs_launcher_button_finalize (GObject *object)
{
    g_signal_handlers_destroy (object);
    G_OBJECT_CLASS (hs_launcher_button_parent_class)->finalize (object);
}

static void
_signal_clicked_cb (GtkButton *button,
                    gpointer   user_data)
{
    HsLauncherButton *self = HS_LAUNCHER_BUTTON (button);
    GError *error = NULL;
    g_app_info_launch (G_APP_INFO (self->priv->appinfo), 
                       NULL, NULL, &error);
    if (error)
    {
        g_signal_emit (button, signals[SIGNAL_ERROR_LAUNCHING], 0, error->message);
        g_error_free (error);
        return;
    }
    g_signal_emit (button, signals[SIGNAL_LAUNCHED], 0);
}

/**
 * hs_launcher_button_new:
 * 
 * Create a new #HsLauncherButton
 * 
 * Returns: a newly-created #HsLauncherButton
 */
GtkWidget *
hs_launcher_button_new (void)
{
    return GTK_WIDGET (g_object_new (HS_TYPE_LAUNCHER_BUTTON, NULL));
}

/**
 * hs_launcher_button_new_with_appinfo:
 * @appinfo: a #GDesktopAppInfo which will be used by #HsLauncherButton
 * 
 * Create a #HsLauncherButton with #GDesktopAppInfo
 * 
 * Returns: a newly-created #HsLauncherButton
 */
GtkWidget *
hs_launcher_button_new_with_appinfo (GDesktopAppInfo *appinfo)
{
    return GTK_WIDGET (g_object_new (HS_TYPE_LAUNCHER_BUTTON, 
                                     "appinfo", appinfo, NULL));
}

/**
 * hs_launcher_button_new_full:
 * 
 * Create a new #HsLauncherButton with properties set
 * 
 * Returns: a newly-created #HsLauncherButton
 */
GtkWidget *
hs_launcher_button_new_full (gboolean launch, 
                             GDesktopAppInfo *appinfo)
{
    return GTK_WIDGET (g_object_new (HS_TYPE_LAUNCHER_BUTTON, 
                                     "launch", launch,
                                     "appinfo", appinfo,
                                     NULL));
}

/**
 * hs_launcher_button_set_launch:
 * @button: a #HsLauncherButton
 * 
 * Set which #GDesktopAppInfo should be used.
 * #HsLauncherButton will take the ownership so that you shouldn't unref it manually.
 */
void 
hs_launcher_button_set_appinfo (HsLauncherButton *button,
                                GDesktopAppInfo  *appinfo)
{
    if (button->priv->appinfo)
        button->priv->appinfo = NULL;
    button->priv->appinfo = appinfo;
}

/**
 * hs_launcher_button_get_launch:
 * @button: a #HsLauncherButton
 * 
 * Get which #GDesktopAppInfo are used by @button.
 * 
 * Returns: (transfer none): the pointer to the #GDesktopAppInfo
 */
GDesktopAppInfo *
hs_launcher_button_get_appinfo (HsLauncherButton *button)
{
    return button->priv->appinfo;
}

/**
 * hs_launcher_button_set_launch:
 * @button: a #HsLauncherButton
 * 
 * Set whether the button should launch the application if it's pressed.
 */
void 
hs_launcher_button_set_launch (HsLauncherButton *button,
                               gboolean          launch)
{
    button->priv->launch = launch;
}

/**
 * hs_launcher_button_get_launch:
 * @button: a #HsLauncherButton
 * 
 * Get whether the button should launch the application if it's pressed.
 * 
 * Returns: TRUE if yes, FALSE if no.
 */
gboolean
hs_launcher_button_get_launch (HsLauncherButton *button)
{
    return button->priv->launch;
}



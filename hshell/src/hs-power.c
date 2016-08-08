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
#include <glib.h>
#include <glib/gi18n.h>
#include <gio/gio.h>
#include "hs-message.h"
#include "hs-power.h"

static gboolean 
init_dbus (const gchar *func,
           GVariant    *args)
{
    GDBusConnection *c = NULL;
    GDBusProxy *proxy = NULL;
    GVariant *ret = NULL;
    GError *error = NULL;
    GString *msg = g_string_new ("");

    c = g_bus_get_sync (G_BUS_TYPE_SYSTEM, NULL, &error);
    if (!c)
    {
        g_string_printf (msg, _("Could not operate (stage 1): %s"), error->message);
        hsm_warning (_("Power management failed"), GTK_BUTTONS_OK, msg->str);
        goto lbl_failed;
    }
    proxy = g_dbus_proxy_new_sync (c, G_DBUS_PROXY_FLAGS_NONE, NULL, 
                                   "org.freedesktop.Hal", 
                                   "/org/freedesktop/Hal/devices/computer",
                                   "org.freedesktop.Hal.Device.SystemPowerManagement",
                                   NULL, &error);
    if (!proxy)
    {
        g_string_printf (msg, _("Could not operate (stage 2): %s"), error->message);
        hsm_warning (_("Power management failed"), GTK_BUTTONS_OK, msg->str);
        goto lbl_failed;
    }
    ret = g_dbus_proxy_call_sync (proxy, func, args, G_DBUS_CALL_FLAGS_NONE, 
                                  -1, NULL, &error);
    if (!ret)
    {
        g_string_printf (msg, _("Could not operate (stage 3): %s"), (error==NULL)?NULL:error->message);
        hsm_warning (_("Power management failed"), GTK_BUTTONS_OK, msg->str);
        goto lbl_failed;
    }
    g_string_free (msg, TRUE);
    return TRUE;

  lbl_failed:
    if (c)
        g_object_unref (c);
    if (proxy)
        g_object_unref (proxy);
    g_string_free (msg, TRUE);
    return FALSE;
}

gboolean 
hs_power_logout (void)
{
    GDBusConnection *c = NULL;
    GDBusProxy *proxy = NULL;
    GVariant *ret = NULL;
    GError *error = NULL;
    GString *msg = g_string_new ("");

    c = g_bus_get_sync (G_BUS_TYPE_SESSION, NULL, &error);
    if (!c)
    {
        g_string_printf (msg, _("Could not operate (stage 1): %s"), error->message);
        hsm_warning (_("Power management failed"), GTK_BUTTONS_OK, msg->str);
        goto lbl_failed;
    }
    proxy = g_dbus_proxy_new_sync (c, G_DBUS_PROXY_FLAGS_NONE, NULL, 
                                   "org.home.hsession.Manager", 
                                   "/org/home/hsession/Manager/Proxy",
                                   "org.home.hsession.Manager.Session",
                                   NULL, &error);
    if (!proxy)
    {
        g_string_printf (msg, _("Could not operate (stage 2): %s"), error->message);
        hsm_warning (_("Power management failed"), GTK_BUTTONS_OK, msg->str);
        goto lbl_failed;
    }
    ret = g_dbus_proxy_call_sync (proxy, "Logout", NULL, G_DBUS_CALL_FLAGS_NONE, 
                                  -1, NULL, &error);
    if (!ret)
    {
        g_string_printf (msg, _("Could not operate (stage 3): %s"), (error==NULL)?NULL:error->message);
        hsm_warning (_("Power management failed"), GTK_BUTTONS_OK, msg->str);
        goto lbl_failed;
    }
    g_string_free (msg, TRUE);
    return TRUE;

  lbl_failed:
    if (c)
        g_object_unref (c);
    if (proxy)
        g_object_unref (proxy);
    g_string_free (msg, TRUE);
    return FALSE;
}

gboolean 
hs_power_halt (void)
{
    if (init_dbus ("Shutdown", NULL))
        return TRUE;
    return FALSE;
}

gboolean 
hs_power_restart (void)
{
    if (init_dbus ("Reboot", NULL))
        return TRUE;
    return FALSE;
}

gboolean 
hs_power_suspend (void)
{
    if (init_dbus ("Suspend", g_variant_new_int32 (0)))
        return TRUE;
    return FALSE;
}

gboolean 
hs_power_hibernate (void)
{
    if (init_dbus ("Hibernate", NULL))
        return TRUE;
    return FALSE;
}

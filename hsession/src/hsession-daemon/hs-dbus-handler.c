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
#include <gio/gio.h>
#include "../dbus/hs-dbus-manager.h"

static GDBusObjectManagerServer *manager = NULL;

/* quit the main loop */
static void 
_on_session_logout (HsSession             *session,
                    GDBusMethodInvocation *invocation,
                    gpointer               user_data)
{
    hs_session_complete_logout (session, invocation);
    g_main_loop_quit ((GMainLoop*)user_data);
}

void 
hs_dbus_handler_bus_acquired (GDBusConnection *connection,
                              const gchar     *name,
                              gpointer         user_data)
{
    HsObjectSkeleton *object;
    HsPower *power;
    HsSession *session;

    manager = g_dbus_object_manager_server_new ("/org/home/hsession/Manager");
    object = hs_object_skeleton_new ("/org/home/hsession/Manager/Proxy");
    power = hs_power_skeleton_new ();
    session = hs_session_skeleton_new ();

    hs_object_skeleton_set_power (object, power);
    g_object_unref (power);
    hs_object_skeleton_set_session (object, session);
    g_object_unref (session);

    g_signal_connect (session, "handle-logout", G_CALLBACK (_on_session_logout), user_data);

    g_dbus_object_manager_server_export (manager, G_DBUS_OBJECT_SKELETON (object));
    g_object_unref (object);

    g_dbus_object_manager_server_set_connection (manager, connection);
}

void 
hs_dbus_handler_name_lost (GDBusConnection *connection,
                           const gchar     *name,
                           gpointer         user_data)
{
    g_critical (_("D-Bus name lost!"));
    g_main_loop_quit ((GMainLoop*)user_data);
}

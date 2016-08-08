/* this file is part of RatDB */
/**
 * Copyright (c) 2011, 2012, 2013 Mike Manilone
 * All rights reserved.
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
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
#include <glib.h>
#include <glib/gstdio.h>
#include <gio/gio.h>
#include <gio/gunixsocketaddress.h>

#include <glib/gi18n.h>

#include "ratdb-engine.h"
#include "ratdb-client.h"
#include "ratdb-daemon.h"
#include "ratdb-dbus-proxy.h"

/**
 * SECTION: RatdbDaemon
 * @Short_description: The Main Daemon
 * @Title: RatdbDaemon
 * @Include: ratdb-daemon.h
 * 
 * A DBus server.
 */

G_DEFINE_TYPE (RatdbDaemon, ratdb_daemon, G_TYPE_THREADED_SOCKET_SERVICE)

#define RATDB_GET_DAEMON_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), RATDB_TYPE_DAEMON, RatdbDaemonPrivate))
struct _RatdbDaemonPrivate
{
    GMainLoop  *mainloop;
    GHashTable *client_list;
    GMutex     *list_lock;
    GMutex      list_lock_impl;
};

static void
ratdb_daemon_dispose (GObject *object)
{
    RatdbDaemonPrivate *priv = RATDB_DAEMON (object)->priv;

    if (priv->mainloop)
    {
        g_main_loop_unref (priv->mainloop);
        priv->mainloop = NULL;
    }

    if (priv->client_list)
    {
        g_hash_table_unref (priv->client_list);
        priv->client_list = NULL;
    }

    G_OBJECT_CLASS (ratdb_daemon_parent_class)->dispose (object);
}

static void
ratdb_daemon_finalize (GObject *object)
{
    RatdbDaemonPrivate *priv = RATDB_DAEMON (object)->priv;

    if (priv->list_lock)
    {
        g_mutex_clear (priv->list_lock);
        priv->list_lock = NULL;
    }

    G_OBJECT_CLASS (ratdb_daemon_parent_class)->finalize (object);
}

static void
ratdb_daemon_init (RatdbDaemon *daemon)
{
    daemon->priv = RATDB_GET_DAEMON_PRIVATE (daemon);

    g_mutex_init (&daemon->priv->list_lock_impl);
    daemon->priv->list_lock = &daemon->priv->list_lock_impl;

    daemon->priv->mainloop = g_main_loop_new (NULL, FALSE);

    daemon->priv->client_list = g_hash_table_new_full (g_str_hash, g_str_equal, g_free, g_object_unref);
}

static void
ratdb_daemon_class_init (RatdbDaemonClass *daemon_klass)
{
    g_type_class_add_private (daemon_klass, sizeof (RatdbDaemonPrivate));
    
    G_OBJECT_CLASS (daemon_klass)->finalize = ratdb_daemon_finalize;
    G_OBJECT_CLASS (daemon_klass)->dispose = ratdb_daemon_dispose;
}

static void 
_on_name_acquired (GDBusConnection *connection,
                   const gchar     *name,
                   gpointer         user_data)
{
    g_debug ("Name acquired %s", name);
}

static void
_on_name_lost (GDBusConnection     *connection,
               const gchar         *name,
               gpointer             user_data)
{
    g_debug ("Name lost %s", name);
}

/**
   Handle "Request"

   @user_data: priv->client_list
*/
static gboolean 
ratdb_dbus_handler_request (RatdbProxyEngine       *skeleton,
                            GDBusMethodInvocation  *invocation,
                            const gchar            *type,
                            gpointer                user_data)
{
    GHashTable *clients = (GHashTable *)user_data;
    const gchar *uuid = NULL;
    RatdbClient *client = ratdb_client_new (type);

    if (!client)
    {
        g_dbus_method_invocation_return_dbus_error (invocation,
                                                    "org.ratdb.Daemon.Error",
                                                    _("failed to create a backend"));
        return TRUE;
    }

    uuid = ratdb_client_get_uuid (client);
    /* we don't take the refcount */
    g_hash_table_insert (clients, g_strdup (uuid), client);

    /* Called successfully */
    ratdb_proxy_engine_complete_request (skeleton, invocation, uuid);
    g_debug ("Object for new client created");
    return TRUE;
}

/**
   Handle "NewDatabase"
   
   @user_data: daemon itself
*/
static gboolean
ratdb_dbus_handler_new_database (RatdbProxyEngine      *skeleton,
                                 GDBusMethodInvocation *invocation,
                                 const gchar           *uuid,
                                 RatdbNewDBFlags        flags,
                                 const gchar           *name,
                                 const gchar           *path,
                                 const gchar           *version,
                                 gpointer               iface_user_data,
                                 gpointer               user_data)
{
    RatdbEngine *engine = ratdb_client_get_engine (ratdb_daemon_get_client (RATDB_DAEMON (user_data), uuid));
    ratdb_engine_new_database (engine, flags, name, path, version, iface_user_data);
    /* if (ratdb_engine_new_database (engine, flags, name, path, version, user_data)) */
    /* { */
    ratdb_proxy_engine_complete_new_database (skeleton, invocation);
    /* } */
    /* else  */
    /* { */
    /*     g_dbus_method_invocation_return_dbus_error (invocation, "org.ratdb.Daemon.Error", _("creating database instance failed")); */
    /* } */
    return TRUE;
}

static gboolean 
ratdb_dbus_handler_new_table (RatdbProxyEngine      *skeleton,
                              GDBusMethodInvocation *invocation,
                              const gchar           *uuid,
                              const gchar           *name,
                              gpointer               iface_user_data,
                              gpointer               user_data)
{
    RatdbEngine *engine = ratdb_client_get_engine (ratdb_daemon_get_client (RATDB_DAEMON (user_data), uuid));
    if (ratdb_engine_new_table (engine, name, iface_user_data))
    {
        ratdb_proxy_engine_complete_new_table (skeleton, invocation);
    }
    else
    {
        g_dbus_method_invocation_return_dbus_error (invocation, "org.ratdb.Daemon.Error", _("creating new table failed"));
    }
    return TRUE;
}

/**
   Handle "RemoveClient"
   
   @user_data: daemon itself
*/
static gboolean 
ratdb_dbus_handler_remove_client (RatdbProxyEngine       *skeleton,
                                  GDBusMethodInvocation  *invocation,
                                  const gchar            *uuid,
                                  gpointer                user_data)
{
    ratdb_daemon_remove (RATDB_DAEMON (user_data), uuid);
    ratdb_proxy_engine_complete_remove_client (skeleton, invocation);
    return TRUE;
}

/**
 * @user_data: daemon itself
 */
static void 
_on_bus_acquired (GDBusConnection *connection,
                  const gchar     *name,
                  gpointer         user_data)
{
    RatdbProxyEngine *skeleton;
    GError *error = NULL;
    RatdbDaemonPrivate *priv = RATDB_DAEMON (user_data)->priv;

    g_debug ("Bus acquired");

    /* /org/ratdb/Interface */
    skeleton = ratdb_proxy_engine_skeleton_new ();
    if (error)
    {
        g_error (_("Couldn't create a skeleton for clients: %s"), error->message);
        g_error_free (error);
    }

    g_dbus_interface_skeleton_export (G_DBUS_INTERFACE_SKELETON (skeleton), 
                                      connection, "/org/ratdb/Interface", &error);
    if (error)
    {
        g_error (_("Failed to export symbols: %s"), error->message);
        g_error_free (error);
    }

    /* handle functions */
    RATDB_CONNECT_DBUS_IFACE (request, ratdb_dbus_handler, priv->client_list);
    RATDB_CONNECT_DBUS_IFACE (remove_client, ratdb_dbus_handler, user_data);
    RATDB_CONNECT_DBUS_IFACE (new_database, ratdb_dbus_handler, user_data);
    RATDB_CONNECT_DBUS_IFACE (new_table, ratdb_dbus_handler, user_data);
}

/**
 * ratdb_daemon_run:
 * @daemon: a #RatdbDaemon
 * 
 * Run the daemon.
 * 
 * Returns: TRUE on success.
 */
gboolean
ratdb_daemon_run (RatdbDaemon *daemon)
{
    RatdbDaemonPrivate *priv = daemon->priv;
    guint id;

    id = g_bus_own_name (G_BUS_TYPE_SESSION,
                         "org.ratdb.Interface",
                         G_BUS_NAME_OWNER_FLAGS_NONE,
                         _on_bus_acquired,
                         _on_name_acquired,
                         _on_name_lost,
                         daemon, NULL);
    g_main_loop_run (priv->mainloop);

    g_bus_unown_name (id);
    g_main_loop_unref (priv->mainloop);
    return TRUE;
}

/**
 * ratdb_daemon_close_all:
 * @daemon: a #RatdbDaemon
 * 
 * Close all the connections.
 */
void
ratdb_daemon_close_all (RatdbDaemon *daemon)
{
    g_hash_table_remove_all (daemon->priv->client_list);
}

/**
 * ratdb_daemon_remove:
 * @daemon: a #RatdbDaemon
 * @uuid: the client UUID
 * 
 * remove a client from the list.
 */
void
ratdb_daemon_remove (RatdbDaemon *daemon,
                     const gchar *uuid)
{
    RatdbDaemonPrivate *priv = daemon->priv;

    g_debug ("Removing client `%s'", uuid);

    g_hash_table_remove (priv->client_list, uuid);
}

/**
 * ratdb_daemon_get_client:
 * @daemon: a #RatdbDaemon
 * @uuid: the client UUID
 * 
 * Get the #RatdbClient of client represented by @uuid.
 * 
 * I don't increase the refcount, no need to unref it ;-)
 * 
 * Returns: (transfer none): the #RatdbClient, or %NULL if none found
 */
RatdbClient *
ratdb_daemon_get_client (RatdbDaemon *daemon,
                         const gchar *uuid)
{
    return RATDB_CLIENT (g_hash_table_lookup (daemon->priv->client_list, uuid));
}

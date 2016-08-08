/* this file is part of RatDB */
/**
 * Copyright (c) 2011 Mike Manilone
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
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include <glib.h>
#include <glib/gi18n.h>
#include <gio/gio.h>
#include <gio/gunixconnection.h>
#include <gio/gunixsocketaddress.h>

#include "ratdb-settings.h"
#include "ratdb-client.h"

G_DEFINE_TYPE (RatdbClient, ratdb_client, G_TYPE_OBJECT)

typedef struct _RatdbClientPrivate RatdbClientPrivate;
struct _RatdbClientPrivate
{
    GSocket       *socket;
    RatdbSettings *settings;
};
#define RATDB_GET_CLIENT_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), RATDB_TYPE_CLIENT, RatdbClientPrivate))

static void
ratdb_client_dispose (GObject *object)
{
    RatdbClientPrivate *priv = RATDB_GET_CLIENT_PRIVATE (object);
    
    if (priv->socket)
    {
        GError *error = NULL;
        if (ratdb_client_disconnect (RATDB_CLIENT (object), &error) 
            == FALSE)
        {
            g_warning (_("Failed to close the socket: %s"), error->message);
            g_error_free (error);
        }
        
        priv->socket = NULL;
    }
    
    if (priv->settings)
    {
        g_object_unref (priv->settings);
        priv->settings = NULL;
    }
    
    G_OBJECT_CLASS (ratdb_client_parent_class)->dispose (object);
}

static void
ratdb_client_finalize (GObject *object)
{
    G_OBJECT_CLASS (ratdb_client_parent_class)->finalize (object);
}

static void
ratdb_client_class_init (RatdbClientClass *client_klass)
{
    g_type_class_add_private (client_klass, sizeof (RatdbClientPrivate));
    
    G_OBJECT_CLASS (client_klass)->dispose = ratdb_client_dispose;
    G_OBJECT_CLASS (client_klass)->finalize = ratdb_client_finalize;
}

static void
ratdb_client_init (RatdbClient *client)
{
    GError *error = NULL;
    RatdbClientPrivate *priv = RATDB_GET_CLIENT_PRIVATE (client);
    
    priv->settings = ratdb_settings_new ();
    
    priv->socket = g_socket_new (G_SOCKET_FAMILY_UNIX, 
                                 G_SOCKET_TYPE_STREAM,
                                 G_SOCKET_PROTOCOL_DEFAULT,
                                 &error);
    if (!priv->socket)
    {
        g_critical (_("Failed to create a socket object: %s"), 
                 error->message);
        g_error_free (error);
    }
}

RatdbClient *
ratdb_client_new (void)
{
    return g_object_new (RATDB_TYPE_CLIENT, NULL);
}

/**
 * ratdb_client_connect:
 * @client: A #RatdbClient.
 * @error: (allow-none): #GError for error reporting, or %NULL to ignore.
 * 
 * Connect to the server.
 * You have to call it before you do anything with the #RatdbClient.
 * 
 * Returns: TRUE on success.
 */
gboolean 
ratdb_client_connect (RatdbClient  *client,
                      GError      **error)
{
    GSocketAddress *addr;
    RatdbClientPrivate *priv;
    
    g_return_val_if_fail (RATDB_IS_CLIENT (client), FALSE);
    
    priv = RATDB_GET_CLIENT_PRIVATE (client);
    
    /* get the path */
    {
        gchar *path;
        path = ratdb_client_raw_get_socket_path (client);
        addr = g_unix_socket_address_new (path);
        g_free (path);
    }
    
    /* connect */
    {
        GError *error = NULL;
        
        if (!g_socket_connect (priv->socket, addr, NULL, &error))
        {
            g_critical (_("Failed to connect to the daemon: %s"), 
                     error->message);
            g_error_free (error);
            return FALSE;
        }
    }
    
    /* clean up */
    {
        g_object_unref (addr);
    }
    
    return TRUE;
}

/**
 * ratdb_client_disconnect:
 * @client: A #RatdbClient.
 * @error: (allow-none): #GError for error reporting, or %NULL to ignore.
 * 
 * Disconnect from the server.
 * This function will be called when it destroys.
 * 
 * However, it will not destroy if you call it.
 * 
 * Returns: TRUE on success.
 */
gboolean
ratdb_client_disconnect (RatdbClient  *client,
                         GError      **error)
{
    RatdbClientPrivate *priv = RATDB_GET_CLIENT_PRIVATE (client);
    
    g_return_val_if_fail (RATDB_IS_CLIENT (client), FALSE);
    
    g_socket_close (priv->socket, error);
    
    return TRUE;
}

/**
 * ratdb_client_get_socket_path:
 * @client: A #RatdbClient.
 * 
 * Get the path of the socket file.
 * Free it with #g_free().
 * 
 * Returns: A newly allocated string.
 */
gchar *
ratdb_client_raw_get_socket_path (RatdbClient *client)
{
    RatdbClientPrivate *priv = RATDB_GET_CLIENT_PRIVATE (client);
    
    g_return_val_if_fail (RATDB_IS_CLIENT (client), NULL);
    
    return g_settings_get_string (priv->settings->settings, 
                                  RATDB_CFG_SOCKET_PATH);
}

/**
 * ratdb_client_get_socket:
 * @client: a #RatdbClient
 * 
 * Get the socket.
 * 
 * Returns: the pointer to the socket. please use g_object_unref if 
 * no longer need it.
 */
GSocket *
ratdb_client_get_socket (RatdbClient *client)
{
    RatdbClientPrivate *priv;

    g_return_val_if_fail (RATDB_IS_CLIENT (client), NULL);

    priv = RATDB_GET_CLIENT_PRIVATE (client);

    return G_SOCKET (g_object_ref (priv->socket));
}


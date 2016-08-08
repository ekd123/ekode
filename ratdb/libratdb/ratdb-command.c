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

#include <string.h>

#include <glib.h>
#include <gio/gio.h>

#include "ratdb-command.h"

G_DEFINE_TYPE (RatdbCommand, ratdb_command, G_TYPE_OBJECT)

#define RATDB_GET_COMMAND_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), RATDB_TYPE_COMMAND, RatdbCommandPrivate))
typedef struct _RatdbCommandPrivate RatdbCommandPrivate;
struct _RatdbCommandPrivate
{
    GSocket *socket;
};

enum
{
    PROP_0,

    PROP_SOCKET
};

static void ratdb_command_dispose ();
static void ratdb_command_finalize ();
static void ratdb_command_set_property ();
static void ratdb_command_get_property ();

static void 
ratdb_command_init (RatdbCommand *commander)
{
}

static void 
ratdb_command_class_init (RatdbCommandClass *klass)
{
    /* priv */
    g_type_class_add_private (klass, sizeof (RatdbCommandPrivate));

    /* v-funcs */
    G_OBJECT_CLASS (klass)->dispose = ratdb_command_dispose;
    G_OBJECT_CLASS (klass)->finalize = ratdb_command_finalize;
    G_OBJECT_CLASS (klass)->set_property = ratdb_command_set_property;
    G_OBJECT_CLASS (klass)->get_property = ratdb_command_get_property;

    /* properties */
    /**
     * RatdbCommand:socket:
     * 
     * The socket which will be processed.
     */
    g_object_class_install_property (G_OBJECT_CLASS (klass), PROP_SOCKET, 
                                     g_param_spec_object ("socket", "the socket",
                                                          "Set/get the socket object.",
                                                          G_TYPE_SOCKET,
                                                          G_PARAM_READWRITE | G_PARAM_CONSTRUCT));
}

static void
ratdb_command_dispose (GObject *object)
{
    RatdbCommandPrivate *priv;

    priv = RATDB_GET_COMMAND_PRIVATE (object);

    if (priv->socket)
    {
        g_object_unref (priv->socket);
        priv->socket = NULL;
    }

    G_OBJECT_CLASS (ratdb_command_parent_class)->dispose (object);
}

static void
ratdb_command_finalize (GObject *object)
{
    G_OBJECT_CLASS (ratdb_command_parent_class)->finalize (object);
}

static void
ratdb_command_set_property (GObject           *object,
                            guint              property_id,
                            const GValue      *value,
                            GParamSpec        *pspec)
{
    RatdbCommandPrivate *priv;
    priv = RATDB_GET_COMMAND_PRIVATE (object);

    switch (property_id)
    {
        case PROP_SOCKET:
            priv->socket = g_value_get_object (value);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
            break;
    }
}

static void
ratdb_command_get_property (GObject    *object,
                            guint       property_id,
                            GValue     *value,
                            GParamSpec *pspec)
{
    RatdbCommandPrivate *priv;
    priv = RATDB_GET_COMMAND_PRIVATE (object);

    switch (property_id)
    {
        case PROP_SOCKET:
            g_value_set_object (value, priv->socket);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
            break;
    }
}


/**
 * ratdb_command_new:
 * @socket: a #GSocket which connected to the server
 * 
 * This function creates a new #RatdbCommand.
 * 
 * Returns: a newly created #RatdbCommand.
 */
RatdbCommand *
ratdb_command_new (GSocket *socket)
{
    return g_object_new (RATDB_TYPE_COMMAND, 
                         "socket", socket,
                         NULL);
}

/**
 * ratdb_command_close:
 * @commander: a #RatdbCommand
 *
 * Destroy a #RatdbCommand. But it won't
 * close the socket.
 */
void
ratdb_command_destroy (RatdbCommand *commander)
{
    g_return_if_fail (RATDB_IS_COMMAND (commander));

    g_object_unref (commander);
}

/**
 * ratdb_command_raw_send:
 * @commander: a #RatdbCommand
 * @buffer: the buffer containing the data to send
 * @size: the number of bytes to send
 * @error: #GError for error reporting, or %NULL to ignore
 * 
 * Send a message to the server.
 * 
 * For more datails please read the documentation of 
 * g_socket_send.
 * 
 * Returns: the same as g_socket_send.
 */
gssize
ratdb_command_raw_send (RatdbCommand *commander,
                        const gchar  *buffer,
                        gsize length,
                        GError      **error)
{
    GSocket *socket;
    gssize returnval;

    g_return_val_if_fail (RATDB_IS_COMMAND (commander), -1);

    g_object_get (commander,
                  "socket", &socket,
                  NULL);

    returnval = g_socket_send (socket, buffer, length, NULL, error);

    g_object_unref (socket);

    return returnval;
}

/**
 * ratdb_command_raw_recv:
 * @commander: a #RatdbCommand
 * @buffer: a buffer to read data into (which should be at least @length long).
 * @length: the number of bytes you want to read from the socket.
 * @error: #GError for error reporting, or %NULL to ignore.
 * 
 * Read from the connection and put it into @buffer.
 * 
 * For more datails please read the documentation of 
 * g_socket_receive.
 *
 * Returns: the same as g_socket_receive.
 */
gssize
ratdb_command_raw_recv (RatdbCommand *commander,
                        gchar *buffer,
                        gsize length,
                        GError **error)
{
    GSocket *socket;
    gssize returnval;

    g_return_val_if_fail (RATDB_IS_COMMAND (commander), -1);

    g_object_get (commander,
                  "socket", &socket,
                  NULL);

    returnval = g_socket_receive (socket, buffer, length, NULL, error);

    g_object_unref (socket);

    return returnval;
}

gboolean
ratdb_command_request (RatdbCommand *commander,
                       const gchar  *line)
{
    GSocket *socket;
    GError *error = NULL;

    g_return_val_if_fail (RATDB_IS_COMMAND (commander), FALSE);

    g_object_get (commander, "socket", &socket, NULL);

    ratdb_command_raw_send (commander, line, strlen (line), &error);

    g_object_unref (socket);

    if (error)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}



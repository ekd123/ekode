/* this file is part of RatDB */
/**
 * Copyright (c) 2011, 2013 Mike Manilone
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

/**
 * SECTION: RatdbClient
 * @Title: RatdbClient
 * @Short_description: Wrapper for client connecting
 * @Include: ratdb-client.h
 * 
 * This is an ABT for clients. Used in #RatdbDaemon for 
 * client managing.
 */

#include <gio/gio.h>

#include "ratdb-engine.h"
#include "ratdb-uuid.h"
#include "ratdb-loader.h"
#include "ratdb-client.h"

struct _RatdbClientPrivate
{
    RatdbEngine *engine;

    gchar *uuid;
};

G_DEFINE_TYPE (RatdbClient, ratdb_client, G_TYPE_OBJECT)

static void
ratdb_client_dispose (GObject *object)
{
    RatdbClient *client = RATDB_CLIENT (object);

    if (client->priv->engine)
    {
        g_object_unref (client->priv->engine);
        client->priv->engine = NULL;
    }

    G_OBJECT_CLASS (ratdb_client_parent_class)->dispose (object);
}

static void
ratdb_client_finalize (GObject *object)
{
    RatdbClient *client = RATDB_CLIENT (object);

    if (client->priv->uuid)
    {
        g_free (client->priv->uuid);
        client->priv->uuid = NULL;
    }

    G_OBJECT_CLASS (ratdb_client_parent_class)->finalize (object);
}

static void
ratdb_client_class_init (RatdbClientClass *klass)
{
    GObjectClass *gobject_class = (GObjectClass *)klass;

    g_type_class_add_private (klass, sizeof (RatdbClientPrivate));
    gobject_class->dispose = ratdb_client_dispose;
    gobject_class->finalize = ratdb_client_finalize;
}

static void
ratdb_client_init (RatdbClient *self)
{
    self->priv = G_TYPE_INSTANCE_GET_PRIVATE (self,
                                              RATDB_TYPE_CLIENT,
                                              RatdbClientPrivate);
    self->priv->uuid = ratdb_uuid_generate ();
}

/**
 * ratdb_client_new:
 * @connect: type of #RatdbEngine
 * 
 * This function creates a newly allocated #RatdbClient.
 * 
 * Returns: a new #RatdbClient
 */
RatdbClient *
ratdb_client_new (const gchar *type)
{
    RatdbClient *object;
    object = g_object_new (RATDB_TYPE_CLIENT, NULL);
    object->priv->engine = RATDB_ENGINE (ratdb_loader_create_instance (type));
    /* failed to create a backend */
    if (!object->priv->engine)
    {
        g_object_unref (object);
        return NULL;
    }
    return object;
}

/**
 * ratdb_client_get_uuid:
 * @client: a #RatdbClient
 * 
 * Get UUID of @client
 * 
 * Returns: (transfer none): UUID
 */
const gchar *
ratdb_client_get_uuid (RatdbClient *client)
{
    return client->priv->uuid;
}

/**
 * ratdb_client_get_engine:
 * @client: a #RatdbClient
 * 
 * Get #RatdbEngine of @client
 * 
 * Returns: (transfer none): a #RatdbEngine instance
 */
RatdbEngine *
ratdb_client_get_engine (RatdbClient *client)
{
    return client->priv->engine;
}

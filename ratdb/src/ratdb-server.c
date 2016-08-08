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

/**
 * SECTION: RatdbServer
 * @Title: RatdbServer
 * @Short_description: #GApplication Support
 * @See_also: #GApplication, #RatdbOptions
 * 
 * Hold a running application, and some basic features like single instance.
 * 
 * The real main function is here. The command option parser is in #RatdbOptions.
 */

#include <stdlib.h>

#include <glib.h>
#include <gio/gio.h>

#include "ratdb-settings.h"
#include "ratdb-loader.h"
#include "ratdb-daemon.h"
#include "ratdb-server.h"

RatdbSettings *settings;

G_DEFINE_TYPE (RatdbServer, ratdb_server, G_TYPE_APPLICATION)

/* private data */
#define RATDB_GET_SERVER_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), RATDB_TYPE_SERVER, RatdbServerPrivate))
typedef struct _RatdbServerPrivate RatdbServerPrivate;
struct _RatdbServerPrivate
{
    gboolean run;
};

static GObject *
ratdb_server_constructor (GType type,
                          guint n_construct_params,
                          GObjectConstructParam *construct_params)
{
    static GObject *_main_instance = NULL;
    if (_main_instance != NULL)
        return _main_instance;
    _main_instance = G_OBJECT_CLASS (ratdb_server_parent_class)->constructor (type, n_construct_params, construct_params);
    g_object_add_weak_pointer (_main_instance, (gpointer)&_main_instance);
    return _main_instance;
}

static void 
ratdb_server_dispose (GObject *object)
{
    G_OBJECT_CLASS (ratdb_server_parent_class)->dispose (object);
}

static void
ratdb_server_finalize (GObject *object)
{
    G_OBJECT_CLASS (ratdb_server_parent_class)->finalize (object);
}

static gboolean ratdb_server_local_command_line ();

static void 
ratdb_server_class_init (RatdbServerClass *server_klass)
{
    g_type_class_add_private (server_klass, sizeof (RatdbServerPrivate));

    G_OBJECT_CLASS (server_klass)->dispose = ratdb_server_dispose;
    G_OBJECT_CLASS (server_klass)->finalize = ratdb_server_finalize;
    G_OBJECT_CLASS (server_klass)->constructor = ratdb_server_constructor;

    G_APPLICATION_CLASS (server_klass)->local_command_line = 
        ratdb_server_local_command_line;
}

static void
ratdb_server_init (RatdbServer *server)
{
    RatdbServerPrivate *priv = RATDB_GET_SERVER_PRIVATE (server);
    
    priv->run = FALSE;
}

/**
 * 
 * This function is called when you invoked `#g_application_run`
 * 
 * The real "main" function.
 * 
 */
static gboolean
ratdb_server_local_command_line (GApplication   *application,
                                 gchar        ***arguments,
                                 int            *exit_status)
{
    GMainLoop *loop = NULL;
    RatdbDaemon *daemon = g_object_new (RATDB_TYPE_DAEMON, NULL);
    RatdbLoader *loader = ratdb_loader_new ();

    g_debug ("program instance is starting");

    /* load backends */
    ratdb_loader_load_engines (loader);
    ratdb_loader_load_filebackends (loader);

    /* start loop */
    loop = g_main_loop_new (NULL, FALSE);
    if (!ratdb_daemon_run (daemon))
    {
    }
    g_main_loop_run (loop);

    /* destroy */
    g_main_loop_unref (loop);
    g_object_unref (daemon);
    
    /* set exit_status */
    *exit_status = 0;
    g_debug ("program instance is closing");
    return TRUE;
}

RatdbServer *
ratdb_server_new (void)
{
    /* initialize the type system */
    g_type_init ();
    
    /* initialize the configuring system */
    settings = ratdb_settings_new ();

    /* here we don't use G_APPLICATION_IS_SERVICE because we
       don't want to abort when another process is running.*/
    return g_object_new (RATDB_TYPE_SERVER,
                         "application-id", RATDB_SERVER_APP_ID,
                         "flags", G_APPLICATION_HANDLES_COMMAND_LINE,
                         NULL);
}

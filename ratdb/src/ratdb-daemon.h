/* this file is part of RatDB */
/**
 * Copyright (c) 2011,2012 Mike Manilone
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
#ifndef __RATDB_DAEMON_H__
#define __RATDB_DAEMON_H__

#include "ratdb-client.h"

/* made with gmacros.sh */
#include <glib-object.h>

G_BEGIN_DECLS

#define RATDB_TYPE_DAEMON            (ratdb_daemon_get_type ())
#define RATDB_DAEMON(o)              (G_TYPE_CHECK_INSTANCE_CAST ((o), RATDB_TYPE_DAEMON, RatdbDaemon))
#define RATDB_DAEMON_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), RATDB_TYPE_DAEMON, RatdbDaemonClass))
#define RATDB_IS_DAEMON(o)           (G_TYPE_CHECK_INSTANCE_TYPE ((o), RATDB_TYPE_DAEMON))
#define RATDB_IS_DAEMON_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), RATDB_TYPE_DAEMON))
#define RATDB_GET_DAEMON_CLASS(o)    (G_TYPE_INSTANCE_GET_CLASS ((o), RATDB_TYPE_DAEMON, RatdbDaemonClass))

typedef struct _RatdbDaemon        RatdbDaemon;
typedef struct _RatdbDaemonClass   RatdbDaemonClass;
typedef struct _RatdbDaemonPrivate RatdbDaemonPrivate;

struct _RatdbDaemon
{
    GThreadedSocketService parent;
    
    /*< public >*/
    const gchar *path;

    /*< private >*/
    RatdbDaemonPrivate *priv;
};

struct _RatdbDaemonClass
{
    GThreadedSocketServiceClass parent_class;
};

GType        ratdb_daemon_get_type        (void);

/* misc */
gchar       *ratdb_daemon_get_socket_path (void);

/* running */
gboolean     ratdb_daemon_run             (RatdbDaemon *daemon);

/* clients managing */
void         ratdb_daemon_remove          (RatdbDaemon *daemon,
                                           const gchar *uuid);
RatdbClient *ratdb_daemon_get_client      (RatdbDaemon *daemon,
                                           const gchar *uuid);

G_END_DECLS

#endif /* __RATDB_DAEMON_H__ */

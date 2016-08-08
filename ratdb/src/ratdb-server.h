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

#ifndef __RATDB_SERVER__
#define __RATDB_SERVER__

/* macros */
#define RATDB_SERVER_APP_ID "org.ratdb.server"

/* made with gmacros.sh */
#include <glib-object.h>

G_BEGIN_DECLS

#define RATDB_TYPE_SERVER            (ratdb_server_get_type ())
#define RATDB_SERVER(o)              (G_TYPE_CHECK_INSTANCE_CAST ((o), RATDB_TYPE_SERVER, RatdbServer))
#define RATDB_SERVER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), RATDB_TYPE_SERVER, RatdbServerClass))
#define RATDB_IS_SERVER(o)           (G_TYPE_CHECK_INSTANCE_TYPE ((o), RATDB_TYPE_SERVER))
#define RATDB_IS_SERVER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), RATDB_TYPE_SERVER))
#define RATDB_GET_SERVER_CLASS(o)    (G_TYPE_INSTANCE_GET_CLASS ((o), RATDB_TYPE_SERVER, RatdbServerClass))

typedef struct _RatdbServer        RatdbServer;
typedef struct _RatdbServerClass   RatdbServerClass;

struct _RatdbServer
{
    GApplication parent;
};

struct _RatdbServerClass
{
    GApplicationClass parent_class;
};

GType ratdb_server_get_type (void);

RatdbServer *ratdb_server_new (void);

G_END_DECLS

#endif /* __RATDB_SERVER__ */


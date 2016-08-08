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

#ifndef __RATDB_COMMAND__
#define __RATDB_COMMAND__

/* made with gmacros.sh */
#include <glib-object.h>

G_BEGIN_DECLS

#define RATDB_TYPE_COMMAND            (ratdb_command_get_type ())
#define RATDB_COMMAND(o)              (G_TYPE_CHECK_INSTANCE_CAST ((o), RATDB_TYPE_COMMAND, RatdbCommand))
#define RATDB_COMMAND_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), RATDB_TYPE_COMMAND, RatdbCommandClass))
#define RATDB_IS_COMMAND(o)           (G_TYPE_CHECK_INSTANCE_TYPE ((o), RATDB_TYPE_COMMAND))
#define RATDB_IS_COMMAND_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), RATDB_TYPE_COMMAND))
#define RATDB_GET_COMMAND_CLASS(o)    (G_TYPE_INSTANCE_GET_CLASS ((o), RATDB_TYPE_COMMAND, RatdbCommandClass))

typedef struct _RatdbCommand        RatdbCommand;
typedef struct _RatdbCommandClass   RatdbCommandClass;

struct _RatdbCommand
{
    GObject parent;
};

struct _RatdbCommandClass
{
    GObjectClass parent_class;
};

GType ratdb_command_get_type (void);
RatdbCommand *ratdb_command_new (GSocket *socket);

G_END_DECLS

#endif /* __RATDB_COMMAND__ */


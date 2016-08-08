/* this file is part of RatDB */
/**
 * Copyright (c) 2012 Mike Manilone
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
#ifndef __RATDB_OPTIONS_H__
#define __RATDB_OPTIONS_H__

/* made with gmacros.sh */
#include <glib-object.h>

G_BEGIN_DECLS

#define RATDB_TYPE_OPTIONS            (ratdb_options_get_type ())
#define RATDB_OPTIONS(o)              (G_TYPE_CHECK_INSTANCE_CAST ((o), RATDB_TYPE_OPTIONS, RatdbOptions))
#define RATDB_OPTIONS_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), RATDB_TYPE_OPTIONS, RatdbOptionsClass))
#define RATDB_IS_OPTIONS(o)           (G_TYPE_CHECK_INSTANCE_TYPE ((o), RATDB_TYPE_OPTIONS))
#define RATDB_IS_OPTIONS_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), RATDB_TYPE_OPTIONS))
#define RATDB_GET_OPTIONS_CLASS(o)    (G_TYPE_INSTANCE_GET_CLASS ((o), RATDB_TYPE_OPTIONS, RatdbOptionsClass))

typedef struct _RatdbOptions        RatdbOptions;
typedef struct _RatdbOptionsClass   RatdbOptionsClass;

struct _RatdbOptions
{
    GObject parent;
};

struct _RatdbOptionsClass
{
    GObjectClass parent_class;
};

GType    ratdb_options_get_type (void);
void     ratdb_options_set      (RatdbOptions *options,
                                 gpointer     key,
                                 gpointer     value);
gpointer ratdb_options_get      (RatdbOptions *options,
                                 gconstpointer key);

G_END_DECLS

#endif


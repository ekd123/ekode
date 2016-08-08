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
#ifndef __RATDB_LOADER_H__
#define __RATDB_LOADER_H__

G_BEGIN_DECLS

#define RATDB_REGISTER_SYMBOL "ratdb_create_instance"

/*------------- START gobject --------------------*/
#define RATDB_TYPE_LOADER                                               \
   (ratdb_loader_get_type())
#define RATDB_LOADER(obj)                                               \
   (G_TYPE_CHECK_INSTANCE_CAST ((obj),                                  \
                                RATDB_TYPE_LOADER,                      \
                                RatdbLoader))
#define RATDB_LOADER_CLASS(klass)                                       \
   (G_TYPE_CHECK_CLASS_CAST ((klass),                                   \
                             RATDB_TYPE_LOADER,                         \
                             RatdbLoaderClass))
#define RATDB_IS_LOADER(obj)                                            \
   (G_TYPE_CHECK_INSTANCE_TYPE ((obj),                                  \
                                RATDB_TYPE_LOADER))
#define RATDB_IS_LOADER_CLASS(klass)                                    \
   (G_TYPE_CHECK_CLASS_TYPE ((klass),                                   \
                             RATDB_TYPE_LOADER))
#define RATDB_GET_LOADER_CLASS(obj)                                     \
   (G_TYPE_INSTANCE_GET_CLASS ((obj),                                   \
                               RATDB_TYPE_LOADER,                       \
                               RatdbLoaderClass))

typedef struct _RatdbLoader      RatdbLoader;
typedef struct _RatdbLoaderClass RatdbLoaderClass;
typedef struct _RatdbLoaderPrivate RatdbLoaderPrivate;

struct _RatdbLoaderClass
{
    GObjectClass parent_class;
};

struct _RatdbLoader
{
    GObject parent;

    RatdbLoaderPrivate *priv;
};
/*------------- END gobject --------------------*/

GType         ratdb_loader_get_type          (void);
RatdbLoader * ratdb_loader_new               (void);
gboolean      ratdb_loader_start             (void);
gboolean      ratdb_loader_load_engines      (RatdbLoader *loader) ;
gboolean      ratdb_loader_load_filebackends (RatdbLoader *loader) ;
gpointer      ratdb_loader_create_instance   (const gchar *type);

G_END_DECLS


#endif /* __RATDB_LOADER_H__ */


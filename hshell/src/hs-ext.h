/**
 * Copyright 2012 Mike Manilone. All rights reserved.
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
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
#ifndef __HS_EXT_H__
#define __HS_EXT_H__

#include <glib-object.h>
#include <lualib.h>
#include <lauxlib.h>

G_BEGIN_DECLS

#define HS_TYPE_EXT                                                     \
    (hs_ext_get_type())
#define HS_EXT(obj)                                                     \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj),                                 \
                                 HS_TYPE_EXT,                           \
                                 HsExt))
#define HS_EXT_CLASS(klass)                                             \
    (G_TYPE_CHECK_CLASS_CAST ((klass),                                  \
                              HS_TYPE_EXT,                              \
                              HsExtClass))
#define HS_IS_EXT(obj)                                                  \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj),                                 \
                                 HS_TYPE_EXT))
#define HS_IS_EXT_CLASS(klass)                                          \
    (G_TYPE_CHECK_CLASS_TYPE ((klass),                                  \
                              HS_TYPE_EXT))
#define HS_GET_EXT_CLASS(obj)                                           \
    (G_TYPE_INSTANCE_GET_CLASS ((obj),                                  \
                                HS_TYPE_EXT,                            \
                                HsExtClass))

typedef struct _HsExt      HsExt;
typedef struct _HsExtClass HsExtClass;
typedef struct _HsExtPrivate HsExtPrivate;

struct _HsExtClass
{
    GObjectClass parent_class;
};

struct _HsExt
{
    GObject parent;

    /*< private >*/
    HsExtPrivate *priv;
};

GType    hs_ext_get_type          (void);
HsExt *  hs_ext_new               (void);
gboolean hs_ext_run_file          (const gchar *file);
double   hs_ext_global_get_number (const gchar *varname);
gchar *  hs_ext_global_get_str    (const gchar *varname);
gint     hs_ext_global_get_int    (const gchar *varname);
gchar ** hs_ext_global_get_strv   (const gchar *varname);

G_END_DECLS

#endif /* __HS_EXT_H__ */

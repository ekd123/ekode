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
#ifndef __HS_RUN_H__
#define __HS_RUN_H__

#include <gtk/gtk.h>


G_BEGIN_DECLS

#define HS_TYPE_RUN                                                     \
   (hs_run_get_type())
#define HS_RUN(obj)                                                     \
   (G_TYPE_CHECK_INSTANCE_CAST ((obj),                                  \
                                HS_TYPE_RUN,                            \
                                HsRun))
#define HS_RUN_CLASS(klass)                                             \
   (G_TYPE_CHECK_CLASS_CAST ((klass),                                   \
                             HS_TYPE_RUN,                               \
                             HsRunClass))
#define HS_IS_RUN(obj)                                                  \
   (G_TYPE_CHECK_INSTANCE_TYPE ((obj),                                  \
                                HS_TYPE_RUN))
#define HS_IS_RUN_CLASS(klass)                                          \
   (G_TYPE_CHECK_CLASS_TYPE ((klass),                                   \
                             HS_TYPE_RUN))
#define HS_GET_RUN_CLASS(obj)                                           \
   (G_TYPE_INSTANCE_GET_CLASS ((obj),                                   \
                               HS_TYPE_RUN,                             \
                               HsRunClass))

typedef struct _HsRun      HsRun;
typedef struct _HsRunClass HsRunClass;
typedef struct _HsRunPrivate HsRunPrivate;

struct _HsRunClass
{
    GtkDialogClass parent_class;
};

struct _HsRun
{
    GtkDialog parent;

    /*< private >*/
    HsRunPrivate *priv;
};

GType    hs_run_get_type     (void);
HsRun *  hs_run_new          (void);
gboolean hs_run_do_run_async (const gchar *commandline,
                              GError     **error);

G_END_DECLS

#endif /* __HS_RUN_H__ */

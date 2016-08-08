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
#ifndef __HS_APP_H__
#define __HS_APP_H__

#include <glib-object.h>

G_BEGIN_DECLS

#define HS_APP_APPID ("org.hshell.application")

#define HS_TYPE_APP                                                     \
   (hs_app_get_type())
#define HS_APP(obj)                                                     \
   (G_TYPE_CHECK_INSTANCE_CAST ((obj),                                  \
                                HS_TYPE_APP,                            \
                                HsApp))
#define HS_APP_CLASS(klass)                                             \
   (G_TYPE_CHECK_CLASS_CAST ((klass),                                   \
                             HS_TYPE_APP,                               \
                             HsAppClass))
#define HS_IS_APP(obj)                                                  \
   (G_TYPE_CHECK_INSTANCE_TYPE ((obj),                                  \
                                HS_TYPE_APP))
#define HS_IS_APP_CLASS(klass)                                          \
   (G_TYPE_CHECK_CLASS_TYPE ((klass),                                   \
                             HS_TYPE_APP))
#define HS_GET_APP_CLASS(obj)                                           \
   (G_TYPE_INSTANCE_GET_CLASS ((obj),                                   \
                               HS_TYPE_APP,                             \
                               HsAppClass))

typedef struct _HsApp      HsApp;
typedef struct _HsAppClass HsAppClass;

struct _HsAppClass
{
    GtkApplicationClass parent_class;
};

struct _HsApp
{
    GtkApplication parent;
};

GType  hs_app_get_type (void);
HsApp *hs_app_new      (void);

G_END_DECLS

#endif /* __HS_APP_H__ */

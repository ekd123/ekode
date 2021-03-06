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

#ifndef __HS_DASH_H__
#define __HS_DASH_H__

#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include "hs-app-chooser.h"

G_BEGIN_DECLS

#define HS_TYPE_DASH                                                    \
   (hs_dash_get_type())
#define HS_DASH(obj)                                                    \
   (G_TYPE_CHECK_INSTANCE_CAST ((obj),                                  \
                                HS_TYPE_DASH,                           \
                                HsDash))
#define HS_DASH_CLASS(klass)                                            \
   (G_TYPE_CHECK_CLASS_CAST ((klass),                                   \
                             HS_TYPE_DASH,                              \
                             HsDashClass))
#define HS_IS_DASH(obj)                                                 \
   (G_TYPE_CHECK_INSTANCE_TYPE ((obj),                                  \
                                HS_TYPE_DASH))
#define HS_IS_DASH_CLASS(klass)                                         \
   (G_TYPE_CHECK_CLASS_TYPE ((klass),                                   \
                             HS_TYPE_DASH))
#define HS_GET_DASH_CLASS(obj)                                          \
   (G_TYPE_INSTANCE_GET_CLASS ((obj),                                   \
                               HS_TYPE_DASH,                            \
                               HsDashClass))

typedef struct _HsDash      HsDash;
typedef struct _HsDashClass HsDashClass;
typedef struct _HsDashPrivate HsDashPrivate;

struct _HsDashClass
{
    GtkGridClass parent_class;

    void (*acted)(void);
};

struct _HsDash
{
    GtkGrid parent;

    /*< private >*/
    HsDashPrivate *priv;
};

GType       hs_dash_get_type      (void);
GtkWidget * hs_dash_new           (HsAppChooser *ac);
void        hs_dash_refresh_icons (HsDash *dash);

G_END_DECLS

#endif /* __HS_DASH_H__ */

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
#ifndef __HS_APP_CHOOSER_H__
#define __HS_APP_CHOOSER_H__

#include <gtk/gtk.h>
#include "hs-filter.h"

G_BEGIN_DECLS

#define HS_TYPE_APP_CHOOSER                                             \
   (hs_app_chooser_get_type())
#define HS_APP_CHOOSER(obj)                                             \
   (G_TYPE_CHECK_INSTANCE_CAST ((obj),                                  \
                                HS_TYPE_APP_CHOOSER,                    \
                                HsAppChooser))
#define HS_APP_CHOOSER_CLASS(klass)                                     \
   (G_TYPE_CHECK_CLASS_CAST ((klass),                                   \
                             HS_TYPE_APP_CHOOSER,                       \
                             HsAppChooserClass))
#define HS_IS_APP_CHOOSER(obj)                                          \
   (G_TYPE_CHECK_INSTANCE_TYPE ((obj),                                  \
                                HS_TYPE_APP_CHOOSER))
#define HS_IS_APP_CHOOSER_CLASS(klass)                                  \
   (G_TYPE_CHECK_CLASS_TYPE ((klass),                                   \
                             HS_TYPE_APP_CHOOSER))
#define HS_GET_APP_CHOOSER_CLASS(obj)                                   \
   (G_TYPE_INSTANCE_GET_CLASS ((obj),                                   \
                               HS_TYPE_APP_CHOOSER,                     \
                               HsAppChooserClass))

typedef struct _HsAppChooser      HsAppChooser;
typedef struct _HsAppChooserClass HsAppChooserClass;
typedef struct _HsAppChooserPrivate HsAppChooserPrivate;

struct _HsAppChooserClass
{
    GtkIconViewClass parent_class;

    void (*filter_updated) (void);
    void (*desktop_updated) (void);
    void (*acted) (void);
};

struct _HsAppChooser
{
    GtkIconView parent;

    /*< private >*/
    HsAppChooserPrivate *priv;
};

GType       hs_app_chooser_get_type                (void);
GtkWidget * hs_app_chooser_new                     (void);
void        hs_app_chooser_add_filter_category     (HsAppChooser *ac,
                                                    const gchar  *value);
void        hs_app_chooser_add_filter_name_include (HsAppChooser *ac,
                                                    const gchar  *value);

G_END_DECLS

#endif /* __HS_APP_CHOOSER_H__ */

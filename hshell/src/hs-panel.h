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
#ifndef __HS_PANEL_H__
#define __HS_PANEL_H__

#include <gtk/gtk.h>


G_BEGIN_DECLS

#define HS_TYPE_PANEL                                                   \
   (hs_panel_get_type())
#define HS_PANEL(obj)                                                   \
   (G_TYPE_CHECK_INSTANCE_CAST ((obj),                                  \
                                HS_TYPE_PANEL,                          \
                                HsPanel))
#define HS_PANEL_CLASS(klass)                                           \
   (G_TYPE_CHECK_CLASS_CAST ((klass),                                   \
                             HS_TYPE_PANEL,                             \
                             HsPanelClass))
#define HS_IS_PANEL(obj)                                                \
   (G_TYPE_CHECK_INSTANCE_TYPE ((obj),                                  \
                                HS_TYPE_PANEL))
#define HS_IS_PANEL_CLASS(klass)                                        \
   (G_TYPE_CHECK_CLASS_TYPE ((klass),                                   \
                             HS_TYPE_PANEL))
#define HS_GET_PANEL_CLASS(obj)                                         \
   (G_TYPE_INSTANCE_GET_CLASS ((obj),                                   \
                               HS_TYPE_PANEL,                           \
                               HsPanelClass))

typedef struct _HsPanel      HsPanel;
typedef struct _HsPanelClass HsPanelClass;
typedef struct _HsPanelPrivate HsPanelPrivate;

struct _HsPanelClass
{
    GtkWindowClass parent_class;
};

struct _HsPanel
{
    GtkWindow parent;

    /*< private >*/
    HsPanelPrivate *priv;
};

GType        hs_panel_get_type      (void);
HsPanel     *hs_panel_new           (void);
void         hs_panel_show          (void);
void         hs_panel_add           (GtkWidget      *widget,
                                     GtkPositionType where);
GdkRectangle hs_panel_get_rectangle (void);

G_END_DECLS

#endif /* __HS_PANEL_H__ */

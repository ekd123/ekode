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
#ifndef __HS_LAUNCHER_BUTTON_H__
#define __HS_LAUNCHER_BUTTON_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define HS_TYPE_LAUNCHER_BUTTON                                         \
   (hs_launcher_button_get_type())
#define HS_LAUNCHER_BUTTON(obj)                                         \
   (G_TYPE_CHECK_INSTANCE_CAST ((obj),                                  \
                                HS_TYPE_LAUNCHER_BUTTON,                \
                                HsLauncherButton))
#define HS_LAUNCHER_BUTTON_CLASS(klass)                                 \
   (G_TYPE_CHECK_CLASS_CAST ((klass),                                   \
                             HS_TYPE_LAUNCHER_BUTTON,                   \
                             HsLauncherButtonClass))
#define HS_IS_LAUNCHER_BUTTON(obj)                                      \
   (G_TYPE_CHECK_INSTANCE_TYPE ((obj),                                  \
                                HS_TYPE_LAUNCHER_BUTTON))
#define HS_IS_LAUNCHER_BUTTON_CLASS(klass)                              \
   (G_TYPE_CHECK_CLASS_TYPE ((klass),                                   \
                             HS_TYPE_LAUNCHER_BUTTON))
#define HS_GET_LAUNCHER_BUTTON_CLASS(obj)                               \
   (G_TYPE_INSTANCE_GET_CLASS ((obj),                                   \
                               HS_TYPE_LAUNCHER_BUTTON,                 \
                               HsLauncherButtonClass))

typedef struct _HsLauncherButton        HsLauncherButton;
typedef struct _HsLauncherButtonClass   HsLauncherButtonClass;
typedef struct _HsLauncherButtonPrivate HsLauncherButtonPrivate;

struct _HsLauncherButtonClass
{
    GtkButtonClass parent_class;

    void (*error_launching)(HsLauncherButton *button, const gchar *errmsg);
    void (*launched)(HsLauncherButton *button);
};

struct _HsLauncherButton
{
    GtkButton parent;

    /*< private >*/
    HsLauncherButtonPrivate *priv;
};

GType             hs_launcher_button_get_type    (void);
GtkWidget *       hs_launcher_button_new         (void);
GtkWidget *       hs_launcher_button_new_full    (gboolean          launch,
                                                  GDesktopAppInfo  *appinfo);
GtkWidget *       hs_launcher_button_new_with_appinfo
                                                 (GDesktopAppInfo  *appinfo);
void              hs_launcher_button_set_appinfo (HsLauncherButton *button,
                                                  GDesktopAppInfo  *appinfo);
GDesktopAppInfo * hs_launcher_button_get_appinfo (HsLauncherButton *button);
void              hs_launcher_button_set_launch  (HsLauncherButton *button,
                                                  gboolean          launch);
gboolean          hs_launcher_button_get_launch  (HsLauncherButton *button);

G_END_DECLS

#endif /* __HS_LAUNCHER_BUTTON_H__ */

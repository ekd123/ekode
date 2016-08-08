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
#ifndef __HS_DATE_H__
#define __HS_DATE_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define HS_TYPE_DATE                                                    \
   (hs_date_get_type())
#define HS_DATE(obj)                                                    \
   (G_TYPE_CHECK_INSTANCE_CAST ((obj),                                  \
                                HS_TYPE_DATE,                           \
                                HsDate))
#define HS_DATE_CLASS(klass)                                            \
   (G_TYPE_CHECK_CLASS_CAST ((klass),                                   \
                             HS_TYPE_DATE,                              \
                             HsDateClass))
#define HS_IS_DATE(obj)                                                 \
   (G_TYPE_CHECK_INSTANCE_TYPE ((obj),                                  \
                                HS_TYPE_DATE))
#define HS_IS_DATE_CLASS(klass)                                         \
   (G_TYPE_CHECK_CLASS_TYPE ((klass),                                   \
                             HS_TYPE_DATE))
#define HS_GET_DATE_CLASS(obj)                                          \
   (G_TYPE_INSTANCE_GET_CLASS ((obj),                                   \
                               HS_TYPE_DATE,                            \
                               HsDateClass))

typedef struct _HsDate      HsDate;
typedef struct _HsDateClass HsDateClass;
typedef struct _HsDatePrivate HsDatePrivate;

struct _HsDateClass
{
    GtkLabelClass parent_class;
};

struct _HsDate
{
    GtkLabel parent;

    /*< private >*/
    HsDatePrivate *priv;
};

GType      hs_date_get_type (void);
GtkWidget *hs_date_new      (void);

G_END_DECLS

#endif /* __HS_DATE_H__ */

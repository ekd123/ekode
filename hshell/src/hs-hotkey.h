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
#ifndef __HS_HOTKEY_H__
#define __HS_HOTKEY_H__

#include <glib-object.h>

G_BEGIN_DECLS

#define HS_TYPE_HOTKEY                                                  \
   (hs_hotkey_get_type())
#define HS_HOTKEY(obj)                                                  \
   (G_TYPE_CHECK_INSTANCE_CAST ((obj),                                  \
                                HS_TYPE_HOTKEY,                         \
                                HsHotkey))
#define HS_HOTKEY_CLASS(klass)                                          \
   (G_TYPE_CHECK_CLASS_CAST ((klass),                                   \
                             HS_TYPE_HOTKEY,                            \
                             HsHotkeyClass))
#define HS_IS_HOTKEY(obj)                                               \
   (G_TYPE_CHECK_INSTANCE_TYPE ((obj),                                  \
                                HS_TYPE_HOTKEY))
#define HS_IS_HOTKEY_CLASS(klass)                                       \
   (G_TYPE_CHECK_CLASS_TYPE ((klass),                                   \
                             HS_TYPE_HOTKEY))
#define HS_GET_HOTKEY_CLASS(obj)                                        \
   (G_TYPE_INSTANCE_GET_CLASS ((obj),                                   \
                               HS_TYPE_HOTKEY,                          \
                               HsHotkeyClass))

/**
 * HsHotkeyErrorCode:
 * @HS_HOTKEY_ERROR_OK: no error
 * @HS_HOTKEY_ERROR_OBJECT_FAILED: fail to create the backend object
 * 
 * The enum type for error reporting.
 */
typedef enum {
    HS_HOTKEY_ERROR_OK = 0,
    HS_HOTKEY_ERROR_OBJECT_FAILED
} HsHotkeyErrorCode;

typedef struct _HsHotkey      HsHotkey;
typedef struct _HsHotkeyClass HsHotkeyClass;
typedef struct _HsHotkeyPrivate HsHotkeyPrivate;

struct _HsHotkeyClass
{
    GObjectClass parent_class;
};

struct _HsHotkey
{
    GObject parent;

    /*< private >*/
    HsHotkeyPrivate *priv;
};

GType      hs_hotkey_get_type (void);
HsHotkey * hs_hotkey_new      (void);
gboolean   hs_hotkey_bind     (const gchar *key_id,
                               const gchar *key,
                               GError     **error);
gboolean   hs_hotkey_unbind   (const gchar *key_id,
                               GError     **error);

G_END_DECLS

#endif /* __HS_HOTKEY_H__ */

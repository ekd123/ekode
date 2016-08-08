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
#include <glib/gi18n.h>
#include <gio/gdesktopappinfo.h>
#include "hs-autostart.h"

static void 
_load_dir (const gchar *path)
{
    const gchar *diritem;
    GDir *dir = NULL;
    dir = g_dir_open (path, 0, NULL);
    if (!dir)
        return;
    while ((diritem = g_dir_read_name (dir)))
    {
        if (g_str_has_suffix (diritem, ".desktop"))
        {
            GError *error = NULL;
            gchar *tmp;
            const gchar *cmd;
            tmp = g_strdup_printf ("%s/%s", path, diritem);
            GDesktopAppInfo *info = g_desktop_app_info_new (diritem);
            g_free(tmp);
            cmd = g_app_info_get_commandline (G_APP_INFO (info));
            if (cmd)
                g_spawn_command_line_async (cmd, &error);
            if (error)
            {
                g_message (_("%s couldn't run: %s"), diritem, error->message);
                g_error_free (error);
            }
            g_object_unref (info);
        }
    }
    g_dir_close (dir);
}

/* run auto-start programs described in XDG_CONF_DIRS/autostart */
void
hs_autostart_init (void)
{
    const gchar *user_conf = g_get_user_config_dir ();
    const gchar *const *sys_conf = g_get_system_config_dirs ();
    gchar *tmp;

    /* open user */
    tmp = g_strdup_printf ("%s/autostart", user_conf);
    _load_dir (tmp);
    g_free (tmp);

    /* open sys */
    for (gint i = 0; i < g_strv_length ((gchar **)sys_conf); i++)
    {
        tmp = g_strdup_printf ("%s/autostart", sys_conf[i]);
        _load_dir (tmp);
        g_free (tmp);
    }
}

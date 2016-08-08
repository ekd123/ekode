/* this file is part of RatDB */
/**
 * Copyright (c) 2011,2012 Mike Manilone
 * All rights reserved.
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
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
#ifndef __RATDB_SETTINGS_H__
#define __RATDB_SETTINGS_H__


#include <glib-object.h>

G_BEGIN_DECLS

#ifdef G_OS_UNIX
# define RATDB_SETTINGS_FILE           "/etc/ratdb.conf"
#endif

#ifdef G_OS_WIN32
# deinfe RATDB_SETTINGS_FILE           "C:\\ratdb.ini"
#endif

/**
 * RATDB_SETTINGS_SCHEMA:
 * the name of the schema for RatDB.
 */
#define RATDB_SETTINGS_SCHEMA          "org.ratdb"
/**
 * RATDB_SETTINGS_PATH:
 * the path of the schema of the RatDB.
 */
#define RATDB_SETTINGS_PATH            "/org/ratdb/"

/**
 * RATDB_DAEMON_CFG_SOCKET_PATH:
 * Configuration key of #RatdbDaemon.
 * 
 * Where should the socket file be in.
 */
#define RATDB_DAEMON_CFG_SOCKET_PATH "daemon-socket-path"

#define RATDB_TYPE_SETTINGS            (ratdb_settings_get_type ())
#define RATDB_SETTINGS(o)              (G_TYPE_CHECK_INSTANCE_CAST ((o), RATDB_TYPE_SETTINGS, RatdbSettings))
#define RATDB_SETTINGS_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), RATDB_TYPE_SETTINGS, RatdbSettingsClass))
#define RATDB_IS_SETTINGS(o)           (G_TYPE_CHECK_INSTANCE_TYPE ((o), RATDB_TYPE_SETTINGS))
#define RATDB_IS_SETTINGS_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), RATDB_TYPE_SETTINGS))
#define RATDB_GET_SETTINGS_CLASS(o)    (G_TYPE_INSTANCE_GET_CLASS ((o), RATDB_TYPE_SETTINGS, RatdbSettingsClass))

typedef struct _RatdbSettings        RatdbSettings;
typedef struct _RatdbSettingsClass   RatdbSettingsClass;

struct _RatdbSettings
{
    GSettings parent;
    
    /*< public >*/
    GSettings        *settings;
    GSettingsBackend *backend;
};

struct _RatdbSettingsClass
{
    GSettingsClass parent_class;
};

GType          ratdb_settings_get_type (void);
RatdbSettings *ratdb_settings_new (void);

G_END_DECLS

#endif /* __RATDB_SETTINGS_H__ */

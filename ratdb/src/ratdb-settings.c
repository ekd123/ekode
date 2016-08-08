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

/**
 * SECTION: ratdbsettings
 * @Title: RatdbSettings
 * @Short_description: Settings Reader
 * @See_also: #GSettings
 * 
 * Read user's settings.
 */

#include <glib.h>
#include <gio/gio.h>

/* for backends */
#define G_SETTINGS_ENABLE_BACKEND
#include <gio/gsettingsbackend.h>

#include "ratdb-settings.h"

G_DEFINE_TYPE (RatdbSettings, ratdb_settings, G_TYPE_OBJECT)

static void
ratdb_settings_dispose (GObject *object)
{
    G_OBJECT_CLASS (ratdb_settings_parent_class)->dispose (object);
}

static void 
ratdb_settings_finalize (GObject *object)
{
    G_OBJECT_CLASS (ratdb_settings_parent_class)->finalize (object);
}

static void
ratdb_settings_init (RatdbSettings *settings)
{
}

static void
ratdb_settings_class_init (RatdbSettingsClass *klass)
{
    G_OBJECT_CLASS (klass)->dispose = ratdb_settings_dispose;
    G_OBJECT_CLASS (klass)->finalize = ratdb_settings_finalize;
}

RatdbSettings *
ratdb_settings_new (void)
{
    RatdbSettings *object = NULL;
    
    object = g_object_new (RATDB_TYPE_SETTINGS, NULL);
    
    object->backend = g_keyfile_settings_backend_new (RATDB_SETTINGS_FILE,
                                                      RATDB_SETTINGS_PATH,
                                                      NULL);
    object->settings = g_settings_new_with_backend (RATDB_SETTINGS_SCHEMA,
                                                    object->backend);
    
    return object;
}



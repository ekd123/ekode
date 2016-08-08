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
#include "hs-dbus-handler.h"
#include "hs-dbus-proxy.h"

void 
hs_dbus_serve (void)
{
    GMainLoop *loop;
    guint id;
    g_type_init ();
    loop = g_main_loop_new (NULL, FALSE);
    id = g_bus_own_name (G_BUS_TYPE_SESSION,
                         "org.home.hsession.Manager",
                         G_BUS_NAME_OWNER_FLAGS_NONE,
                         hs_dbus_handler_bus_acquired,
                         NULL /* hs_dbus_handler_name_acquired */,
                         hs_dbus_handler_name_lost,
                         loop,
                         NULL);
    g_main_loop_run (loop);
    g_bus_unown_name (id);
    g_main_loop_unref (loop);
}

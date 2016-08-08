/* this file is part of RatDB */
/**
 * Copyright (c) 2011, 2012, 2013 Mike Manilone
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
#include <config.h>
#include <glib.h>
#include <gio/gio.h>

#include <glib/gi18n.h>

#include "ratdb-server.h"

#ifdef G_OS_WIN32
# error "Sorry. RatDB still needs work to port it to MS Windows. Maybe it won't work in MS Windows, forever."
#endif

int
main (int argc,
      char *argv[])
{
    RatdbServer *server;
    GError *reg_err = NULL;/* when registration fails */

    /* initialize i18n */
    bindtextdomain (GETTEXT_PACKAGE, LOCALEDIR);
    bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
    textdomain (GETTEXT_PACKAGE);

    /* begin to serve. */
    g_print (_("Welcome to RatDB. Please report bugs to our issues board\n"));

    server = ratdb_server_new ();
    if (!g_application_register (G_APPLICATION (server), NULL, &reg_err))
    {
        g_critical (_("Failed to register: %s.\n"
                      "Please check whether 'ratdbd' is running right now.\n"),
                    reg_err->message);
        g_error_free (reg_err);
        return 0;
    }

    return g_application_run (G_APPLICATION (server), argc, argv);
}


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

#include <glib.h>
#include "hs-misc.h"

/**
 * SECTION: hs-misc
 * @Title: HsMisc
 * @Short_description: Some misc functions
 * @Include: hs-misc.h
 * 
 * Some useful functions, but not belong to any other parts.
 */

/**
 * hs_misc_in_strv:
 * @strv: the array of strings
 * @str: the string
 * 
 * Check if @str is in @strv. This function won't change @strv.
 * 
 * Returns: whether @str is in @strv
 */
gboolean 
hs_misc_in_strv (gchar **      strv,
                 const gchar * str)
{
    if (!strv)
        return FALSE;
    for (guint i = 0; i < g_strv_length ((gchar **)strv); i++)
    {
        if (!g_strcmp0 (strv[i], str))
            return TRUE;
    }
    return FALSE;
}


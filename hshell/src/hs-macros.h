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
#ifndef __HS_MACROS_H__
#define __HS_MACROS_H__

/**
 * SECTION: hs-macros
 * @Title: HsMacros
 * @Short_description: Macros made development easier
 * @Include: hs-macros.h
 * 
 * There are some macros which make development easier.
 * 
 * These macros may be slow. So you have to enable the optimization 
 * option of your compiler, which is enabled by default.
 */

/**
 * g_lish_iforeach:
 * @list: a #GList
 * 
 * Foreach @list, inside the code block. The position is named "i".
 */
#define g_list_iforeach(list) for(guint i=0; i<g_list_length((GList*)list);i++)

/**
 * g_lish_jforeach:
 * @list: a #GList
 * 
 * Foreach @list, inside the code block. The position is named "j".
 */
#define g_list_jforeach(list) for(guint j=0; j<g_list_length((GList*)list);j++)

/**
 * g_slish_iforeach:
 * @list: a #GSList
 * 
 * Foreach @list, inside the code block. The position is named "i".
 */
#define g_slist_iforeach(list) for(guint i=0; i<g_slist_length(list);i++)

/**
 * g_slish_jforeach:
 * @list: a #GSList
 * 
 * Foreach @list, inside the code block. The position is named "j".
 */
#define g_slist_jforeach(list) for(guint j=0; j<g_slist_length(list);j++)

/**
 * g_strv_iforeach:
 * @strv: a NULL-terminated array of string
 * 
 * Foreach @strv, inside the code block. The position is named "i".
 */
#define g_strv_iforeach(strv) for(guint i=0; i<g_strv_length((gchar**)strv);i++)

/**
 * g_strv_jforeach:
 * @strv: a NULL-terminated array of string
 * 
 * Foreach @strv, inside the code block. The position is named "j".
 */
#define g_strv_jforeach(strv) for(guint j=0; j<g_strv_length((gchar**)strv);j++)

/**
 * HS_FINAL_FREE_PRIVVAR:
 * @v: name of the private variable
 * 
 * Check whether is NULL, g_free() it and set it to %NULL.
 */
#define HS_FINAL_FREE_PRIVVAR(v)                            \
    if (self->priv->v)                                      \
    { g_free (self->priv->v); self->priv->v = NULL; }       \

/**
 * HS_FINAL_FREE_PRIVOBJ:
 * @v: name of the private object
 * 
 * Check whether is NULL, g_object_unref() it and set it to %NULL.
 */
#define HS_FINAL_FREE_PRIVOBJ(v)                                    \
    if (self->priv->v)                                              \
    { g_object_unref (self->priv->v); self->priv->v = NULL; }       \

#endif




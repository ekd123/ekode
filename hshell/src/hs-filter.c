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
#include <string.h>
#include <glib.h>
#include "hs-macros.h"
#include "hs-filter.h"

/**
 * SECTION: hs-filter
 * @Title: HsFilter
 * @Short_description: Filter information structure
 * 
 * #HsFilter is a class stores filter-related information.
 * 
 * Note that info stored in #HsFilter is not parsed by #HsFilter
 * by the object uses #HsFilter.
 * 
 * Format is 
 * * name1 ||| value1\:/value2\:/value3
 * * name2 ||| value1\:/value2\:/value3
 * 
 * So there can't be any '\:/' in your value.
 */

struct _HsFilter
{
    gulong refcount;

    GHashTable *tbl;
};

static void
_destroy (HsFilter *f)
{
    g_hash_table_destroy (f->tbl);
    g_free (f);
}

HsFilter *
hs_filter_new (void)
{
    HsFilter *f;

    f = g_malloc (sizeof (HsFilter));
    f->tbl = g_hash_table_new (g_str_hash, g_str_equal);
    f->refcount = 1;

    return f;
}

HsFilter *
hs_filter_ref (HsFilter *f)
{
    f->refcount ++;
    return f;
}

void 
hs_filter_unref (HsFilter *f)
{
    f->refcount --;
    if (f->refcount == 0)
        _destroy (f);
}

/**
 * hs_filter_clear:
 * @f: a #HsFilter
 * @name: the name of info you wanna delete
 * 
 * Delete the value of @name.
 */
void 
hs_filter_clear (HsFilter    *f,
                 const gchar *name)
{
    g_hash_table_remove (f->tbl, name);
}

gboolean 
hs_filter_has (HsFilter *f, 
               const gchar *name)
{
    return g_hash_table_contains (f->tbl, name);
}

/**
 * hs_filter_add:
 * @f: a #HsFilter
 * @name: the name 
 * @src: the value
 * 
 * Add @src into @name stored in @f. There can't be 
 * '\:/' in your value.
 * 
 * Returns: whether this succeeded
 */
gboolean 
hs_filter_add (HsFilter    *f,
               const gchar *name,
               const gchar *src)
{
    GString *value = g_string_new ("");

    if (!name || !src)
        return FALSE;

    /* if there's one here... */
    if (hs_filter_has (f, name))
    {
        /* add original value to the buffer */
        g_string_append (value, (gchar *)g_hash_table_lookup (f->tbl, name));
    }
    else 
        goto add_value;

    /* add the separator */
    g_string_append (value, "\\:/");

  add_value:
    g_string_append (value, src);
    g_hash_table_insert (f->tbl, (gpointer)name, (gpointer)value->str);
    g_string_free (value, TRUE);

    return TRUE;
}

gchar **
hs_filter_get (HsFilter *f,
               const gchar *name)
{
    gchar *value = (gchar *)g_hash_table_lookup (f->tbl, name);
    if (!value)
        return NULL;
    return g_strsplit (value, "\\:/", -1);
}

void
hs_filter_concat (HsFilter *f1,
                  const HsFilter *f2)
{
    GList *keys;

    keys = g_hash_table_get_keys (f2->tbl);

    g_list_iforeach (keys)
    {
        hs_filter_add (f1, g_list_nth_data (keys, i), (gchar *)g_hash_table_lookup (f2->tbl, g_list_nth_data (keys, i)));
    }
}

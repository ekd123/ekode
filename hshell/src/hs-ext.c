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
#include <gio/gio.h>
#include "hs-ext.h"

/**
 * SECTION: hs-ext
 * @Title: HsExt
 * @Short_description: Extensions subsystem of HShell
 * @Include: hs-ext.h
 * @See_also: hs-ext-manager
 * 
 * The extensions subsystem of HShell uses Lua as its extension language. 
 * It initializes Lua.
 * 
 * This class provides a GObject interface to Lua. 
 */

struct _HsExtPrivate
{
    lua_State *vm;
};

static void hs_ext_dispose            (GObject        *object);
static void hs_ext_finalize           (GObject        *object);
static void g_initable_interface_init (GInitableIface *interface);

G_DEFINE_TYPE_WITH_CODE (HsExt, hs_ext, G_TYPE_OBJECT, 
                         G_IMPLEMENT_INTERFACE (G_TYPE_INITABLE, 
                                                g_initable_interface_init))
G_LOCK_DEFINE_STATIC (stack);

static GObject *
hs_ext_constructor (GType type,
                    guint n_construct_params,
                    GObjectConstructParam *construct_params)
{
    static GObject *_main_instance = NULL;
    if (_main_instance != NULL)
        return _main_instance;
    _main_instance = G_OBJECT_CLASS (hs_ext_parent_class)->constructor (type, n_construct_params, construct_params);
    g_object_add_weak_pointer (_main_instance, (gpointer)&_main_instance);
    return _main_instance;
}

static void
hs_ext_class_init (HsExtClass *klass)
{
    GObjectClass *gobject_class = (GObjectClass *)klass;

    g_type_class_add_private (gobject_class, sizeof (HsExtPrivate));

    gobject_class->dispose = hs_ext_dispose;
    gobject_class->finalize = hs_ext_finalize;
    gobject_class->constructor = hs_ext_constructor;
}

/**
   GInitable, failable init
 */
static gboolean
hs_ext_init_failable (GInitable    *initable,
                      GCancellable *cancellable,
                      GError      **error)
{
    HsExt *self = HS_EXT (initable);

    g_debug (_("HShell Extension System is initializing Lua support."));
    self->priv = G_TYPE_INSTANCE_GET_PRIVATE (self,
                                              HS_TYPE_EXT,
                                              HsExtPrivate);
    self->priv->vm = lua_open ();

    if (!self->priv->vm)
    {
        g_debug (_("lua_open() returned NULL. ExtSys can't be started."));
        return FALSE;
    }

    luaL_openlibs (self->priv->vm);

    return TRUE;
}

static void 
hs_ext_init (HsExt *self)
{
    g_debug (_("HShell Extension System is starting."));
}

static void
hs_ext_dispose (GObject *object)
{
    G_OBJECT_CLASS (hs_ext_parent_class)->dispose (object);
}

static void
hs_ext_finalize (GObject *object)
{
    HsExt *self = (HsExt *)object;

    if (self->priv->vm)
    {
        lua_close (self->priv->vm);
        self->priv->vm = NULL;
    }

    g_signal_handlers_destroy (object);
    G_OBJECT_CLASS (hs_ext_parent_class)->finalize (object);
}

static void 
g_initable_interface_init (GInitableIface *interface)
{
    interface->init = hs_ext_init_failable;
}

/**
 * hs_ext_new:
 * 
 * Try to initialize #HsExt.
 * 
 * Returns: the pointer of #HsExt if initalized successfully
 */
HsExt *
hs_ext_new (void)
{
    static HsExt * instance = NULL;
    if (!instance)
        instance = HS_EXT (g_initable_new (HS_TYPE_EXT, NULL, NULL, NULL));
    return instance;
}

/**
 * hs_ext_run_file:
 * @file: the path to @file
 * 
 * Run an extension in the VM
 * 
 * Returns: %TRUE on success.
 */
gboolean
hs_ext_run_file (const gchar *file)
{
    HsExt *ext = hs_ext_new ();

    g_return_val_if_fail (HS_IS_EXT (ext), FALSE);
    if (luaL_dofile (ext->priv->vm, file))
        return FALSE;
    return TRUE;
}

/**
   load var into stack by lua_getglobal

   not public because, user may make mistakes on stack
 */
static void
_load_var_into_stack (const gchar *varname)
{
    HsExt *ext = hs_ext_new ();

    lua_getglobal (ext->priv->vm, varname);
}

/**
 * hs_ext_global_get_int:
 * @varname: name of the variable
 * 
 * Get the value of a global integer variable.
 * 
 * Returns: value of the integer, or 0 if any errors
 */
gint
hs_ext_global_get_int (const gchar *varname)
{
    HsExt *ext = hs_ext_new ();
    gint retval;

    G_LOCK (stack);
    _load_var_into_stack (varname);
    retval = lua_tointeger (ext->priv->vm, -1);
    lua_pop (ext->priv->vm, 1);
    G_UNLOCK (stack);
    return retval;
}

/**
 * hs_ext_global_get_number:
 * @varname: name of the variable
 * @number: where to store
 * 
 * Get the value of a global number variable.
 * 
 * Returns: value of the number, or 0.0 if any errors
 */
double
hs_ext_global_get_number (const gchar *varname)
{
    HsExt *ext = hs_ext_new ();
    gdouble retval;

    G_LOCK (stack);
    _load_var_into_stack (varname);
    retval = lua_tonumber (ext->priv->vm, -1);
    lua_pop (ext->priv->vm, 1);
    G_UNLOCK (stack);
    return retval;
}

/**
 * hs_ext_global_get_str:
 * @varname: name of the variable
 * 
 * Get the value of a global string variable.
 * 
 * Returns: (transfer-full): a newly-allocated string.
 * Or %NULL if any errors.
 */
gchar *
hs_ext_global_get_str (const gchar *varname)
{
    HsExt *ext = hs_ext_new ();
    const gchar *str;
    gchar *retval;

    G_LOCK (stack);
    _load_var_into_stack (varname);
    str = lua_tostring (ext->priv->vm, -1);
    retval = g_strdup (str);
    lua_pop (ext->priv->vm, 1);
    G_UNLOCK (stack);
    return retval;
}

/**
 * hs_ext_provide:
 * @func: pointer to the function you wish to add
 * @alias: alias used in Lua scripts
 * 
 * Add a function into the Lua environment.
 */
void
hs_ext_provide (gpointer func, 
                const gchar *alias)
{
    HsExt *ext = hs_ext_new ();
    lua_pushcfunction (ext->priv->vm, (lua_CFunction)func);
    lua_setglobal (ext->priv->vm, alias);
}

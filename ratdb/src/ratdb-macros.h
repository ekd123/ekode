/* this file is part of RatDB */
/**
 * Copyright (c) 2012 Mike Manilone
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
 * SECTION: RatdbMacros
 * @Short_description: Common Macros for Easier GObject use.
 * @Title: RatdbMacros
 * @Include: ratdbmacros.h
 * 
 * Some useful common macros for easier GObject use.
 * Only useful for specific domain.
 */

#ifndef __RATDB_MACROS_H__
#define __RATDB_MACROS_H__

#include <stdarg.h>

/**
 * RATDB_DEFINE_VARIABLE_IFACE_ITEM:
 * @type: return type
 * @name: item name
 * @class: belong to which interface
 * 
 * Generate an item for GObject interface.
 */
#define RATDB_DEFINE_VARIABLE_IFACE_ITEM(type, name, class)         \
    type (*name) (class *self, gpointer first_para, va_list args)

/**
 * RATDB_DEFINE_VARIABLE_FUNC_IFACE:
 * @type: return type
 * @func: the func name
 * @class: which class's interface
 * 
 * Generate two interfaces for users' use
 */
#define RATDB_DEFINE_VARIABLE_FUNC_IFACE(type, func, class)             \
    type func (class *self, gpointer first_para, ...);                  \
    type func##_valist (class *self, gpointer first_para, va_list args);

/**
 * RATDB_DEFINE_VARIABLE_FUNC:
 * @type: return type
 * @func: the func name
 * @class: which class's method
 * 
 * Generate a function with variable arguments that calls the 
 * _valist version of the set of functions.
 */
#define RATDB_DEFINE_VARIABLE_FUNC(type, func, class)    \
    type func (class *self, gpointer first_para, ...)    \
    {                                                    \
        type ret; va_list args;                          \
        va_start (args, first_para);                     \
        ret = func ## _valist (self, first_para, args);  \
        va_end (args);                                   \
        return ret;                                      \
    }

/**
 * RATDB_FIND_REAL_FILENAME
 * @filename: the filename with suffix
 * @suffix: the suffix
 * 
 * Get the filename without suffix
 * "A.hi"-> "A"
 * "A.hi.second" -> "A.hi"
 * 
 * NOTICE: this causes crashes when @filename doesn't have a suffix
 * 
 * Returns: (transfer full): newly-allocated string
 */
#define RATDB_FIND_REAL_FILENAME(filename)  \
    (g_strndup(filename, g_utf8_strrchr(filename, -1, '.')-filename))

/**
 * RATDB_CONNECT_DBUS_IFACE
 * @ifacename: name of the interface
 * @prefix: prefix
 * @userdata: user_data
 * 
 * RATDB_CONNECT_DBUS_IFACE(ok, ratdb_dbus_handler) => g_signal_connect (skeleton, "handle_ok"
 * G_CALLBACK(ratdb_dbus_handler_ok, userdata);
 */ 
#define RATDB_CONNECT_DBUS_IFACE(ifacename,prefix,userdata) \
    g_signal_connect (skeleton, "handle_" # ifacename, G_CALLBACK(prefix ## _ ## ifacename), userdata)

#endif /* __RATDB_MACROS_H__ */

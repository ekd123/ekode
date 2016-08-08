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

#include <gdk/gdk.h>
#include <gdk/gdkx.h>

#include <gtk/gtk.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>

#include "hs-x.h"

/**
 * SECTION: hs-x
 * @Title: HsX
 * @Short_description: X-related functions
 * 
 * There are X-related functions.
 */

/**
 * hs_x_set_hint_strut:
 * @window: a #GdkWindow
 * @left: left
 * @right: right
 * @top: top
 * @bottom: bottom
 *
 * Set _NET_WM_HINT_STRUT hint for @window.
 * Note that you must call gtk_widget_show() before calling this, or you'll get
 * a SIGSEGV.
 */ 
void
hs_x_set_hint_strut (GdkWindow *window,
                     int        left,
                     int        right,
                     int        top,
                     int        bottom)
{
    GdkAtom atom;

    long vals[4];
    vals[0] = left;
    vals[1] = right;
    vals[2] = top;
    vals[3] = bottom;

    atom = gdk_atom_intern ("_NET_WM_STRUT", FALSE);

    gdk_property_change (window, atom, gdk_x11_xatom_to_atom (XA_CARDINAL), 
                         32, GDK_PROP_MODE_REPLACE, (guchar *)vals, 4);
}

/**
 * hs_x_set_window_strut:
 * @window: a #GtkWindow
 * 
 * Set the _NET_WM_HINT_STRUT property for @window. 
 * Reverse the space where there's %window.
 * 
 * As the documentation of hs_x_set_hint_strut() said, you mustn't call this 
 * before your window is realized.
 */
void
hs_x_set_window_strut (GtkWindow *window)
{
    GdkWindow *gdk_win;
    GdkRectangle win_rect;

    g_return_if_fail (GTK_IS_WINDOW (window));
    gdk_win = gtk_widget_get_window (GTK_WIDGET (window));
    gdk_window_get_geometry (gdk_win, NULL, NULL, 
                             NULL, &win_rect.height);
    hs_x_set_hint_strut (gdk_win, 0, 0, win_rect.height, 0);
}

/**
 * hs_x_get_monitor_geometry:
 * @which: which monitor's info. -1 if you want primary monitor
 * @geometry: where to store
 * 
 * Get the monitor's geometry.
 *
 * Returns: %TRUE on success
 */
gboolean
hs_x_get_monitor_geometry (gint          which,
                           GdkRectangle *geometry)
{
    GdkScreen *screen;

    screen = gdk_screen_get_default ();
    if (which == -1)
        which = gdk_screen_get_primary_monitor (screen);
    gdk_screen_get_monitor_geometry (screen, which, geometry);

    if ((*geometry).width <= 0 || (*geometry).height <= 0)
        return FALSE;

    return TRUE;
}

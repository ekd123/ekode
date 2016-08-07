#include <glib.h>
#include <gdk/gdk.h>

gint main (gint argc, gchar *argv[])
{
    gdk_init (&argc, &argv);
    GdkKeymap *keymap = gdk_keymap_get_default ();
    g_print ("%s\n", (gdk_keymap_get_caps_lock_state (keymap))?"on":"off");
    return 0;
}

#include <unistd.h>
#include <locale.h>
#include <glib.h>
#include <gio/gio.h>
#include <gio/gunixinputstream.h>

gint 
main (void)
{
    g_type_init ();
    setlocale (LC_ALL, "");

    GInputStream *is = g_unix_input_stream_new (STDIN_FILENO, FALSE);
    GDataInputStream *dis = g_data_input_stream_new (is);
    gchar *line = NULL;

    while ((line = g_data_input_stream_read_line (dis, NULL, NULL, NULL)))
    {
        g_print ("%s\n", line);
        g_free (line);
    }
    return 0;
}

#include <gio/gio.h>

#include "data.h"

gint 
main (gint argc, 
      gchar *argv[])
{
    g_type_init ();

    data_register_resource ();

    GError *error = NULL;

    GResource *res = data_get_resource ();
    GInputStream *input = g_resource_open_stream (res, "/org/example/hello", G_RESOURCE_LOOKUP_FLAGS_NONE, &error);
    if (!input)
    {
        g_error ("Error %s", error->message);
        g_error_free (error);
        return 0;
    }
    GDataInputStream *dis = g_data_input_stream_new (input);

    gchar *line = NULL;
    while ((line = g_data_input_stream_read_line (dis, NULL, NULL, NULL)))
    {
        g_print ("%s\n", line);
    }

    data_unregister_resource ();

    return 0;
}

#include <locale.h>
#include <gio/gio.h>

gint 
main (gint argc,
      gchar *argv[])
{
    if (argc < 2)
        g_error ("need a path or an URI at least");

    g_type_init ();
    setlocale (LC_ALL, "");

    GError *error = NULL;
    GFile *file = g_file_new_for_commandline_arg (argv[1]);
    GInputStream *is = G_INPUT_STREAM (g_file_read (file, NULL, &error));
    if (error)
        g_error ("%s\n", error->message);
    GDataInputStream *dis = g_data_input_stream_new (is);
    gchar *line = NULL;

    while ((line = g_data_input_stream_read_line (dis, NULL, NULL, &error)))
    {
        g_print ("%s\n", line);
        g_free (line);
    }
    if (error)
        g_error ("%s\n", error->message);

    return 0;
}


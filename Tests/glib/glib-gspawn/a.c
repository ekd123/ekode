/*
  Show current time in a GtkLabel

  Key feature: Just obtain what 'date' says. It calls g_spawn_* of GLib for portability.
  Reference: GLib Reference Manual - GLib Utilities - Spawning Processes
 */

#include <gtk/gtk.h>

gint 
main (gint argc,
      gchar *argv[])
{
    gchar *output = NULL;
    GError *error = NULL;
    GtkWidget *window, *label;

    gtk_init (&argc, &argv);

    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    g_signal_connect (window, "delete-event", 
                      G_CALLBACK(gtk_main_quit), NULL);

    g_spawn_command_line_sync ("date", &output, NULL, NULL, &error);
    if (error)
    {
        g_critical ("%s\n", error->message);
        g_error_free (error);
        return -1;
    }

    label = gtk_label_new (output);
    g_free (output);

    gtk_container_add (GTK_CONTAINER (window), label);

    gtk_widget_show_all (window);
    gtk_main ();

    return 0;
}

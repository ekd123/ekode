/* this code shows you how to make a window in the dark variant */

#include <gtk/gtk.h>

gint 
main (gint argc,
      gchar * argv[])
{
    GtkSettings *settings;
    GtkWidget *window;

    gtk_init (&argc, &argv);

    /* create window */
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (window), "hello world");

    /* reset theme */
    settings = gtk_settings_get_default ();

    g_object_set (settings, "gtk-application-prefer-dark-theme", TRUE, NULL);

    /* show window */
    g_signal_connect (window, "delete-event", G_CALLBACK (gtk_main_quit),
                      NULL);
    gtk_widget_show_all (window);
    gtk_main ();
    return 0;
}

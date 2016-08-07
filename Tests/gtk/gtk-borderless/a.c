#include <gtk/gtk.h>
#include "csd.h"

int main (int argc, char *argv[])
{
    gtk_init (&argc, &argv);

    GtkWidget *window = gtk_csd_window_new (0);
    gtk_window_set_title (GTK_WINDOW (window), "hello");
    gtk_window_csd_add_default_title_bar ();

    g_signal_connect (window, "delete-event", G_CALLBACK (gtk_main_quit), NULL);

    gtk_widget_show_all (window);
    gtk_main ();

    return 0;
}


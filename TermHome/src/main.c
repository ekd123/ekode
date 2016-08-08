#include <gtk/gtk.h>
#include <vte/vte.h>

gint 
main (gint argc,
      gchar *argv[])
{
    GtkWidget *win, *tty;
    gchar *chld_argv[] = {"/bin/sh", NULL};

    gtk_init (&argc, &argv);

    win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    tty = vte_terminal_new ();

    gtk_container_add (GTK_CONTAINER (win), tty);
    gtk_window_set_title (GTK_WINDOW (win), "Term Home");
    gtk_widget_show_all (win);

    vte_terminal_fork_command_full (VTE_TERMINAL (tty), VTE_PTY_DEFAULT, NULL,
                                    chld_argv, NULL, G_SPAWN_SEARCH_PATH, NULL, 
                                    NULL, NULL, NULL);
    g_signal_connect (win, "delete-event", G_CALLBACK (gtk_main_quit), NULL);
    g_signal_connect (tty, "child-exited", G_CALLBACK (gtk_main_quit), NULL);

    gtk_main ();
    return 0;
}

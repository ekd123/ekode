#include <signal.h>
#include <glib.h>

gboolean 
sig_handler (gpointer user_data)
{
    g_printf ("entered!\n");
}

int
main (int argc,
      char *argv[])
{
    GMainLoop *loop;
    guint event_id;
    
    event_id = g_unix_signal_add (SIGINT, sig_handler, NULL);
    
    loop = g_main_loop_new (NULL, FALSE);
    g_main_loop_run (loop);
}

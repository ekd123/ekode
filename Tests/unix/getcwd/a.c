#include <unistd.h>
#include <stdio.h>
#include <glib.h>

int
main (int argc,
      char *argv[])
{
    /* System Call */
    {
        char buf[1024+1];
        getcwd (buf, sizeof(buf));
        printf ("By system call: %s\n", buf);
    }
    
    /* GLib */
    {
        gchar *ptr;
        ptr = g_get_current_dir ();
        g_print ("By GLib: %s\n", ptr);
        g_free (ptr);
    }
    
    return 0;
}

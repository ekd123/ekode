#include <glib.h>
#include <gio/gio.h>
#include <libratdb/ratdb-client.h>

int main ()
{
    RatdbClient *client;
    gboolean     ret;
    
    g_type_init ();
    
    client =  ratdb_client_new ();
    ret = ratdb_client_connect (client, NULL);
    if (ret)
    {
        g_print ("Oh yeah~! Connected!\n");
    }
    else
    {
        g_error ("I'm sorry.. I can't connect to the daemon");
    }
    ret = ratdb_client_disconnect (client, NULL);
    if (ret)
    {
        g_print ("Good bye~!\n");
    }
    else
    {
        g_error ("failed to disconnect...");
    }
    g_object_unref (client); /* destroy */
    
    return 0;
}


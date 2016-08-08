/* in public domain, enjoy it! */
/* follows C89. */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <gio/gio.h>
#include <gio/gunixinputstream.h>
#include <libratdb/ratdb.h>

void help ()
{
    g_print ("List of commands: \n\
s\t\tsend a message \n\
q\t\tquit \n\
\n\
If a message comes, then I will print it.\n");
}

main ()
{
    RatdbClient *client;
    RatdbCommand *cmd;
    GSocket *socket;
    gint ch = 0;

    g_type_init ();

    client = ratdb_client_new ();
    if (!ratdb_client_connect (client, NULL))
    {
        g_error ("failed to connect!");
    }
    socket = ratdb_client_get_socket (client);
    cmd = ratdb_command_new (socket);
    g_object_unref (socket);

    while (ch != 'q')
    {
        g_print ("> ");
        ch = getchar ();
        switch (ch)
        {
            gchar line[1024+1];

            case 'h':
                help();
                break;
            case 's':
                fgets (line, sizeof (line), stdin);
                if (!ratdb_command_request (cmd, line))
                {
                    g_critical ("cannot send!");
                }
                else
                    g_print ("what're you going to send?\n");
                break;
            case 'q':
                break;
            case 'V':
                g_print ("this is a telnet-like program for testing the \
server. this is the first edition. Made by Mike Manilone.");
                break;
            default:
                g_print ("?\n");
                break;
        }
    }
    ratdb_client_disconnect (client, NULL);

    exit (0);
}


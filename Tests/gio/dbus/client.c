#include "ratdb-engine-dbus.h"

static void
on_notify_name_owner (GObject    *object,
                      GParamSpec *pspec,
                      gpointer    user_data)
{
    GDBusObjectManagerClient *manager = G_DBUS_OBJECT_MANAGER_CLIENT (object);
    gchar *name_owner;

    name_owner = g_dbus_object_manager_client_get_name_owner (manager);
    g_print ("name-owner: %s\n", name_owner);
    g_free (name_owner);
}

gint
main (gint argc, gchar *argv[])
{
    GDBusObjectManager *manager;
    GMainLoop *loop;
    GError *error;
    gchar *name_owner;

    g_type_init ();

    manager = NULL;
    loop = NULL;

    loop = g_main_loop_new (NULL, FALSE);

    error = NULL;
    manager = ratdb_dbus_object_manager_client_new_for_bus_sync (G_BUS_TYPE_SESSION,
                                                                 G_DBUS_OBJECT_MANAGER_CLIENT_FLAGS_NONE,
                                                                 "org.ratdb.ObjectManager",
                                                                 "/org/ratdb/Engine/Interface",
                                                                 NULL, /* GCancellable */
                                                                 &error);
    if (manager == NULL)
    {
        g_printerr ("Error getting object manager client: %s", error->message);
        g_error_free (error);
        goto out;
    }

    name_owner = g_dbus_object_manager_client_get_name_owner (G_DBUS_OBJECT_MANAGER_CLIENT (manager));
    g_print ("name-owner: %s\n", name_owner);
    g_free (name_owner);

    g_signal_connect (manager,
                      "notify::name-owner",
                      G_CALLBACK (on_notify_name_owner),
                      NULL);
    /* g_signal_connect (manager, */
    /*                   "object-added", */
    /*                   G_CALLBACK (on_object_added), */
    /*                   NULL); */
    /* g_signal_connect (manager, */
    /*                   "object-removed", */
    /*                   G_CALLBACK (on_object_removed), */
    /*                   NULL); */
    /* g_signal_connect (manager, */
    /*                   "interface-proxy-properties-changed", */
    /*                   G_CALLBACK (on_interface_proxy_properties_changed), */
    /*                   NULL); */

    g_main_loop_run (loop);

  out:
    if (manager != NULL)
        g_object_unref (manager);
    if (loop != NULL)
        g_main_loop_unref (loop);

    return 0;
}

#include "ratdb-engine-dbus.h"

static GDBusObjectManagerServer *manager = NULL;
static guint num_conn = 0;

static gboolean 
on_engine_request (RatdbDbusInterface *engine,
                   GDBusMethodInvocation    *invocation,
                   gchar                    *type,
                   gpointer                  user_data)
{
    g_dbus_method_invocation_return_dbus_error (invocation, 
                                                "org.ratdb.Engine.Interface.Errors.NotImplemented",
                                                "This function is not implemented");
    return TRUE;
}

static void
on_bus_acquired (GDBusConnection *connection,
                 const gchar     *name,
                 gpointer         user_data)
{
    g_print ("Acquired a message bus connection\n");

    manager = g_dbus_object_manager_server_new ("/org/ratdb/Engine/Clients");

    /* Create a new D-Bus object at the path /org/ratdb/Engine/Clients/xxx where xxx is the counter */
    RatdbDbusObjectSkeleton *object;
    object = ratdb_dbus_object_skeleton_new ("/org/ratdb/Engine/Clients/1");

    /* create new object for skeleton
       // @object takes its own refcount!
     */
    RatdbDbusInterface *interface;
    interface = ratdb_dbus_interface_skeleton_new ();
    ratdb_dbus_object_skeleton_set_interface (object, interface);
    g_object_unref (interface);

    /* handle methods */
    g_signal_connect (interface,
                      "handle-request",
                      G_CALLBACK (on_engine_request),
                      NULL); /* user data */

    /* Export the object (@manager takes its own reference to @object) */
    g_dbus_object_manager_server_export (manager, G_DBUS_OBJECT_SKELETON (object));
    g_object_unref (object);

    /* Export all objects */
    g_dbus_object_manager_server_set_connection (manager, connection);
}

static void
on_name_acquired (GDBusConnection *connection,
                  const gchar     *name,
                  gpointer         user_data)
{
    g_print ("Acquired the name %s\n", name);
}

static void
on_name_lost (GDBusConnection *connection,
              const gchar     *name,
              gpointer         user_data)
{
    g_print ("Lost the name %s\n", name);
}

gint
main (gint argc, gchar *argv[])
{
    g_type_init();

    GMainLoop *loop;
    guint id;

    loop = g_main_loop_new (NULL, FALSE);

    id = g_bus_own_name (G_BUS_TYPE_SESSION,
                         "org.ratdb.ObjectManager",
                         G_BUS_NAME_OWNER_FLAGS_ALLOW_REPLACEMENT |
                         G_BUS_NAME_OWNER_FLAGS_REPLACE,
                         on_bus_acquired,
                         on_name_acquired,
                         on_name_lost,
                         loop,
                         NULL);

    g_main_loop_run (loop);

    g_bus_unown_name (id);
    g_main_loop_unref (loop);

    return 0;
}

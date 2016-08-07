/*
  clang a.c `pkg-config --cflags --libs polkit-gobject-1`
  copied from http://hal.freedesktop.org/docs/polkit/polkit-apps.html
*/
#include <polkit/polkit.h>

static gboolean
on_tensec_timeout (gpointer user_data)
{
    GMainLoop *loop = user_data;
    g_print ("Ten seconds has passed. Now exiting.\n");
    g_main_loop_quit (loop);
    return FALSE;
}

static void
check_authorization_cb (PolkitAuthority *authority,
                        GAsyncResult    *res,
                        gpointer         user_data)
{
    GMainLoop *loop = user_data;
    PolkitAuthorizationResult *result;
    GError *error;

    error = NULL;
    result = polkit_authority_check_authorization_finish (authority, res, &error);
    if (error != NULL)
    {
        g_print ("Error checking authorization: %s\n", error->message);
        g_error_free (error);
    }
    else
    {
        const gchar *result_str;
        if (polkit_authorization_result_get_is_authorized (result))
        {
            result_str = "authorized";
        }
        else if (polkit_authorization_result_get_is_challenge (result))
        {
            result_str = "challenge";
        }
        else
        {
            result_str = "not authorized";
        }

        g_print ("Authorization result: %s\n", result_str);
    }

    g_print ("Authorization check has been cancelled and the dialog should now be hidden.\n"
             "This process will exit in ten seconds.\n");
    g_timeout_add (10000, on_tensec_timeout, loop);
}

static gboolean
do_cancel (GCancellable *cancellable)
{
    g_print ("Timer has expired; cancelling authorization check\n");
    g_cancellable_cancel (cancellable);
    return FALSE;
}

int
main (int argc, char *argv[])
{
    pid_t parent_pid;
    const gchar *action_id;
    GMainLoop *loop;
    PolkitSubject *subject;
    PolkitAuthority *authority;
    GCancellable *cancellable;

    g_type_init ();

    if (argc != 2)
    {
        g_printerr ("usage: %s <action_id>\n", argv[0]);
        return 1;
    }
    action_id = argv[1];

    loop = g_main_loop_new (NULL, FALSE);

    authority = polkit_authority_get_sync (NULL, NULL);

    /* Typically mechanisms will use a PolkitSystemBusName since most
     * clients communicate with the mechanism via D-Bus. However for
     * this simple example we use the process id of the calling process.
     *
     * Note that if the parent was reaped we have to be careful not to
     * check if init(1) is authorized (it always is).
     */
    parent_pid = getppid ();
    if (parent_pid == 1)
    {
        g_printerr ("Parent process was reaped by init(1)\n");
        return 1;
    }
    subject = polkit_unix_process_new (parent_pid);

    cancellable = g_cancellable_new ();

    g_print ("Will cancel authorization check in 10 seconds\n");

    /* Set up a 10 second timer to cancel the check */
    g_timeout_add (10 * 1000,
                   (GSourceFunc) do_cancel,
                   cancellable);

    polkit_authority_check_authorization (authority,
                                          subject,
                                          action_id,
                                          NULL, /* PolkitDetails */
                                          POLKIT_CHECK_AUTHORIZATION_FLAGS_ALLOW_USER_INTERACTION,
                                          cancellable,
                                          (GAsyncReadyCallback) check_authorization_cb,
                                          loop);

    g_main_loop_run (loop);

    g_object_unref (authority);
    g_object_unref (subject);
    g_object_unref (cancellable);
    g_main_loop_unref (loop);

    return 0;
}


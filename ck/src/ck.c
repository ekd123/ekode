#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>
#include <pthread.h>
#include "ckconfig.h"
#include "cksignal.h"
#include "cklist.h"
#include "ck.h"

/* if a child quits, remove the pid item from the list */
static void
_ck_child_quit (int signum)
{
    if (signum == SIGCHLD)
    {
        int status;
        pid_t pid;
        pid = wait (&status);
        _ck_list_del (pid);
    }
}

static void 
_ck_kill_all (void)
{
    unsigned int len;
    pid_t *list;
    list = _ck_list_get ();
    len = _ck_list_get_length ();
    for (; len != 0; len --)
        kill (list[len], SIGKILL);
}

static void
_ck_quit_impl (void)
{
    _ck_kill_all ();
    _ck_list_destroy ();
}

static void
_ck_quit (int signum)
{
    _ck_quit_impl ();
}

/**
 * 
 * initialize ck the library
 * 
 * * add a signal handler to SIGCHLD
 * * add an atexit handler
 * * add handlers for unexpected signals
 */
void
ck_init (void)
{
    _ck_list_init ();

    _ck_signal (SIGCHLD, _ck_child_quit);
    _ck_signal (SIGQUIT, _ck_quit);
    _ck_signal (SIGTERM, _ck_quit);
    _ck_signal (SIGABRT, _ck_quit);

    atexit (_ck_quit_impl);
}

static void* _ck_subprocess_setup_polling (void*arg)
{
    while (1)
    {
        pid_t pid;

        pid=getppid();
        if (pid == 1)
            raise (SIGKILL);
#ifdef CK_POLL_SECONDS
        sleep (CK_POLL_SECONDS);
#else
        sleep (1);
#endif
    }

    return NULL;
}

static void _ck_subprocess (void)
{
    pthread_t tid;
    pthread_create (&tid, NULL, _ck_subprocess_setup_polling, NULL);
}


pid_t 
ck_fork (void)
{
    pid_t pid;
    if ((pid = fork ()) == -1)
        return -1;
    else if (pid == 0)
        _ck_subprocess ();
    else 
        return pid;

    return -1;
}

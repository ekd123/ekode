#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "ck.h"

/* generate 100 children */
void generate_children (int sleepingtime)
{
    unsigned int i;
    pid_t pid;

    for (i = 0; i<100; i++)
    {
        pid = ck_fork ();
        if (pid == 0)
        {
            sleep (sleepingtime);
            puts ("still here! this is the child!");
            exit (0);
        }
        else if (pid == -1)
        {
            fprintf (stderr, "error forking\n");
            exit (1);
        }
        else break;
    }
}

/* kill the parent */
void test_kill_parent (void)
{
    generate_children (10);
    puts ("pass!");
    raise (SIGKILL);
}

/* let children quit first */
void test_children_quit_first (void)
{
    generate_children (0);
    sleep (2);
    puts ("pass!");
}

/* quit normally */
void test_quit_normally (void)
{
    generate_children (5);
    puts ("pass!");
}

int main (int argc, char *argv[])
{
    ck_init ();
    if (argc == 1)
    {
        puts ("test suite: \nkill_parent\nchildren_quit_first\nquit_normally");
        return 0;
    }
    else if (!strcmp (argv[1], "kill_parent"))
    {
        test_kill_parent ();
        return 0;
    }
    else if (!strcmp (argv[1], "children_quit_first"))
    {
        test_children_quit_first ();
        return 0;
    }
    else if (!strcmp (argv[1], "quit_normally"))
    {
        test_quit_normally ();
        return 0;
    }
    else 
    {
        printf ("unknown mode!\n");
    }

    return 0;
}

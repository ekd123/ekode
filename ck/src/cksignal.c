#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include "cksignal.h"

/**
   register a signal handler without unexpected behaviors
*/
void
_ck_signal (int signum,
         void (*handler)(int))
{
    struct sigaction act;
    sigemptyset (&act.sa_mask);
    act.sa_handler = handler;
    sigaction (signum, &act, NULL);
}

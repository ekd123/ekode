#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "fsize.h"

int 
main (int argc,
      char *argv[])
{
    FILE *fp = NULL;
    int fd;

    if ((fp = fopen (__FILE__ ,"r")) == NULL)
    {
        perror ("error opening the file.");
        return ENOENT;
    }
    printf ("stream: %ld\n", fsize(fp));
    fclose (fp);
    //----------------
    printf ("stat: %ld\n", fnsize (__FILE__));
    //----------------
    fd = open (__FILE__, O_RDONLY);
    printf ("fd: %ld\n", fdsize (fd));
    close (fd);

    return 0;
}

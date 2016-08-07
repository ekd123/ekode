#include <stdio.h>

int 
main (int argc,
      char *argv[])
{
    FILE *stream;
    char line[1024];

    stream = popen ("cat /etc/issue | head -n1", "r");
    fgets (line, sizeof (line), stream);
    puts (line);
    pclose (stream);

    return 0;
}

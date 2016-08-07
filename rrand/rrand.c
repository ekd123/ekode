#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

void 
rrand (char *buffer, 
       size_t size)
{
    int fd = open ("/dev/random", O_RDONLY);
    read (fd, buffer, size);
    close (fd);
}

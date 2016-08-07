#include <stdio.h>
#ifndef __WIN32__
#  include <unistd.h>
#  include <sys/stat.h>
#else
#  include <windows.h>
#endif

/*
   the portable `fsize' written on standard-c (ANSI C)
   I think it is not thread-safe.

   Usage: (Note that check return-values)
   * FILE *fp = fopen (__FILE__, "r");
   * long filesize = fsize (fp);
   * fclose (fp);
*/
long int
fsize (FILE *fp)
{
    long int old_pos = 0;
    long int size = 0;
    if ((old_pos = ftell (fp)) == -1)
        goto LBL_ERR_CANNOT_FTELL;

    if (fseek (fp, 0l, SEEK_END)) /* seek to the beginning */
        goto LBL_ERR_CANNOT_FSEEK;

    if ((size = ftell (fp)) == -1) /* get the lenght */
        goto LBL_ERR_CANNOT_FTELL;

    if (fseek (fp, old_pos, SEEK_SET)) /* seek back to old pos */
        goto LBL_ERR_CANNOT_FSEEK;

    return size;

  LBL_ERR_CANNOT_FTELL:
    return -1;

  LBL_ERR_CANNOT_FSEEK:
    return -2;
}

long int
fdsize (int fd)
{
#ifndef __WIN32__
    struct stat stbuf;
    if (fstat (fd, &stbuf))
#else
    struct _stat stbuf;
    if (_fstat (fd, &stbuf))
#endif
        return -1;

    return stbuf.st_size;
}

long int 
fnsize (const char *filename)
{
#ifndef __WIN32__
    struct stat buf;
    if (lstat (filename, &buf))
#else
    struct _stat buf;
    if (_lstat (filename, &buf))
#endif
        return -1;

    return buf.st_size;
}

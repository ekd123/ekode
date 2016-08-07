#ifndef __FSIZE_H__
#define __FSIZE_H__

long int
fsize (FILE *fp);

long int
fnsize (const char *filename);

long int
fdsize (int fd);

#endif /* __FSIZE_H__ */

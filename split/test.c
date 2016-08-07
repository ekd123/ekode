#include <sys/types.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "split.h"

/* strdup */
#ifdef strdup
#    undef strdup
#endif
char * strdup(const char *src)
{
    char *m = malloc (strlen (src));
    strcpy (m, src);
    return m;
}

int main (void)
{
    /* the str will be split */
    char sstr[] = "First:Second:THIRD:";
    printf ("string is %s\n", sstr);
    
    /* strdup */
    char *str = strdup (sstr);
    
    /* split */
    size_t number = split(str, ':');
    printf ("there are %d strings.\n", number);
    
    /* print the result */
    int i;
    for (i = 0; i < number; i++)
    {
        printf ("str %d: %X\n", i, (unsigned int)((char **)str)[i]);
    }
    
    /* clean up */
    free (str);
    
    /* quit */
    return 0;
}

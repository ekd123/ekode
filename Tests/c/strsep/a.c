/**
 * test for strsep(). man 3 strsep
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main ()
{
    char *str = strdup ("system=DEVFS subsystem=CDEV type=CREATE cdev=dsp4.1");
    char *tofree = str;
    char *tok;
    tofree = strdup (str);
    while ((tok = strsep (&str, " ")))
    {
        char *astr;
        int c = 1; /* key */
        while ((astr = strsep (&tok, "=")))
        {
            if (c % 2)
            {
                printf ("key: ");
                if (!strcmp ("system", astr))
                    printf ("--- it's system! ---");
            }
            else
                printf ("value: ");
            puts (astr);
            c ++;
        }
    }
    free (tofree);
    return 0;
}

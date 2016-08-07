#include <stdio.h>
#include <glib.h>

gint64 number16_reverse (gint64 source)
{
    char str[20];
    sprintf (str, "%X", source);
    g_strreverse (str);
    gint64 done = g_ascii_strtoll (str, NULL, 16);
    return done;
}

gint64 byte_reverse (gint64 source)
{
    
}

int main ()
{
    gint64 i;
    for (i=0; i<G_MAXINT64; i++)
    {
        g_print ("old number %X\tnew number %X\n", 
                 i, number16_reverse (i));
    }
    return 0;
}

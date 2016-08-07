/* 
   this code differs a string char by char.

   if chars are the same, then print a '-', or print the second char

   example:
   ABCDEF
   AACCEE
-> -A-C-E

   note: `differ' accepts strings in any lengths, enjoy!
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

char *differ (const char *src, const char *dest, int *length)
{
    int local_length = 0;
    char *buffer = NULL;
    for(int i=0; (i<strlen(src)) && (i<strlen(dest)); i++)
    {
        local_length ++;
        buffer = (char*)realloc(buffer, local_length);
        if (src[i] == dest[i])
            buffer[i] = '-';
        else 
            buffer[i] = dest[i];
    }
    if (length) *length = local_length;
    buffer = (char*)realloc(buffer, ++local_length);
    return buffer;
}

int main (int argc, char *argv[])
{
    if (argc < 3)
        exit (1);
    char *buffer = differ(argv[1], argv[2], NULL);
    printf ("%s\n", buffer);
    free (buffer);
    return 0;
}

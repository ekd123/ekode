#include <sys/types.h>

/* this function will change your string */
/* so maybe you want to strdup your string before calling split. */
/* your string must look like this, A:B:C:D:E:. */
size_t 
split (char *source,
       char  separator)
{
    size_t number;
    
    for (number = 0; *source != '\0'; source ++)
    {
        if (*source == separator)
        {
            *source = '\0';
            number ++;
        }
    }
    
    return number;
}

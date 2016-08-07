#include <stdio.h>
#include <unistd.h>

#define lambda(l_ret_type, l_arguments, l_body)                 \
    __extension__                                               \
    ({                                                          \
        l_ret_type __lambda_self l_arguments                    \
            l_body                                              \
            &__lambda_self;                                     \
    })

int 
main (int argc, char *argv[])
{
    printf ("hello world\n");
    int x = 
        lambda (int, (int x), 
                {
                    if (x < 3)
                        return 1;
                    x --;
                    __lambda_self (x);
                }) (100);
    printf ("%d\n", x);
    return 0;
}

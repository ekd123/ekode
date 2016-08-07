#include <stdio.h>

int main(void) {
    void (^hello)(void) = ^(void) {
        puts ("Hello block");
    };
    hello ();
    return 0;
}


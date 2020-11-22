// Bring in a libc header
#include <stdio.h>

#include "func.h"

int main()
{
    // Use a non-trivial libc function that nominally would require syscalls, heap availability, etc
    printf("Hello, World!\n");

    // Some simple test code that should not be optimized away
    volatile int a = 5;
    volatile int b = 6;
    volatile int c = func( a, b );

    while(1){
        c = (c * 11)/10;
    };

    return 0;
}

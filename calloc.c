#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char *argv[])
{
    char *ptr = calloc(3, sizeof(char));
    printf("addr: %p\n", ptr);
    printf("addr: %p\n", ptr+1);

    return 0;
}

/**
 * This code is GPL'ed!
 * monkey-coder:Shawn the R0ck <citypw@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
        int *p, i;

        /* allocate 80-byte in memory */
        p = (int*)malloc(sizeof(int) * 20);

        if(p == NULL)
        {
                fprintf(stderr, "mem allocation failed!\n");
                exit(1);
        }

        for(i = 1; i <= 20; i++){
                *(p + i - 1) = i;
                printf("i: %d\n",*(p + i - 1));
        }

        free(p);

        return 0;
}

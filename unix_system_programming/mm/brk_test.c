/**
 * This code is GPL'ed!
 * monkey-coder:Shawn the R0ck <citypw@gmail.com>
 */

#include <stdio.h>
#include <unistd.h> /* exit() */
#include <stdlib.h> 

void *mm_init(void *heap_mm)
{
        /* return current addr of heap */
        heap_mm = sbrk(0);
}

void *my_malloc(unsigned n)
{
        /* return: addr + n */
        return sbrk(n);
}

void *my_free(void * heap_mm)
{
        /* free your mind? */
        brk(heap_mm);
}

int main(int argc, char *argv[])
{
        int *p, i;

        mm_init(p);

        /* allocate 80-byte in memory */
        p = (int*)my_malloc(sizeof(int) * 20);

        if(p == NULL)
        {
                fprintf(stderr, "mem allocation failed!\n");
                exit(1);
        }

        for(i = 1; i <= 20; i++){
                *(p + i - 1) = i;
                printf("i: %d\n",*(p + i - 1));
        }

        my_free(p);

        return 0;
}

#include <stdio.h>

typedef struct{
        int x;
        int y;
}test;

int main(int argc, char *argv[])
{
        test *t1 = NULL;

        t1->x = 20; /* boom... */

        printf("%d\n", t1->x);        
        return 0;
}

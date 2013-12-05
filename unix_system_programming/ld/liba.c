#include <stdio.h>
int funcB(int, int);
int funcA(int a, int b)
{
    printf("hello from funcA\n");
    return funcB(a, b);
}

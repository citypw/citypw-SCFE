#include <stdio.h>
#include <dlfcn.h>
typedef int (*funcA)(int, int);
int main()
{
    void * plugin = dlopen("./libA.so", RTLD_LAZY);
    funcA f = (funcA)dlsym(plugin, "funcA");
    printf("main: %d\n", f(3,4));
    return 0;
}

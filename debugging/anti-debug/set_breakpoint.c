#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void signal_handler(int signo)
{
        printf("do not fuck with a hacker!\n");
        exit(1);
}

int main( int argc, char* argv[])
{
        signal(SIGTRAP, signal_handler);

        /* int3 will cause the SIGTRAP */
	__asm__("int3\n\t");
        printf("can you debug me?\n");
        return 0;
}

#include <stdio.h>

void test()
{
        printf("Hello\n");
}

int main()
{
        /* 0xcc is the opcode that equal to the assembly instruction "int3 */
        if ((*(volatile unsigned *)((unsigned)test + 4) & 0xff) == 0xcc) {
                printf("no shit...you can't set breakpoint dude!\n");
                exit(1);
        }
        test();
}


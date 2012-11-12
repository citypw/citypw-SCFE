/* This code is GPL'ed ! */

#include <stdio.h>

void fuck_me()
{
	printf("WTH!\n");
}

int main(int argc, char *argv[])
{
	unsigned int *p = ( unsigned int*) strtol( argv[1], NULL, 16);

        /* Dude, you should change the addr of fuck_me() on your own machine */
	p[0] = 0x08048454;

	printf("What? You're not fuc*ing supposed to be here: %p\n", p);
	return 0;
}

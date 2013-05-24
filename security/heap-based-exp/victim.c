   /* 
    * Just the vulnerable program we will exploit.
    * Compile as: gcc -o vulprog vulprog.c (or change exploit macros)
    */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define ERROR -1
#define BUFSIZE 64

int goodfunc(const char *str);	/* funcptr starts out as this */

int main(int argc, char **argv)
{
	static char buf[BUFSIZE];
	static int (*funcptr) (const char *str);

	if (argc <= 2) {
		fprintf(stderr, "Usage: %s <buf> <goodfunc arg>\n",
			argv[0]);
		exit(ERROR);
	}

	printf("(for 1st exploit) system() = %p\n", system);
	printf("(for 2nd exploit, stack method) argv[2] = %p\n", argv[2]);
	printf("(for 2nd exploit, heap offset method) buf = %p\n\n", buf);

	funcptr = (int (*)(const char *str)) goodfunc;
	printf("before overflow: funcptr points to %p\n", funcptr);

	memset(buf, 0, sizeof(buf));
	strncpy(buf, argv[1], strlen(argv[1]));
	printf("after overflow: funcptr points to %p\n", funcptr);

	(void) (*funcptr) (argv[2]);
	return 0;
}

   /* This is what funcptr would point to if we didn't overflow it */
int goodfunc(const char *str)
{
	printf("\nHi, I'm a good function.  I was passed: %s\n", str);
	return 0;
}

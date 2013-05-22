#include <stdlib.h>
#include <stdio.h>

#define DEFAULT_OFFSET                    0
#define DEFAULT_BUFFER_SIZE             512
#define DEFAULT_EGG_SIZE               1024
#define NOP                            0x90

char shellcode[] =
    "\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b"
    "\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd"
    "\x80\xe8\xdc\xff\xff\xff/bin/sh";

unsigned long get_esp(void)
{
	__asm__("mov %rsp,%rax");
}

#define HELLO "/home/shawn/hello"
#define WORLD "/home/shawn/world"

void main(int argc, char *argv[])
{
	char *buff, *ptr, *egg, tmp_buf[256];
	long *addr_ptr, addr;
	int offset = DEFAULT_OFFSET, bsize = DEFAULT_BUFFER_SIZE;
	int i, eggsize = DEFAULT_EGG_SIZE;
	FILE *fp = NULL;

	memset(tmp_buf, 0x00, 256);
	
	printf("We are gonna run a few tests for apparmor's policies!\n");

	fp = fopen("/etc/shadow", "r");
	if( fp == NULL)
		printf("[1] no READ permission of /etc/shadow\n");
	if( system("/bin/sh") != 0)
		printf("[2] Spawn shell failed!\n");

	fp = fopen(HELLO, "r");
	if( fp != NULL)
		printf("[3] apparmor allows this program read from the file...\n");

	fp = fopen(WORLD, "w");
	if( fp != NULL)
		printf("[4] apparmor allows this program write to the file...\n");
}

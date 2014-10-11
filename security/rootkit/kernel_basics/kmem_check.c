#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

struct {
        unsigned short limit;
        unsigned int base;
} __attribute__ ((packed)) idtr;

struct {
        unsigned short off1;
        unsigned short sel;
        unsigned char none,flags;
        unsigned short off2;
} __attribute__ ((packed)) idt;

int kmem;
void readkmem (void *m,unsigned off,int sz)
{
        if (lseek(kmem,off,SEEK_SET)!=off) {
                perror("kmem lseek"); exit(2);
        }
        if (read(kmem,m,sz)!=sz) {
                perror("kmem read"); exit(2);
        }
}

#define CALLOFF 100     /* we'll read first 100 bytes of int $0x80*/

int main()
{
	unsigned sys_call_off;
	unsigned sct;
	char sc_asm[CALLOFF], *p;
	asm("sidt %0" : "=m" (idtr));

	printf("idtr base: 0x%x\nChecking the restriction of /dev/kmem...\n", (int)idtr.base);

	kmem = open("/dev/kmem", O_RDONLY);
	if( kmem < 0 ) perror("open");
	unsigned int i, ret;
	for( i = 0; i < 0xffffffff; i+=4){
		if( (ret = lseek(kmem, i, SEEK_SET)) != i){
			printf("The address of /dev/kmem is restricted less than 0x%x\n", i);
			break;
		}
	}
/*
	readkmem( &idt, idtr.base+8 * 0x80, sizeof(idt));
	printf("idt80: flags=%X sel=%X off=%X\n", 
		(unsigned)idt.flags,(unsigned)idt.sel,sys_call_off);
*/
	return 0;
}

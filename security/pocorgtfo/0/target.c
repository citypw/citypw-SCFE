#include <stdio.h>
#include "interp.inc"

const char my_interp[] __attribute__(( section(".interp"))) = INTERP;

extern int fuc();

int foo(){
	// printf(" Calling func\n");
	func();
	exit(1); // Needed, because there is no crt.o
}

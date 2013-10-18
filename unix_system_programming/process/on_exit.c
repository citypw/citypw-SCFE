#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

void print_status(int status, void *p)
{
	printf("Status: %d, MSG=%s\n", status, (char*)p);
}

int main( int argc, char *argv[])
{
	void (*p)(int, void*);

	p = print_status;

	on_exit(p, "WTH");	
	return -1;
}

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

void print_status()
{
	printf("called after atexit()...bye, cruel world!\n");
}

int main(int argc, char *argv[])
{
	void (*test)() = print_status;

	atexit(test);

	return 0;
}

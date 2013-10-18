#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

void print_status()
{
	printf("called after atexit()...bye, cruel world!\n");
}

int main(int argc, char *argv[])
{
	void (*test)() = print_status;

	atexit(test);

	_exit(0);
}

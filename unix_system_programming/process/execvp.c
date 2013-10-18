#include <stdio.h>
#include <errno.h>
#include <unistd.h>

int main( int argc, char *argv[])
{
	char *cp[3];
	int ret;

	cp[0] = "cat";
	cp[1] = "/proc/cpuinfo";
	cp[2] = NULL;

	ret = execvp(cp[0], cp);
	if( ret == -1)
		perror("execvp");	
	return 0;
}

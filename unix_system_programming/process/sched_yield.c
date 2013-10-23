#include <stdio.h>
#include <sched.h>

int main( int arc, char *argv[])
{

	while(1)
	if (sched_yield ( ))
		perror ("sched_yield");
	
	return 0;
}

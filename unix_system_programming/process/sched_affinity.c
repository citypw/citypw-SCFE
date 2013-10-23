#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <sched.h>

void print_status(cpu_set_t *set)
{
	int i;

	for(i=0; i < CPU_SETSIZE; i++){
		int cpu;

		cpu = CPU_ISSET(i, set);
		if( cpu)
			printf("cpu=%d is set!\n", i);
	}

}
int main( int argc, char *argv[])
{
	cpu_set_t set;
	int ret, i;

	CPU_ZERO(&set);
	ret = sched_getaffinity(0, sizeof(cpu_set_t), &set);
	if( ret == -1)
		perror("sched_getaffinity");

	print_status(&set);

	printf("Now, let's trying to unset CPU #1\n");
	CPU_CLR(1, &set);
	ret = sched_setaffinity(0, sizeof(cpu_set_t), &set);
	if( ret == -1)
		perror("sched_setaffinity");

	print_status(&set);

	return 0;
}

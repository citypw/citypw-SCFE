#include <stdio.h>
#include <errno.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	pid_t pid;
	pid = fork();
	if (pid > 0)
		printf("I am the parent of pid=%d!\n", getpid());
	else if (pid == 0)
		printf("I am the baby pid=%d\n", getpid());
	else if (pid == -1)
		perror("fork");

	return 0;
}

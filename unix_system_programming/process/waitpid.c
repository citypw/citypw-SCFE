#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(void)
{
	int status, ret;
	pid_t pid;

	ret = fork();
	if( ret == -1){
		perror("fork");
		exit(1);
	}
	else if( ret  == 0){ /* child process */
		sleep(1);
		return 1;
	}
	else{

	pid = waitpid(ret, &status, 0);
	if (pid == -1)
		perror("wait");
	printf("pid=%d\n", pid);
	if (WIFEXITED(status))
		printf("Normal termination with exit status=%d\n",
		       WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		printf("Killed by signal=%d%s\n",
		       WTERMSIG(status),
		       WCOREDUMP(status) ? " (dumped core)" : "");
	if (WIFSTOPPED(status))
		printf("Stopped by signal=%d\n", WSTOPSIG(status));
	if (WIFCONTINUED(status))
		printf("Continued\n");
}
	return 0;
}

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int ret, nic;

	nic = nice(0);
	printf("current nice value: %d\n", nic);

	ret = fork();
	if (ret == -1) {
		perror("fork");
		exit(1);
	} else if (ret == 0) {
		setuid(getuid());
		nic = nice(-10);
		if (nic == -1) {
			perror("nice");
			fprintf(stderr,
				"non-root can't decrease the nice value!\n");
			exit(1);
		}

		printf("Child process's nice value: %d\n", nic);
		return 0;
	}

	/* child process */
	nic = nice(-10);
	if (nic == -1) {
		perror("nice");
		exit(1);
	}

	printf("Parent's process nice value: %d\n", nic);

	return 0;
}

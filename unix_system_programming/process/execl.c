#include <stdio.h>
#include <errno.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int ret;
	ret = execl("/usr/bin/vim", "vim", NULL);
	if (ret == -1)
		perror("execl");

	/* not gonna happen */
	ret = execl("/bin/ls", "ls", NULL);
	if (ret == -1)
		perror("execl");

	printf("done!\n");
	return 0;
}

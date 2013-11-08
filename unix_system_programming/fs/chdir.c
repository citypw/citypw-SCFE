#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	char *swd;
	int ret;
/* save the current working directory */
	swd = getcwd(NULL, 0);
	if (!swd) {
		perror("getcwd");
		exit(EXIT_FAILURE);
	}
	printf("current: %s\n", swd);
/* change to a different directory */
	ret = chdir("/");
	if (ret) {
		perror("chdir");
		exit(EXIT_FAILURE);
	}
	printf("changing to %s\n", getcwd(NULL,0));
/* do some other work in the new directory... */
/* return to the saved directory */
	ret = chdir(swd);
	if (ret) {
		perror("chdir");
		exit(EXIT_FAILURE);
	}
	printf("current: %s\n", swd);
	free(swd);

	return 0;
}

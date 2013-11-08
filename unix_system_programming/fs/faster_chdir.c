#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	int swd_fd, ret;
	char *s;

	swd_fd = open(".", O_RDONLY);
	if (swd_fd == -1) {
		perror("open");
		exit(EXIT_FAILURE);
	}
	
	printf("current: %s\n", getcwd(NULL, 0));
/* change to a different directory */
	ret = chdir("/");
	if (ret) {
		perror("chdir");
		exit(EXIT_FAILURE);
	}
	printf("changing to: %s\n", getcwd(NULL, 0));
/* do some other work in the new directory... */
/* return to the saved directory */
	ret = fchdir(swd_fd);
	if (ret) {
		perror("fchdir");
		exit(EXIT_FAILURE);
	}
	printf("changing back to: %s\n", getcwd(NULL, 0));
/* close the directory's fd */
	ret = close(swd_fd);
	if (ret) {
		perror("close");
		exit(EXIT_FAILURE);
	}

	return 0;
}

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	char *cwd;
	cwd = getcwd(NULL, 0);

	if( cwd == NULL)
	{
		perror("getcwd");
		exit(EXIT_FAILURE);
	}
	printf("cwd = %s\n", cwd);
	free(cwd);
	return 0;
}

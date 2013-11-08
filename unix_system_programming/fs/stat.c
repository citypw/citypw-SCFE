#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
	struct stat sb;
	int ret;

	if( argc < 2){
		fprintf(stderr, "usage: %s <file> \n", argv[0]);
		return -1;
	}

	ret = stat(argv[1], &sb);
	if( ret == -1){
		perror("stat");
		return -1;
	}

	printf("%s is %ld bytes!\n", argv[1], sb.st_size);
	return 0;
}

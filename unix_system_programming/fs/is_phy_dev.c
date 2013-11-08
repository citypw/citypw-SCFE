#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
/*
* is_on_physical_device – returns a positive
* integer if 'fd' resides on a physical device,
* 0 if the file resides on a nonphysical or
* virtual device (e.g., on an NFS mount), and
* -1 on error.
*/
int is_on_physical_device(int fd)
{
	struct stat sb;
	int ret;
	ret = fstat(fd, &sb);
	if (ret) {
		perror("fstat");
		return -1;
	}
	return gnu_dev_major(sb.st_dev);
}

int main(int argc, char *argv[])
{
	int ret, fd;

	if (argc < 2) {
		fprintf(stderr, "usage: %s <file> \n", argv[0]);
		return -1;
	}

	fd = open(argv[1], O_RDONLY | O_EXCL, 0644);
	if (fd == -1) {
		perror("open");
		return -1;
	}

	ret = is_on_physical_device(fd);

	if (ret == 0)
		printf("On virtual device( NFS?)\n");
	else
		printf("On physical device...\n");
	return 0;
}

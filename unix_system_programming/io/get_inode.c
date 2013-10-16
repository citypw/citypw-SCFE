/*
 * In old days, even writing profiles for SELinux, you need inode number;-)
 */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

/*
* get_inode - returns the inode of the file associated
* with the given file descriptor, or -1 on failure
*/
unsigned long int get_inode(int fd)
{
	struct stat buf;
	int ret;
	ret = fstat(fd, &buf);
	if (ret < 0) {
		perror("fstat");
		return -1;
	}
	return buf.st_ino;
}

int main(int argc, char *argv[])
{
	int fd;
	unsigned long int inode;
	if (argc < 2) {
		fprintf(stderr, "usage: %s <file>\n", argv[0]);
		return 1;
	}
	fd = open(argv[1], O_RDONLY);
	if (fd < 0) {
		perror("open");
		return 1;
	}
	inode = get_inode(fd);
	printf("%ld\n", inode);
	return 0;
}

/* This is read_all example for nonblocking read() manunipulation */

#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

int read_all(int fd, void *buf, size_t len)
{
	ssize_t ret;
	while (len != 0 && (ret = read(fd, buf, len)) != 0) {
		if (ret == -1) {
			if (errno == EINTR) /* EINTR = signal break, EAGAIN = no data currently available */
				continue;
                        if( errno == EAGAIN) /* EAGAIN, no data to read */
                                perror("read");
                        return -1;
			perror("read");
			break;
		}
		len -= ret;
		buf += ret;
	}

        return 0;
}

int main(int argc, char *argv[])
{
	int fd;
	char buf[256] = {0};

	fd = open("test.log", O_RDONLY | O_EXCL | O_NONBLOCK, 0644);

	read_all(fd, buf, sizeof(buf));

	printf("buf: %s\n", buf);
	return 0;
}

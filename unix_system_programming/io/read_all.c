#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

/*
Read everything even something bad happened. End conditions are:
--------------------------------------------------------------------------------
read all len bytes( return len) or EOF is reached( return zero).
--------------------------------------------------------------------------------
And, execeptional handling:
--------------------------------------------------------------------------------
return a value less than len, re-calculate the buffer and number of
bytes haven't read yet, then reissue read() again.  return -1 and
errno=EINTR, it's signal break before read anything.  return -1 and
errno=?, loop terminates
--------------------------------------------------------------------------------
*/

void read_all(int fd, void *buf, size_t len)
{
	ssize_t ret;
	while (len != 0 && (ret = read(fd, buf, len)) != 0) {
		if (ret == -1) {
			if (errno == EINTR) /* EINTR = signal break, EAGAIN = no data currently available */
				continue;
			perror("read");
			break;
		}
		len -= ret;
		buf += ret;
	}
}

int main(int argc, char *argv[])
{
	int fd;
	char buf[256] = {0};

	fd = open("test.log", O_RDONLY | O_EXCL, 0644);

	read_all(fd, buf, sizeof(buf));

	printf("buf: %s\n", buf);
	return 0;
}

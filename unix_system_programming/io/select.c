#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h> 
#include <fcntl.h>

#define TIMEOUT 5
#define BUF_LEN 1024
/* select timeout in seconds */
/* read buffer in bytes */
int main(int argc, char *argv[])
{
	struct timeval tv;
	fd_set readfds;
	int ret, fd, fd_max;

	fd = open(argv[1] , O_RDONLY | O_EXCL, 0644);
	if( fd == -1)
		fd_max = 0;
/*
	if( fd == -1){
		perror("open");
		return 1;
	}*/
/* Wait on stdin for input. */
	FD_ZERO(&readfds);
	FD_SET(STDIN_FILENO, &readfds);
	FD_SET(fd, &readfds);

/* Wait up to five seconds. */
	tv.tv_sec = TIMEOUT;
	tv.tv_usec = 0;

/* All right, now block! */
	ret = select( fd_max + 1, &readfds, NULL, NULL, &tv);
	if (ret == -1) {
		perror("select");
		return 1;
	} else if (!ret) {
		printf("%d seconds elapsed.\n", TIMEOUT);
		return 0;
	}
/*
* Is our file descriptor ready to read?
* (It must be, as it was the only fd that
* we provided and the call returned
* nonzero, but we will humor ourselves.)
*/

	if (FD_ISSET(STDIN_FILENO, &readfds)) {
		char buf[BUF_LEN + 1];
		int len;
/* guaranteed to not block */
		len = read(STDIN_FILENO, buf, BUF_LEN);
		if (len == -1) {
			perror("read");
			return 1;
		}
		if (len) {
			buf[len] = '\0';
			printf("read: %s\n", buf);
		}
		return 0;
	}

	if( FD_ISSET(fd, &readfds)){
		char buf[128];
		int len;

		printf("\"%s\" is readable now!\n", argv[1]);
		len = read(fd, buf, 128);
		if( !len){
			printf("file closed already!\n");
			return 1;
		}
		printf("buf: %s\n", buf);
		return 0;
	}

	fprintf(stderr, "This should not happen!\n");
	return 1;
}

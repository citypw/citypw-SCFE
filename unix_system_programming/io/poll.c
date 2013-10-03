#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h> 
#include <fcntl.h>
#include <sys/poll.h>

#define TIMEOUT 5
#define BUF_LEN 1024

/* select timeout in seconds */
/* read buffer in bytes */
int main(int argc, char *argv[])
{
        struct pollfd fds[2];
	int ret, fd, fd_max;

	fd = open(argv[1] , O_WRONLY | O_EXCL, 0644);
	if( fd == -1){
		perror("open");
		return 1;
        }

        /* watch stdin and opened fd */
        fds[0].fd = STDIN_FILENO;
        fds[0].events = POLLIN;

        fds[1].fd = fd;
        fds[1].events = POLLOUT;

        ret = poll(fds, sizeof(fds) / sizeof(struct pollfd), TIMEOUT * 1000);
        if( ret == -1){
                perror("poll");
                return 1;
        }

        if( !ret){
                printf("%d seconds elapsed!\n", TIMEOUT);
                return 0;
        }

        if(fds[0].revents & POLLIN)
                printf("stdin is readable!\n");

        if(fds[1].revents & POLLOUT)
                printf("opened fd is writable!\n");

	return 0;
}

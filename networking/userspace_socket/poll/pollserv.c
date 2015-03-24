#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <poll.h>
 
 
#define POLL_SIZE 32
#define LISTEN_QUEUE 5
#define SOCKET_PATH "web/server_socket.sock"
 
 
#define TRUE 1
#define FALSE 0
 
 
 
 
int main()
{
    int server_sockfd, client_sockfd;
    int server_len, client_len;
    struct sockaddr_un server_address;
    struct sockaddr_un client_address;
    int result;
 
 
    struct pollfd poll_set[POLL_SIZE];
    int numfds = 0;
 
 
    unlink(SOCKET_PATH);
    server_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
 
 
    server_address.sun_family = AF_UNIX;
    strcpy(server_address.sun_path, SOCKET_PATH);
    server_len = sizeof(server_address);
 
 
    bind(server_sockfd, (struct sockaddr *)&server_address, (socklen_t)server_len);
    listen(server_sockfd, 5);
    memset(poll_set, '\0', sizeof(poll_set));
    poll_set[0].fd = server_sockfd;
    poll_set[0].events = POLLIN | POLLPRI;
    numfds++;
    //signal(SIGPIPE, SIG_IGN);
 
 
 
 
 
 
    while (1) {
        char ch;
        int fd_index;
        int nread;
 
 
        printf("Waiting for client (%d total)...\n", numfds);
        //result = select( (max_fd + 1), &testfds, (fd_set *)0, (fd_set *)0, (struct timeval *) 0);
        poll(poll_set, numfds, 2000);
        for(fd_index = 0; fd_index < numfds; fd_index++)
        {
            if( poll_set[fd_index].revents & POLLIN ) {
                if(poll_set[fd_index].fd == server_sockfd) {
                    client_len = sizeof(client_address);
                    client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, (socklen_t *)&client_len);
 
 
                    poll_set[numfds].fd = client_sockfd;
                    poll_set[numfds].events = POLLIN;
                    numfds++;
 
 
                    printf("Adding client on fd %d\n", client_sockfd);
                }
                else {
                    ioctl(poll_set[fd_index].fd, FIONREAD, &nread);
 
 
                    if( nread == 0 )
                    {
                        close(poll_set[fd_index].fd);
                        poll_set[fd_index].events = 0;
                        printf("Removing client on fd %d\n", poll_set[fd_index].fd);
                        //int i = fd_index;
                        //for( i = fd_index; i<numfds; i++)
                        //{
                        poll_set[fd_index] = poll_set[fd_index + 1];
                        //}
                        numfds--;
                    }
 
 
                    else {
                        read(poll_set[fd_index].fd, &ch, 1);
                        printf("Serving client on fd %d\n", poll_set[fd_index].fd);
                        ch++;
                        write(poll_set[fd_index].fd, &ch, 1);
                    }
                }
            }
 
 
 
 
        }
 
 
 
 
 
 
    }
}

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/un.h>
#include <unistd.h>
 
 
#define SOCKET_PATH "web/server_socket.sock"
 
 
int main()
{
    int sockfd;
    int len;
    struct sockaddr_un address;
    int result;
    char ch = 'A';
 
 
    /* -AF_UNIX is a UNIX internal socket, created on the filesystem.
     * -SOCK_STREAM is a reliable, sequenced, connection-based two-way byte stream
     *  (INET equivalent is TCP) */
      
    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
 
 
    /* -sun_family - specifies type (domain) of socket
     * -sun_path - socket path location (on filesystem) */
 
 
    address.sun_family = AF_UNIX;
    strcpy(address.sun_path, SOCKET_PATH);
    len = sizeof(address);
 
 
    /* request connection from the server, connecting socket to server with specified address
     * 'len' is the length of the address */
    result = connect(sockfd, (struct sockaddr *)&address, len);
 
 
    /* exit with error if connection is not successful */
    if(result == -1) {
        perror("oops: client1");
        exit(1);
    }
 
 
    /* otherwise write data to server and obtain response */
    write(sockfd, &ch, 1);
    read(sockfd, &ch, 1);
    printf("char from server: %c\n", ch);
    close(sockfd);
 
 
    exit(0);
}

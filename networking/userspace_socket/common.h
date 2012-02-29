#ifndef __COMMON_TEST__
#define __COMMON_TEST__

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <dirent.h>
#include <signal.h>

#define SA struct sockaddr
#define SERV_PORT 9556

/* Using these 2 macros instead of traditional printf */
#define DEBUG_ERR(format, args...) fprintf(stderr, format, ##args)

#define DEBUG_MSG(string, args...) do{ \
        DEBUG_ERR("File: %s, Function: %s, Line: %d\nDescription: ", __FILE__, __FUNCTION__, __LINE__); \
        DEBUG_ERR(string, ##args); \
        DEBUG_ERR("-------------------------------\n"); \
}while(0);


int open_udp_server_socket(unsigned short srv_port, struct sockaddr_in* serv_addr)
{
int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
if (sockfd < 0) {
DEBUG_MSG("socket() error");
return -1;
}

memset(serv_addr, 0x00, sizeof(struct sockaddr_in));

serv_addr->sin_family = AF_INET;
serv_addr->sin_addr.s_addr = htonl(INADDR_ANY);
serv_addr->sin_port = htons(srv_port);

if (bind(sockfd, (struct sockaddr *) serv_addr, sizeof(struct sockaddr_in)) < 0) {
DEBUG_MSG("bind() error\n");
return -1;
}

return sockfd;

}

int open_udp_client_socket(unsigned int cli_port, struct sockaddr_in *cli_addr, char *ip_addr)
{
  int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
if (sockfd < 0) {
DEBUG_MSG("socket() error");
return -1;
}

memset(cli_addr, 0x00, sizeof(struct sockaddr_in));

cli_addr->sin_family = AF_INET;
inet_aton(ip_addr, cli_addr->sin_addr);
cli_addr->sin_port = htons(cli_port);

return sockfd;
}
#endif

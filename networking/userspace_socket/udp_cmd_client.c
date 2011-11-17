#include "common.h"

int main(int argc, char *argv[])
{
  int sockfd, addrlen;
  struct sockaddr_in cli_addr;

  addrlen = sizeof(struct sockaddr_in);
  if(argc != 3){
    printf("usage: %s [IP_ADDR] [COMMAND]\n", argv[0]);
    exit(1);
  }

  sockfd = open_udp_client_socket(SERV_PORT, &cli_addr, argv[1]);

  sendto(sockfd, argv[2], strlen(argv[2]), 0, (struct sockaddr*)&cli_addr, addrlen);

  close(sockfd);

  return 0; 

}

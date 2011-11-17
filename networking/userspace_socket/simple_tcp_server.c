/* Please follow the principle of "RTFSC" and you will understandy
 * everything, finally! This program is an simple example about
 * tcp-based communication between server and client.
 *
 * monkey-coders: Shawn the R0ck@copyleft, <citypw@gmail.com>
 *
 * Date: Aug 18th, 2011
 *
 * This code is GPL'ed!
 */

#include "common.h"

#define GREETING "Shalom!"

int main(int argc, char *argv[])
{
  int sockfd, clientfd, sockaddr_size;
  struct sockaddr_in serv_addr, client_addr;
  char buf[256], answer[32];
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd == -1){
    perror("socket()");
    exit(1);
  }

  memset(&serv_addr, 0x00, sizeof(struct sockaddr_in));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(SERV_PORT);
  serv_addr.sin_addr.s_addr = INADDR_ANY;

  if(bind(sockfd, (SA*)&serv_addr, sizeof(SA)) == -1){
    perror("Unable to bind");
    exit(1);
  }

  listen(sockfd, 5);

  printf("TCP server waiting for client on port 5566!\n");
  sockaddr_size = sizeof(SA);
  while(1){
  memset(answer, '\0', sizeof(answer));
  memset(buf, '\0', sizeof(buf));

    clientfd = accept(sockfd, (SA*)&client_addr, &sockaddr_size);
    printf("Got a connection from %s:%d\n",inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
   
    send(clientfd, GREETING, strlen(GREETING), 0);
    printf("Server:%s\n", GREETING);

    recv(clientfd, buf, sizeof(buf), 0);
    printf("Client:%s\n", buf);

    gets(answer);
    send(clientfd, answer, strlen(answer), 0);
    printf("Server: %s\n",answer);

    recv(clientfd, buf, sizeof(buf), 0);
    printf("Client: %s\n", buf);
    close(clientfd);
    printf("client is closed!\n");
  }
  return 0;
}

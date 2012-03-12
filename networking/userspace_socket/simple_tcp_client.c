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

#define QUESTION1 "Are you Jew?"

int main(int argc, char *argv[])
{
  int sockfd;
  struct sockaddr_in serv_addr;
  struct hostent *host;
  char buf[256], answer[32];

  if(argc != 2){
    fprintf(stderr, "usage:serv_tcp [IP ADDR]\n");
    exit(1);
  }

  host = gethostbyname(argv[1]);

  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  memset(answer, '\0', sizeof(answer));
  memset(buf, '\0', sizeof(buf));
  memset(&serv_addr, 0x00, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(SERV_PORT);
  //  serv_addr.sin_addr = *((struct in_addr*)host->h_addr);
  inet_aton(argv[1], &serv_addr.sin_addr);

  if(connect(sockfd, (SA*)&serv_addr, sizeof(SA)) == -1){
    perror("connect()");
    exit(1);
  }

  recv(sockfd, buf, sizeof(buf), 0);
  printf("Server: %s\n", buf);
  // strncpy(buf, QUESTION1, strlen(QUESTION1));
  sleep(1);
  send(sockfd, QUESTION1, strlen(QUESTION1), 0);
  printf("Client: %s\n", QUESTION1);

  recv(sockfd, answer, sizeof(buf), 0);
  printf("Server: %s\n", answer);
  if(strncmp(answer, "Yes", 3) == 0 || strncmp(answer, "yes", 3) == 0){
	strcpy(buf,"Actually, the singularitarian has always believed in .....\n");
	send(sockfd, buf, strlen(buf), 0);
	printf("Client: %s\n", buf);
  }
  else{
	strcpy(buf, "Yo, how you doing? buddy\n");
	send(sockfd, buf, strlen(buf), 0);
	printf("Client: %s\n", buf);
  }
  close(sockfd);
  return 0;
}

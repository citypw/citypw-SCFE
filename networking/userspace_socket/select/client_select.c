/* Please follow the principle of "RTFSC" and you will understandy
 * everything, finally! This program is an sample tcp server with
 * select() implementation of IO multiplexing.
 *
 * monkey-coders: Shawn the R0ck@copyleft, <citypw@gmail.com>
 *
 * Date: Feb 29th, 2012
 *
 * This code is GPL'ed!
 */

#include <common.h>

#define QUESTION1 "Shalom!"

int main(int argc, char *argv[])
{
  fd_set fds_rd;
  int sockfd, ret;
  struct timeval tv;
  struct sockaddr_in serv_addr;
  struct hostent *host;
  char buf[256], answer[32];

  if (argc != 2) {
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

    FD_ZERO(&fds_rd);
    FD_SET(sockfd, &fds_rd);

    tv.tv_sec = 5;
    tv.tv_usec = 0;

    if (connect(sockfd, (SA *) & serv_addr, sizeof(SA)) == -1) {
	perror("connect()");
	exit(1);
    }

    send(sockfd, QUESTION1, strlen(QUESTION1), 0);
    printf("Client: %s\n", QUESTION1);

    ret = select(sockfd + 1, &fds_rd, NULL, NULL, &tv);
    if(ret == 0){
      DEBUG_MSG("No data within %d seconds.!\n", tv.tv_sec);
      exit(1);
    }
    else if(ret > 0){
      recv(sockfd, buf, sizeof(buf), 0);
      printf("Received Server: %s\n", buf);
    }
    else{
      perror("select()");
      exit(1);
    }

    close(sockfd);
    return 0;
}

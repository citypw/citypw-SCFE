/* Please follow the principle of "RTFSC" and you will understandy
 * everything, finally! This program is an sample tcp server with
 * select() implementation of single process and single thread.
 *
 * monkey-coders: Shawn the R0ck@copyleft, <citypw@gmail.com>
 *
 * Date: Feb 29th, 2012
 *
 * This code is GPL'ed!
 */

#include <common.h>

/* According to POSIX.1-2001 */
#include <sys/select.h>

/* According to earlier standards */
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#define GREETING "Is this a metaphor?"
#define BACKLOG 5

int main(int argc, char *argv[])
{
  int sockfd, clientfd, sockaddr_size, ret;
  fd_set fds_tmp, fds_rd;
  int fd_max, i, j, max_client = -1, addrlen, conn_num = 0;;
  struct sockaddr_in serv_addr, client_addr;
  struct timeval tv;
  char buf[256], answer[32];
  int client_arr[FD_SETSIZE];

  /* create a socket */
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
	perror("socket()");
	exit(1);
  }
  DEBUG_MSG("Server socket() is OK...\n");

  memset(&serv_addr, 0x00, sizeof(struct sockaddr_in));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(SERV_PORT);
  serv_addr.sin_addr.s_addr = INADDR_ANY;

  if (bind(sockfd, (SA *) & serv_addr, sizeof(SA)) == -1) {
	perror("Unable to bind");
	exit(1);
  }
  DEBUG_MSG("Server bind() is OK...\n");

  if(listen(sockfd, BACKLOG) == -1){
    perror("listen()");
    exit(1);
  }
  DEBUG_MSG("Server listen() is OK...\n");

  /* initialize the fd set and add one to it */
  FD_ZERO(&fds_rd);
  FD_SET(sockfd, &fds_rd);
  for(i = 0; i < FD_SETSIZE; i++)
    client_arr[i] = -1;

  /* Keep track of the biggest fd */
  fd_max = sockfd;

  printf("TCP server waiting for client on port 5566!\n");
  sockaddr_size = sizeof(SA);

  while (1) {
    memset(buf, '\0', sizeof(buf));

    /* focus on fds_rd, fds_tmp is only a tmp variable */
    fds_tmp = fds_rd;

    /* wait for 5 secs */
    tv.tv_sec =5;
    tv.tv_usec =0;

    ret = select(fd_max + 1, &fds_tmp, NULL, NULL, &tv);
    //    printf("ret is %d\n", ret);
    if(ret == 0){
      DEBUG_MSG("No data within %d seconds.!\n", tv.tv_sec);
      continue;
    }
    else if( ret == -1){
      perror("select()");
      exit(1);
    }

    /* handle new connection */
    if(FD_ISSET(sockfd, &fds_tmp)){

      DEBUG_MSG("Client is trying to connect...\n");
      addrlen = sizeof(client_addr);

      clientfd = accept(sockfd, (SA *) & client_addr, &sockaddr_size);
      if(clientfd == -1){
            perror("accept()");
      }
      DEBUG_MSG("Server accept() is OK...\n");
      DEBUG_MSG("Got a new connection from %s:%d\n",
                inet_ntoa(client_addr.sin_addr),
                ntohs(client_addr.sin_port));

      /* find a place where to save the fd */
      for(i = 0; i < FD_SETSIZE; i++){
        if(client_arr[i] < 0)
        {
          client_arr[i] = clientfd;
          break;
        }
      }

      FD_SET(clientfd, &fds_rd);
      if(clientfd > fd_max){
        fd_max = clientfd;
      }
      if(i > max_client){
        max_client = i;
      }

      /* check all clients */
      for(j = 0; j <= i; j++)
      {
        if(client_arr[j] > 0)
        {
          if(FD_ISSET(client_arr[j], &fds_rd)){
            int n;
            n = recv(client_arr[j], buf, sizeof(buf), 0);
            if(n > 0){
              buf[n] = '\0';
              printf("Received Client %d: %s\n", client_arr[j], buf);
              send(client_arr[j], GREETING, strlen(GREETING), 0);
              printf("Server sent to Client %d:%s\n", client_arr[j], GREETING);
            }

            /* close and clear the fd from fd set, if no data received */
            if(n == 0){
              close(client_arr[j]);
              FD_CLR(client_arr[j], &fds_rd);
              client_arr[j] = -1;
            }
          }
        }
      }
    }
  }

  close(sockfd);
  return 0;
}

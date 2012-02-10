/**
 * This code is GPL'ed!
 * monkey-coder:Shawn the R0ck <citypw@gmail.com>
 */


#include <stdio.h>
#include <debug_msg.h>
#include <unistd.h> /* using pid_t */
#include <string.h> /* memset() */

int main(int argc, char **argv[])
{
  pid_t pid;

  /* create 2 pipes, 0: read, 1: write */
  int fd[2];

  int rd_num;
  char wr_buf[256], rd_buf[256];
  char *test_msg = "hello pipe!";

  /* set the buffer to zero in memory */
  memset(wr_buf, '\0', 256);
  memset(rd_buf, '\0', 256);

  if(pipe(fd) == -1){
    DEBUG_MSG("failed to create pipes!\n");
    return -1;
  }

  pid = fork(); /* create a new process */
  if(pid < 0){
    DEBUG_MSG("failed to create process!\n");
    return -1;
  }
  else if(pid == 0){
    /* child process */
    close(fd[0]);
    sleep(1);
    write(fd[1], test_msg, strlen(test_msg));
    DEBUG_MSG("The msg \"%s\" was written to another pipe!\n", test_msg);
    close(fd[1]);
    return 0;
  }
  else{
    /* parent process will read the msg from the child-side */
    close(fd[1]);
    while(1){
      rd_num = read(fd[0], rd_buf, sizeof(rd_buf));
      if(rd_num > 0){
        DEBUG_MSG("Got a msg:\"%s\" from another pipe!\n", rd_buf);
        memset(rd_buf, '\0', 256);
      }
    }

    close(fd[0]);
    return 0;
  }

  return 0;
}

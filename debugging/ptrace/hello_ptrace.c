/*
 * Monkey coder: Shawn the R0ck(C)2012
 * This "hello ptrace" program is GPL'ed!
 */

#include <stdio.h>
#include <stdlib.h> /* exit() */
#include <sys/types.h> /* pid_t */
#include <unistd.h> /* execl() */
#include <sys/wait.h> /* WIFSTOPPED */
#include <sys/ptrace.h> /* using ptrace() */

int main(int argc, char **argv[])
{
  pid_t pid, parent;
  int wait_ret;
  long long counter = 0;

  pid = fork();
  if(pid < 0){
    perror("fork()");
  }
  else if(pid == 0){
    /* start tracing */
    if(ptrace(PTRACE_TRACEME, pid, 0, 0) == -1){
      perror("ptrace()");
      exit(1);
    }
    /* execl() will send SIGTRAP signal to parent process in default.
     * read manual if you want to know more information! */
    execl("/bin/ls", "ls", 0);
    exit(1);
  }
  else{
    /* waiting for child process hung up */
    wait(&wait_ret);
    while(WIFSTOPPED(wait_ret)){ /* wait_ret == 1407 */
      if(ptrace(PTRACE_SINGLESTEP, pid, 0, 0) == -1){
        perror("ptrace()");
        exit(1);
      }
      wait(&wait_ret);
      ++counter;
    }
  }

  printf("instructions: %lld\n", counter);
  return 0;
}

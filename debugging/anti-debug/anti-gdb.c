/*
 * GPL'ed!
 */

#include <stdio.h>
#include <sys/ptrace.h> /* using ptrace() */
#include <stdlib.h>

int main(int argc, char **argv[])
{
  int status;

    /* start tracing, in most cases, it only fails when other process(
       such as, GDB) has already in wired */
    if(ptrace(PTRACE_TRACEME, 0, 0, 0) < 0){
      printf("don't try to debug me...\n");
      exit(1);
    }
  return 0;
}

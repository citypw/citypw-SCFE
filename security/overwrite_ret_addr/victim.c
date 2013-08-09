/*
 * MOnkey-coder: Shawn the R0ck@Copyleft
 * This sample code is GPL'ed!
 */
#include <stdio.h>
#include <string.h>

void test(const char *input)
{
  int x; /* integer variable need 4-bytes in memory */
  char buf[10]; /* buf[1024] for testing exploit */
  strcpy(buf, input);
  printf("%s \n", buf);
}

void fuck_me(void)
{
  printf("being hacked\n");
}

int main(int argc, char *argv[])
{
  printf("The address of func test(): %p, func fuck_me(): %p\n", test, fuck_me);
  test(argv[1]);
  return 0;
}

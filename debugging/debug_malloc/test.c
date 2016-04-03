/**
 * This code is GPL'ed!
 * monkey-coder:Shawn the R0ck <citypw@gmail.com>
 *
 * Come on, crack me on GNU/Linux!
 *
 */

#include <stdio.h>


int main(int argc, char* argv[])
{
	char *s1, *s2;

	s1 = (char*)malloc( sizeof(char) * 20);
	s2 = (char*)malloc( sizeof(char) * 40);

	free(s2);
        return 0;
}

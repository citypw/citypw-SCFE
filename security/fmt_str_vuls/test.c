#include <stdio.h>

void go(char *msg)
{
	int i = 0;
	char tmpbuf[512];
	
	printf("i = %p\n", &i);
	snprintf(tmpbuf, sizeof(tmpbuf), "helloworld\n");
	tmpbuf[ sizeof(tmpbuf) - 1] = '\0';

	printf("msg: %s, i = %d\n", tmpbuf, i);
}

int main(int argc, char *argv[])
{
	go(argv[1]);
	return 0;
}

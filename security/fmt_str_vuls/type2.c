#include <stdio.h>

/* Indeed, it's harder to find the potential threat by automated tools */
int error_msg(char *fmt)
{
	int i = 20;
	char buf[56];
	printf("i = %p\n", &i);
	strcpy(buf, fmt);
	printf(buf);
	printf("i = %d\n", i);
	return 0;
}

int main(int argc, char *argv[])
{
	error_msg(argv[1]);
	return 0;
}

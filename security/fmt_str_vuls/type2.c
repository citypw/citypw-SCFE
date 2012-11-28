#include <stdio.h>

int main(int argc, char **argv)
{
	char text[512];
	int i = 100;

	printf("\n&i = %08x, value = %d\n", &i, i);
	strcpy(text, argv[1]);
	printf("input: %s\n", text);
	printf(text);

	printf("\n&i = %08x, value = %d\n", &i, i);
	return (0);
}

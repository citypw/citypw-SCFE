#include <stdio.h>

int main(int argc, char *argv[])
{
	int x, y;
	printf("hello%n world%n\n", &x, &y);

	printf("%d %d\n", x, y);
	return 0;
}

#include <stdio.h>

int main()
{
	printf("%d %d\n", getuid(), geteuid());
	return 0;
}

/*
 * I have always forget the GNU inline assembly shit. So I need
 * re-study when I need to review some code every time.  
 */


#include <stdio.h>

int main()
{
	int x = 10, y =20;

	printf("x = %d, y = %d\n", x, y);
	__asm__ __volatile__("movl %1, %%eax\n\t"
				"movl %0, %1\n\t"
				"movl %%eax, %0\n\t"
				:"=r"(y),"=r"(x)
				:"0"(y),"1"(x)
				:"%eax");

	printf("x = %d, y = %d\n", x, y);

	return 0;
}

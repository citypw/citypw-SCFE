#include <stdio.h>

int main()
{
	char *p1 = "data";	//non-const pointer, non-const data
	const char *p2 = "data";	//non-const pointer, const data
	char *const p3 = "data";	//const pointer, non-const data
	const char *const p4 = "data";	//const pointer, const data

	p1[0] = 'x';		// valid
	p1 = "data1";		// valid, point to somewhere else

	// p2[0] = 'x'; is invalid
	p2 = "data2";		// valid

	p3[0] = 'x';		// valid
	// p3 = "data3"; is  invalid

	// p4[0] = 'x'; is invalid
	// p4 = "data4"; is invalid

	return 0;
}

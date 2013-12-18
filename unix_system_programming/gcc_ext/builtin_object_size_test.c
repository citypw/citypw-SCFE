#include <stdio.h>
#include <assert.h>

#pragma pack(1)

struct V{
	char buf[10];
	int x;
	char buf2[20]
};
int main()
{
	struct V v1;
          char *p = v1.buf;
          
	printf("%d\n", __builtin_object_size(p, 0));

	/* the object p points to is v1 */
	assert( __builtin_object_size(p, 0) == sizeof(v1));

	/* the object p points to is v1.buf */
	assert( __builtin_object_size(p, 1) == sizeof(v1.buf));

	return 0;
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

__attribute__ ((__nothrow__)) __my_memcpy_chk(void *__restrict __dest,
					      __const void *__restrict
					      __src, size_t __len,
					      size_t buf_len)
{
}

__attribute__ ((__nothrow__)) my_memcpy(void *__restrict __dest,
					__const void *__restrict __src,
					size_t __len)
{
	printf("%d\n", __builtin_object_size(__dest, 0));
	return __my_memcpy_chk(__dest, __src, __len,
			       __builtin_object_size(__dest, 0));
}

int main(int argc, char *argv[])
{
	char buf[20];
	char *p = buf;
	size_t buf_len, len;

	len = strlen(argv[1]);

	/* my memory copy should do */
	buf_len = __builtin_object_size(p, 0);

	if (buf_len < len) {
		fprintf(stderr,
			"buffer size:%ld, src size:%ld, buffer overflow detected!\n",
			buf_len, len);
		exit(1);
	}

	printf(" my_memcpy() done!\n");
	printf("%d\n", buf_len);
	return 0;
}

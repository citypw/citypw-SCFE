/**
 * This code is GPL'ed!
 * monkey-coder:Shawn the R0ck <citypw@gmail.com>
 *
 * The only thing I've already known is there's an half of address
 * become zero when you use -pie compilation option. Does the PIE
 * really work this way?
 */

#include <stdio.h>
#include <stdlib.h>
int main()
{
	FILE *fp;
	int ret;

	fp = fopen("/dev/urandom", "r");
	if(fp == NULL){
		fprintf(stderr, "fp is null\n");
		exit(1);
	}

	int reslen = fread(&ret, sizeof(int), 1, fp);

	printf("0x%x\n", ret);

	fclose(fp);

	ret = 0;
	unsigned char *p = (unsigned char*) &ret;
	p[ sizeof(ret) - 1] = 255;
	p[ sizeof(ret) -2] = '\n';

	printf("0x%x\n", ret);

	return 0;
}

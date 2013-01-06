#include <stdio.h>

#define CRYPTO_FIPS "/proc/sys/crypto/fips_enabled"

int main(int argc, char *argv[])
{
	FILE *fp;
	char c;

	fp = fopen( CRYPTO_FIPS, "r");
	if( fp == NULL)
	{
		printf("The OS doesn't support kernel crypto module!\n");
		return 1;
	}

	if(fread(&c, sizeof(c), 1, fp) <= 0){
		printf("failed to read from: %s\n", CRYPTO_FIPS);
		fclose( fp);
		return 1;
	}

	printf("FIPS_MODE: %c\n", c);
	fclose(fp);
	return 0;
}

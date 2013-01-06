#include <stdio.h>
#include <openssl/crypto.h>

#define CRYPTO_FIPS "/proc/sys/crypto/fips_enabled"

int main(int argc, char *argv[])
{
        int mode;
        FILE *fp;

	fp = fopen( CRYPTO_FIPS, "r");
	if( fp == NULL)
	{
		printf("The OS doesn't support kernel crypto module!\n");
		return 1;
	}

        if(argc != 2)
        {
                printf("Usage: %s [mode: 0, 1]\n", argv[0]);
                return 1;
        }

        mode = atoi( argv[1]);
        printf("Current FIPS mode: %d\n", FIPS_mode());

        FIPS_mode_set( mode);
        mode = FIPS_mode();
        printf("After setting, current FIPS mode is: %d\n", mode);

        fclose( fp);
        return 0;
}

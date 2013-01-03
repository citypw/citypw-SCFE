/*
 * Example of how to use MD5 functions to generate hash shit via libcrypto
 * GPL'ed!
 */
#include <stdio.h>
#include <string.h>
#include <openssl/md5.h>

int main(int argc, char *argv[])
{
        MD5_CTX prog_md5;
        MD5_CTX prog2_md5;
        char prog_hash[17] = {0};
        char prog_hash2[17] = {0};
        int i;

        if( argc != 3)
        {
                printf("Usage:\n\t%s <prog> <prog2>\n", argv[0]);
                return 1;
        }

        MD5_Init(&prog_md5);
        MD5_Update(&prog_md5, argv[1], strlen(argv[1]));
        MD5_Final(prog_hash, &prog_md5);

        MD5_Init(&prog2_md5);
        MD5_Update(&prog2_md5, argv[2], strlen(argv[2]));
        MD5_Final(prog_hash2, &prog2_md5);

        for( i = 0; i < 16; i++){
                if(prog_hash[i] != prog_hash2[i]){
                        printf("Two values are not the same.\n%s: %s\n%s: %s\n", argv[1], prog_hash, argv[2], prog_hash2);
                        return 1;
                }
        }
        printf("Two values are the same: %s: %s\n", argv[1], prog_hash);
        return 0;
}

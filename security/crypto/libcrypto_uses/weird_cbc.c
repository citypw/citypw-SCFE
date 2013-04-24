#include <stdio.h>
#include <string.h>
#include <openssl/des.h>

/* 42 bytes data + 6 bytes padding */
static char *data ="Yesterday's enemy becomes today's bitch!@#";
//static unsigned char cbc_key[8] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};
static unsigned char cbc_key[8] = {0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef};
static unsigned char cbc_iv[8] = {0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10};

int main(int argc, char *argv[])
{
        DES_key_schedule ks;
        DES_cblock iv;
        int len = strlen(data);
        int ret, i;
        unsigned char buf[256], cbc_out[256];

        ret = DES_set_key_checked(&cbc_key, &ks);
        if(ret != 0){
                fprintf(stderr, "key error %d\n", ret);
                return -1;
        }

        memset(buf, 0x00, sizeof(buf));
        memset(cbc_out, 0x00, sizeof(cbc_out));
        memcpy(iv, cbc_iv, sizeof(cbc_iv));
        des_cbc_encrypt(data, cbc_out, strlen(data) + 1, ks, &iv, DES_ENCRYPT);
        
        printf("Ciphertext: ");
        for( i =0;i < strlen(cbc_out); i++)
                printf("%x", cbc_out[i]);
        printf("\n%d bytes!\nDecrypting now...\n", i);

        des_cbc_encrypt(cbc_out, buf, strlen(cbc_out) + 1, ks, &iv, DES_DECRYPT);

        printf("Plaint text: %s\n", buf);
        return 0;
}

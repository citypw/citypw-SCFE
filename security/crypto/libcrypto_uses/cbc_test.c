#include <stdio.h>
#include <openssl/evp.h>
#include <string.h>

/* 42 bytes data + 6 bytes padding */
static char *data ="Yesterday's enemy becomes today's bitch!@#";

static unsigned char key[8] = {0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef};
static unsigned char iv[8] = {0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10};

#define EVP_DES_CBC EVP_des_cbc()

int main(int argc, char *argv[])
{
        EVP_CIPHER_CTX ctx;
        int ret, update_len, final_len, i;
        unsigned char buf[256], cbc_out[256];

        memset(buf, 0x00, sizeof(buf));
        memset(cbc_out, 0x00, sizeof(cbc_out));

        EVP_CIPHER_CTX_init(&ctx);
        ret = EVP_EncryptInit_ex(&ctx, EVP_DES_CBC, NULL, key, iv);
        if(ret == 0){
                fprintf(stderr, "EVP_EncryptInit_ex() failed!\n");
                return -1;
        }

        ret = EVP_EncryptUpdate(&ctx, cbc_out, &update_len, data, strlen(data));
        if(ret == 0){
                fprintf(stderr, "EVP_EncryptUpdate() failed!\n");
                return -1;
        }

        ret = EVP_EncryptFinal_ex(&ctx, cbc_out + update_len, &final_len);
  
        final_len += update_len;
        printf("Ciphertext is %d bytes: ", final_len);
        for( i =0;i < final_len; i++)
                printf("%.02x", cbc_out[i]);
        printf("\nDecrypting now");

        EVP_DecryptInit_ex(&ctx, EVP_DES_CBC, NULL, key, iv);

        ret = EVP_DecryptUpdate(&ctx, buf, &update_len, cbc_out, final_len);
        if(ret == 0){
                fprintf(stderr, "EVP_DecryptUpdate() failed!\n");
                return -1;
        }

        ret = EVP_DecryptFinal_ex(&ctx, buf + update_len, &final_len);
        if(ret == 0){
                fprintf(stderr, "EVP_DecryptFinal_ex() failed!\n");
                return -1;
        }

        final_len += update_len;
        buf[ final_len] = '\0';

        printf("plaintext is: %s\n", buf);
        return 0;
}

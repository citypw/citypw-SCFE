/* This is a pretty simple xor encryption/decryption shit! Plz read
   the frenk'in source code!

   btw: It's GPL'ed!  
*/

#include <stdio.h>

int main(int argc, char *argv[])
{
        FILE *fi, *fo;
        char *cp = argv[1];
        int c;

        /* key is very important */
        if(argc != 3){
                printf("Usage: %s [key] [input-file]\n", argv[0]);
                return -1;
        }

        fi = fopen(argv[2], "rb");
        if( *cp == '\0' || fi == NULL)
        {
                fclose( fi);
                return -1;
        }

        fo = fopen("output.log", "wb");
        if( fo == NULL)
        {
                fclose( fo);
                return -1;
        }

        /* here we go */
        while(( c = fgetc(fi)) != EOF){
                if( *cp == '\0')
                        cp = argv[1];

                c ^= *(cp++);
                fputc(c, fo);
        }

        fclose(fo);
        fclose(fi);

        return 0;
}

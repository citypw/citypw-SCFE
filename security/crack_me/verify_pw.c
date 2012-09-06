/**
 * This code is GPL'ed!
 * monkey-coder:Shawn the R0ck <citypw@gmail.com>
 *
 * Come on, crack me on GNU/Linux!
 *
 */

#include <stdio.h>

#define PASSWD "12345678"

int verify_pw(char *pw)
{
        int auth;
        auth = strcmp(pw, PASSWD);

        return auth;
}

int main(int argc, char* argv[])
{
        int flag = 0;
        char pw[1024];

        while(1)
        {
                printf("input ur password:\n");
                scanf("%s", pw);

                flag = verify_pw(pw);
                if(flag)
                {
                        printf("incorrect!\n");
                }
                else
                {
                        printf("bingo!\n");
                        break;
                }
        }

        return 0;
}

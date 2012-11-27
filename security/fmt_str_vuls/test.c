#include <stdio.h>

int main(void)
{
int x = 5;
char buffer[52];
printf("&x = %pn",&x);
fgets(buffer,52,stdin);
printf(buffer),
printf("x = %dn",x);
}

#include <stdio.h>
#include <string.h>
void evilfunction(char* input)
{
char buffer[1000];
strcpy(buffer, input); // Vulnerable function.
}
int main(int argc, char** argv)
{
evilfunction(argv[1]);
return 0;
}


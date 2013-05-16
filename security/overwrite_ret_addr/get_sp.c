#include <stdio.h>

unsigned int get_sp(void){
	__asm__("movl %esp, %eax");
}

void print_esp()
{
	printf("Stack pointer (ESP): 0x%x\n", get_sp());
}
int main(){
	print_esp();
}

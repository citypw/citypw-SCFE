/**
 * This code is GPL'ed!
 * monkey-coder:Shawn the R0ck <citypw@gmail.com>
 */

#include <stdio.h>



int i;

void test1(){
  printf("running in test1!\n");
}

int test2(){
  int p[2];

  for(i=0;i<5;i++){
    p[i] = i;
    printf("%d\n",p[i]);
  }
  return 0;
}

int main(int argc,char *argv[]){

  test1();
  test2();
  printf("end of the program...\n");

  return 0;
}

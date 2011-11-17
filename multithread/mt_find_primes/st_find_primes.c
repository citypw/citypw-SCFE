#include <stdio.h>
#include <omp.h>

unsigned count =0;
int is_prime(unsigned int x)
{
	int i = 2, ret = 0;
	for(;i < x && !ret; i++){
		if(x % i == 0)
			ret =1;
	}

	if(!ret){
		count++;
	//	printf("%d is a prime!\n",x);
		return 0;
	}
	else
		//printf("%d not a prime!\n",x);

	return -1;
}

int main()
{
	int i;
#pragma omp parallel for
	for(i=0; i < 200000;i++)
		is_prime(i);

	printf("There are %d prime numbers in range from 0 to 100,000!\n",count);
}

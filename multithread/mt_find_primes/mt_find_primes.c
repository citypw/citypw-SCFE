#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

unsigned count=0,count1 =0,count2=0,count3=0;

pthread_mutex_t my_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t my_cond;

#define THREAD1_FIND_MAX 90000 /* 0 -- 50000 */
#define THREAD2_FIND_MAX 140000 /* 50000 -- 80000 */
#define THREAD3_FIND_MAX 200000 /* 80000 -- 100000 */

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

void *thread1_find(void *arg){
  int i;
  for(i=0; i< THREAD1_FIND_MAX;i++)
    is_prime(i);
}

void *thread2_find(void *arg){
  int i;
  for(i=THREAD1_FIND_MAX;i<THREAD2_FIND_MAX;i++)
    is_prime(i);
}

void *thread3_find(void *arg){
  int i;
  for(i=THREAD2_FIND_MAX;i< THREAD3_FIND_MAX;i++)
    is_prime(i);
}
int main()
{
  pthread_t tid1,tid2,tid3;

  if(pthread_create(&tid1,NULL,thread1_find,NULL)){
    fprintf(stderr,"error creating thread.");
    exit(1);
  }

  if(pthread_create(&tid2,NULL,thread2_find,NULL)){
    fprintf(stderr,"error creating thread.");
    exit(1);
  }
  if(pthread_create(&tid3,NULL,thread3_find,NULL)){
    fprintf(stderr,"error creating thread.");
    exit(1);
  }

  if(pthread_join(tid1,NULL)){
    fprintf(stderr,"error joining thread.\n");
    exit(1);
  }

  if(pthread_join(tid2,NULL)){
    fprintf(stderr,"error joining thread.\n");
    exit(1);
  }

  if(pthread_join(tid3,NULL)){
    fprintf(stderr,"error joining thread.\n");
    exit(1);
  }
  printf("There are %d prime numbers in range from 0 to 100,000!\n",count);
}

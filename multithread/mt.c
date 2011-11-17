#include <pthread.h>
#include <iostream>

using namespace std;

pthread_cond_t taxiCond; 

int traveler_count = 0;

 // 同步锁
 pthread_mutex_t taxiMutex, traveler_mutex;


 // 旅客到达等待出租车
 void * traveler_arrive(void * name) { 
    cout<< " Traveler: " <<(char *)name<< " needs a taxi now! " <<endl; 

    pthread_mutex_lock(&traveler_mutex); 
    traveler_count++;
    pthread_cond_wait (&taxiCond, &taxiMutex); 
    pthread_mutex_unlock (&traveler_mutex); 

    cout<< " Traveler: " << (char *)name << " now got a taxi! " <<endl; 
    pthread_exit( (void *)0 ); 
 } 

 // 出租车到达
 void * taxi_arrive(void *name) { 
    cout<< " Taxi " <<(char *)name<< " arrives. " <<endl; 

    pthread_mutex_lock(&taxiMutex);
    while(1){
	    if(traveler_count > 0){
//		    cout<<"traveler count:"<<traveler_count<<endl;
	    pthread_cond_signal(&taxiCond);
	    traveler_count--;
	    pthread_mutex_unlock(&taxiMutex);
	    break;
	    }
    }
    pthread_exit( (void *)0 ); 
 } 

int main() {  
    // 初始化
    pthread_mutexattr_t mutex_attr;
    pthread_mutexattr_init(&mutex_attr);

    taxiCond= PTHREAD_COND_INITIALIZER; 
    pthread_t thread; 
    pthread_attr_t threadAttr; 
    pthread_attr_init(&threadAttr);

    pthread_mutexattr_init(&mutex_attr);
    pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_RECURSIVE_NP);

    pthread_mutex_init(&taxiMutex, &mutex_attr);
    pthread_mutex_init(&traveler_mutex, &mutex_attr);

    pthread_create(&thread, & threadAttr, taxi_arrive, (void *)( " Jack " )); 
    sleep(1); 
    pthread_create(&thread, &threadAttr, traveler_arrive, (void *)( " Susan1 " )); 
    pthread_create(&thread, &threadAttr, traveler_arrive, (void *)( " Susan2 " )); 
    pthread_create(&thread, &threadAttr, traveler_arrive, (void *)( " Susan3 " )); 
    sleep(1); 
    pthread_create(&thread, &threadAttr, taxi_arrive, (void *)( " Mike " )); 
    pthread_create(&thread, &threadAttr, taxi_arrive, (void *)( " Smith " )); 
    sleep(1); 

    return 0; 
 }

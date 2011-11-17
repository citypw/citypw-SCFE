/*
 * 5 philosophers:A,B,C,D,E
 */
/*
         cp1        cp2
           \ shawn /
             \   /
        fug  / | \ ute
           /   |   \
          /sam  | rms \
       cp3     cp4    cp5
*/

#include "n-philosophers.h"

/*
 * thinking_init - initialization of group of philosophers
 * @ph: the pointer that point to a struct
 *
 * this function is suck, no exception checking and no return
 **/
void thinking_init(struct philosopher *ph)
{
    int i;
    for (i = 0; i < NR; i++) {
	memset((ph + i)->hands, 0x00, sizeof((ph + i)->hands));
	(ph + i)->status = THINKING;
	(ph + i)->ith = i;
	printf("%p = status %d %d\n", (ph + i), (ph + i)->status,
	       (ph + i)->hands[0]);
    }
}

/* print out those states we might get: thinking, eating, hungry*/
void thinking(struct philosopher *ph)
{
  printf("%dth philosopher is thinking now!\n",ph->ith);
}

void eating(struct philosopher *ph)
{
  printf("%dth philospher is eating now!\n",ph->ith);
}

void hungry(struct philosopher *ph)
{
  printf("%dth philospher is still hungry! S O S...........\n",ph->ith);
}


/*
 * try_eating - thread function
 * @arg: the pointer which point to an struct
 *
 * it seems should return a void pointer, but it's not actually.
 * This function is going be a dead loop unless you interrupt it,
 * manually.
 */
void *try_eating(void *arg)
{
    /* get which philosopher reaching here */
    struct philosopher *ph = (struct philosopher *) arg;
    int i;

    /* time seconds as our paramaters for random number */
    srand(time(0));
    while (1) {
      if (count >= NR)
	    count = 0;

      /* lock it up */
      pthread_mutex_lock(&cp_mutex[count]);
      i = rand()%NR;

      thinking(ph+i);
      pthread_mutex_unlock(&cp_mutex[count]);

      phv_index[i] = i;

      /* check the philosopher's location */
      if ((ph + phv_index[i])->ith == 0) {

	pthread_mutex_lock(&cp_mutex[count]);
          if (cs_shared[NR - 1] == 0 && cs_shared[(ph + phv_index[i])->ith] == 0) {

		    cs_shared[NR - 1] = 1;
		    cs_shared[0] = 1;
		    pthread_mutex_unlock(&cp_mutex[count]);

		    eating(ph + phv_index[i]);

		    pthread_mutex_lock(&cp_mutex[count]);
		    cs_shared[NR - 1] = 0;
		    cs_shared[0] = 0;
		    pthread_mutex_unlock(&cp_mutex[count]);
		
          } 
	  else {
	    hungry(ph + phv_index[i]);
	    cs_shared[NR - 1] = 0;
	    cs_shared[0] = 0;

	    pthread_mutex_unlock(&cp_mutex[count]);
          }
      }
        else {
	  pthread_mutex_lock(&cp_mutex[1]);
          if (cs_shared[(ph + phv_index[i])->ith - 1] == 0 && cs_shared[(ph + phv_index[i])->ith] == 0) {

		    cs_shared[(ph + phv_index[i])->ith - 1] = 1;
		    cs_shared[(ph + phv_index[i])->ith] = 1;
		    eating(ph + phv_index[i]);

		    pthread_mutex_unlock(&cp_mutex[1]);

		    pthread_mutex_lock(&cp_mutex[1]);
		    cs_shared[(ph + phv_index[i])->ith - 1] = 0;
		    cs_shared[(ph + phv_index[i])->ith] = 0;
		  
		    pthread_mutex_unlock(&cp_mutex[1]);
	
		} 
          else {
	    hungry(ph + phv_index[i]);
	    cs_shared[(ph + phv_index[i])->ith - 1] = 0;
	    cs_shared[(ph + phv_index[i])->ith] = 0;

	    pthread_mutex_unlock(&cp_mutex[1]);
          }
        }
      sleep(1);
    }
}

int main()
{
    int i;

    struct philosopher *ph_p =
	(struct philosopher *) malloc(sizeof(struct philosopher) * NR);

    thinking_init(ph_p);

    /* creating NR threads */
    for (i = 0; i < NR; i++) {
	if (pthread_create(&(ph_p + i)->tid, NULL, try_eating, ph_p)) {
	    fprintf(stderr, "error creating thread %d.\n", i);
	    exit(1);
	}
	printf("tid:%d\n", (ph_p + i)->tid);
    }

    while (1)
	sleep(5);

    return 0;
}

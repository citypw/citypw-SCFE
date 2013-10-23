#include <stdio.h>
#include <sched.h>

int get_sched_policy()
{
	int policy;
	/* get our scheduling policy */
	policy = sched_getscheduler(0);
	switch (policy) {
	case SCHED_OTHER:
		printf("Policy is normal\n");
		break;
	case SCHED_RR:
		printf("Policy is round-robin\n");
		break;
	case SCHED_FIFO:
		printf("Policy is first-in, first-out\n");
		break;
	case -1:
		perror("sched_getscheduler");
		break;
	default:
		fprintf(stderr, "Unknown policy!\n");

		return -1;
	}
	return policy;

}
int main()
{
	int ret;
	struct sched_param sp;

	ret = get_sched_policy();

	printf("Tyring to set a new policy: RR?\n");
	sp.sched_priority = 2;

	ret = sched_setscheduler(0, SCHED_RR, &sp);
	if( ret == -1)
		perror("sched_setpriority");

	printf("The current policy is....\n");
	ret = get_sched_policy();	
	
	return 0;
}

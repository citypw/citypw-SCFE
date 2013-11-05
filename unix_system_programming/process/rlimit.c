#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>

int main()
{
	struct rlimit rlim;
	int ret;
/* get the limit on core sizes */
	ret = getrlimit(RLIMIT_CORE, &rlim);
	if (ret == -1) {
		perror("getrlimit");
		return 1;
	}
	printf("RLIMIT_CORE limits: soft=%ld hard=%ld\n",
	       rlim.rlim_cur, rlim.rlim_max);

	/* set the limit on core sizes */
	rlim.rlim_cur = 16 * 1024 * 1024; /* 16MB */
	rlim.rlim_max = RLIM_INFINITY; /* -1 */
	ret = setrlimit( RLIMIT_CORE, &rlim);
	if( ret == -1){
		perror("setrlimit");
		return 1;
	}

	printf("RLIMIT_CORE limits: soft=%ld hard=%ld\n",
	       rlim.rlim_cur, rlim.rlim_max);
}

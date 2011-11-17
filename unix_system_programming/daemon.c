#include <stdio.h>
#include <unistd.h>
#include <sys/param.h>
#include <signal.h>

void init_daemon()
{
	int pid;
	int i;
	int max_fd=64;

	if((pid = fork()) != 0)
		exit(0);

	setsid();
	signal(SIGHUP, SIG_IGN);

	if((pid = fork()) != 0)
		exit(0);

	for(i =3; i < max_fd; i++)
		close(i);
	printf("%d\n",i);

	chdir("/");
	umask(0);

}

int main()
{
	FILE *fp;
	init_daemon();

	while(1){
		sleep(2);
		if((fp = fopen("/root/test.log","a")) >=0)
		{
			fprintf(fp, "stcik into...\n");
			fclose(fp);
		}

		fprintf(stderr,"shitshit\n");
	}
	return 0;
}

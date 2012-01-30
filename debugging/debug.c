#include <stdio.h>
#include <dirent.h>

#define PID_FILE  "/tmp/wlogd.pid"


#define DEBUG_ERR(format, args...) fprintf(stderr, format, ##args)

#define DEBUG_MSG(string) do{ \
	DEBUG_ERR("File: %s, Function: %s, Line: %d\nDescription: ", __FILE__, __FUNCTION__, __LINE__); \
	DEBUG_ERR(string); \
}while(0);

int main(int argc, char *argv[])
{
	FILE *fp;
	DIR *dir;
	char proc_buf[128] = {0};
	unsigned int pid = 0;

	fp = fopen(PID_FILE, "r");
	if(fp == NULL){
		DEBUG_MSG("fp is null\n");
		return 0;
	}

	fscanf(fp, "%u\n", &pid);
	fclose(fp);

	sprintf(proc_buf, "/proc/%d", pid);

	printf("pid : %d\n",pid);
	if((dir = opendir(proc_buf)) == NULL){
		DEBUG_MSG("can't open dir!\n");
		return 0;
	}

	return 0;
}

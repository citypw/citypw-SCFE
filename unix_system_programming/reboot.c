#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sysinfo.h>
#include <string.h>
#include <linux/reboot.h> 

int main()
{
	reboot(LINUX_REBOOT_CMD_RESTART); 
	return 0;
}

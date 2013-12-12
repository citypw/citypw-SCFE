#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

uid_t runner_uid;
uid_t runner_gid;
uid_t owner_uid;
uid_t owner_gid;

void init()
{
	runner_uid = getuid();
	runner_gid = getgid();
	owner_uid = geteuid();
	owner_gid = getegid();
}

void print_info()
{

	printf
	    ("runner_uid:%d, runnder_gid:%d, owner_uid:%d, owner_gid:%d\n",
	     runner_uid, runner_gid, owner_uid, owner_gid);
}

int test_priv()
{
	int fd;
	fd = open("/proc/kcore", O_RDONLY | O_EXCL, 0644);
	if (fd == -1) {
		perror("open");
		if (errno == EACCES)
			printf("non-root now...\n");
		close(fd);
		return -1;
	}
	printf("root now...\n");
	close(fd);

	return 0;
}

int main()
{
	int fd;

	init();
	printf("original version:......\n");
	print_info();

	printf("set*id(0)...\n");
	setuid(0);
	setgid(0);
	seteuid(0);
	setegid(0);

	print_info();
	test_priv();
	printf("drop privileges for a moment......\n");
	setreuid(owner_uid, runner_uid);
	setregid(owner_gid, runner_gid);

	print_info();
	test_priv();
	printf("take priviledges back...\n");
	if (setreuid(runner_uid, owner_uid)
	    || setregid(runner_gid, owner_gid))
		perror("setreuid");

	print_info();
	test_priv();
	return 0;
}

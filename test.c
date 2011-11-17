#include <stdio.h>
#include <getopt.h>

struct option longopts[] = {
	{"debug", 0, NULL, 'd'},
	{"help", 0, NULL, 'h'}
};

static unsigned short DEBUG_ON = 0;

int main(int argc, char *argv[])
{
	int ch;

	while ((ch = getopt_long(argc, argv, "dh", longopts, NULL)) != -1) {
			switch (ch) {
		case 'd':
			DEBUG_ON =1;
			printf("defined debug!\n");
			break;
		case 'h':
		  printf("Usage:%s [OPTION]\nShort options:\n   -h, --help\n   -d, --debug   Run the program without daemon\n", argv[0]);
			break;
		}
	}

	if(DEBUG_ON)
	printf("debuging...\n");

	return 0;
}

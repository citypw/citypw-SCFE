/*
 * http://pentest.cryptocity.net/files/code_analysis/demott_source_code_auditing.pdf
 * 
 */
#include <syslog.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFLEN 16
#define WORDSIZE 2
#define DWORDSIZE WORDSIZE + WORDSIZE

void mylog(int kind, char *msg){
	syslog(LOG_USER | kind, msg);
}

void mycpy(char *dst, char *src){
	if( strlen(src) < BUFLEN - 1)
		while( *src)
			*dst++ = *src++;
	*dst = '\x00';
}

int main(int argc, char *argv[])
{
	char buf1[16];
	char buf2[16];
	char buf3[BUFLEN];
	char *buf4;
	char *buf5;
	char buf6[16];
	char *buf7;
	int i, len;

	if( argc != 12)
		exit(0);

	/* [1] off-by-one */
	strncpy(buf1, argv[1], sizeof(buf1));
	len = atoi(argv[2]);

	if(len < 16)
		memcpy(buf2, argv[3], len); /* [2] if len is -4, which is equal to signed integer 4294967292*/
	else{ 
		char *buf = malloc(len + 20);
		if( buf){
			snprintf(buf, len + 20, "String too long: %s", argv[3]);
			mylog(LOG_ERR,buf); /* [3] string fmt vuln */ 
		}
	}

	mycpy(buf3, argv[4]); 
	/* [4] off-by-one without NULL termination, here's strncat not *cpy */
	strncat(buf3, argv[5], sizeof(buf3) - 1);
	if( fork()){
		execl("/bin/ls", "/bin/ls", argv[6], 0);
	}

	// filter metacharacters
	char *p;
	if( p = strchr(argv[7], '&'))
		*p = 0;
	if( p = strchr(argv[7], '`'))
		*p = 0;
	if( p = strchr(argv[7], ';'))
		*p = 0;
	if( p = strchr(argv[7], '|'))
		*p = 0;
	/* [5] we can control the input */
	if(strlen(argv[7]) < 1024){
		buf4 = malloc(20 + strlen(argv[7]));
		sprintf(buf4, "/bin/cat %s", argv[7]);
		system(buf4);
	}

	/* [6] should do validate the input, a[8]+a[9]+2 = 0? damn...*/
	buf5 = malloc(strlen(argv[8]) + strlen(argv[9]) + 2);
	strcpy(buf5, argv[8]);
	strcat(buf5, argv[9]);
	/* [7] buf6 is on the stack...overflow motherfucker */
	memcpy(buf6, argv[10], strlen(argv[10]));
	buf7 = malloc(4 * DWORDSIZE);

	/* [8] buf7 will overflow... 4 * 2 + 2 = 10..boom... */
	for( i = 0; i<4; i++){
		memcpy(buf7 + 4 * i, argv[11] + 4 * i, DWORDSIZE);
	}

	printf("\nGot %s, (%d) %s, %s, %s, %s, %s, %s\n", buf1,len, buf2, buf3, buf4, buf5, buf6, buf7);

	return 0;
}

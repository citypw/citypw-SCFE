/*
 * Copyright (c) 2010 HackerFellowship
 *
 * Authors: Shawn the R0ck
 * Date: Oct 12th,2010
 *
 * Purpose:
 *
 * This program will show you a simple case how to use fck_des.You can
 * go to input 2 strings as parameters and compare those 2 whether
 * equal or not.
 *
 * This program is free software;you can redistribute it and /or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundataion;either version 3 of the
 * License,or (at your option) any later version.  */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sample_en.h"

/**
 * $main: generates 2 md5-key by using 2 strings
 * @argv[1],@argv[2]:input strings
 *
 * if succeed,return 0.
 */
int main(int argc, char *argv[])
{
	int i;
	unsigned char buf[16], buf2[16];
	memset(buf, '0', 16);
	memset(buf2, '0', 16);

	if (argc != 3) {
		printf("usage:test s1 s2\n");
		exit(1);
	}
	fck_des(argv[1]);
	strncpy(buf, final_md5buf, 16);
        memset(final_md5buf, 0x00, 16);

	fck_des(argv[2]);
	strncpy(buf2, final_md5buf, 16);

	if (strncmp(buf, buf2, 16) == 0)
		printf("match, dude!\n");
	else
		printf("cant match\n");

        printf("s1: %s\nMD5: ", argv[1]);
	for (i = 0; i < 16; i++)
		printf("%x", buf[i]);
	printf("\n");

        printf("s2: %s\nMD5: ", argv[2]);
	for (i = 0; i < 16; i++)
		printf("%x", buf2[i]);
	printf("\n");
}

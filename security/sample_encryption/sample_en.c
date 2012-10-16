/*
 * Copyright (c) 2010 HackerFellowship
 *
 * Authors: Shawn the R0ck
 * Date: Oct 9th,2010
 *
 * Purpose: 
 *
 * This program is to generate sort of codes that will not be human
 * readable.The code look suck but it works.  The program takes input
 * by something(17 bytes of Mac address and 13 bytes serial number)
 * that will generates 90 unreadable characters at first.That process
 * of generate stuff is a symmetric algorithm.Then the program going
 * to use a simplified md5 implementation that was written by
 * MIT.Finnally,we get a md5 string.
 *
 *
 * This program is free software;you can redistribute it and /or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundataion;either version 3 of the
 * License,or (at your option) any later version. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sample_en.h"

/* we have 30 bytes length array and to generates 90 bytes
   characters. */

#define NR_GENS 30
#define MAX_BUF NR_GENS*3

unsigned char buf[MAX_BUF + 1];

void generator(char *arg)
{
	int i, buf_offset = 2;
	int count = 0, count_for_semi = 0;
	int c_minus = 0;


	buf[0] = '-';
	buf[1] = 's';

	/* a char respect with a 3-characters cipher */
	for (i = 0; i < NR_GENS; i++) {
		switch (arg[i]) {
		case '0':
			if (count++ == 0) {
				buf[buf_offset++] = '@';
				buf[buf_offset++] = 'u';
				buf[buf_offset++] = '/';
			} else {
				buf[buf_offset++] = '#' + count;
				buf[buf_offset++] = '/' + count;
				buf[buf_offset++] = '9' + count;
			}
			break;
		case '1':
			buf[buf_offset++] = 'm';
			buf[buf_offset++] = 'Q';
			buf[buf_offset++] = '0';
			break;
		case '2':
			buf[buf_offset++] = 'G';
			buf[buf_offset++] = 'i';
			buf[buf_offset++] = 'f';
			break;
		case '3':
			buf[buf_offset++] = 'B';
			buf[buf_offset++] = 'x';
			buf[buf_offset++] = 'g';
			break;
		case '4':
			buf[buf_offset++] = 'Y';
			buf[buf_offset++] = '*';
			buf[buf_offset++] = '6';
		case '5':
			buf[buf_offset++] = '^';
			buf[buf_offset++] = 'n';
			buf[buf_offset++] = '1';
			break;
		case '6':
			buf[buf_offset++] = 'J';
			buf[buf_offset++] = 'I';
			buf[buf_offset++] = 'K';
			break;
		case '7':
			buf[buf_offset++] = '+';
			buf[buf_offset++] = 'l';
			buf[buf_offset++] = 'S';
			break;
		case '8':
			buf[buf_offset++] = '1';
			buf[buf_offset++] = 's';
			buf[buf_offset++] = 'h';
			break;
		case '9':
			buf[buf_offset++] = 't';
			buf[buf_offset++] = 'b';
			buf[buf_offset++] = 'P';
			break;
		case 'A':
			buf[buf_offset++] = 'L';
			buf[buf_offset++] = 'F';
			buf[buf_offset++] = '&';
			break;
		case 'B':
			buf[buf_offset++] = 'V';
			buf[buf_offset++] = 'T';
			buf[buf_offset++] = 'w';
			break;
		case 'C':
			buf[buf_offset++] = 'F';
			buf[buf_offset++] = 'P';
			buf[buf_offset++] = 'y';
			break;
		case 'D':
			buf[buf_offset++] = 'c';
			buf[buf_offset++] = '%';
			buf[buf_offset++] = 'R';
			break;
		case 'E':
			buf[buf_offset++] = '0';
			buf[buf_offset++] = 'S';
			buf[buf_offset++] = 'i';
			break;
		case 'F':
			buf[buf_offset++] = 'e';
			buf[buf_offset++] = 'Z';
			buf[buf_offset++] = 'U';
			break;
		case 'G':
			buf[buf_offset++] = '%';
			buf[buf_offset++] = 'p';
			buf[buf_offset++] = '3';
			break;
		case 'H':
			buf[buf_offset++] = 'A';
			buf[buf_offset++] = '2';
			buf[buf_offset++] = 'E';
			break;
		case 'I':
			buf[buf_offset++] = '2';
			buf[buf_offset++] = 'b';
			buf[buf_offset++] = 'W';
			break;
		case 'J':
			buf[buf_offset++] = '5';
			buf[buf_offset++] = 'f';
			buf[buf_offset++] = 'S';
			break;
		case 'K':
			buf[buf_offset++] = '9';
			buf[buf_offset++] = 'i';
			buf[buf_offset++] = 'H';
			break;
		case 'L':
			buf[buf_offset++] = 'O';
			buf[buf_offset++] = '+';
			buf[buf_offset++] = '3';
			break;
		case 'M':
			buf[buf_offset++] = 'w';
			buf[buf_offset++] = 'Q';
			buf[buf_offset++] = 'q';
			break;
		case 'N':
			buf[buf_offset++] = 'n';
			buf[buf_offset++] = 'O';
			buf[buf_offset++] = '8';
		case 'O':
			buf[buf_offset++] = 'M';
			buf[buf_offset++] = '-';
			buf[buf_offset++] = '2';
			break;
		case 'P':
			buf[buf_offset++] = '#';
			buf[buf_offset++] = 'N';
			buf[buf_offset++] = 'v';
			break;
		case 'Q':
			buf[buf_offset++] = 'l';
			buf[buf_offset++] = '$';
			buf[buf_offset++] = '5';
			break;
		case 'R':
			buf[buf_offset++] = 'B';
			buf[buf_offset++] = 'M';
			buf[buf_offset++] = '/';
			break;
		case 'S':
			buf[buf_offset++] = 'H';
			buf[buf_offset++] = '-';
			buf[buf_offset++] = 'B';
			break;
		case 'T':
			buf[buf_offset++] = 'P';
			buf[buf_offset++] = 'H';
			buf[buf_offset++] = '0';
			break;
		case 'U':
			buf[buf_offset++] = '8';
			buf[buf_offset++] = 'C';
			buf[buf_offset++] = 'K';
			break;
		case 'V':
			buf[buf_offset++] = 'x';
			buf[buf_offset++] = 'X';
			buf[buf_offset++] = 'x';
			break;
		case 'W':
			buf[buf_offset++] = 'Q';
			buf[buf_offset++] = '8';
			buf[buf_offset++] = '6';
			break;
		case 'X':
			buf[buf_offset++] = 'S';
			buf[buf_offset++] = 'e';
			buf[buf_offset++] = 'B';
			break;
		case 'Y':
			buf[buf_offset++] = 'Y';
			buf[buf_offset++] = 'T';
			buf[buf_offset++] = '&';
			break;
		case 'Z':
			buf[buf_offset++] = '+';
			buf[buf_offset++] = 'z';
			buf[buf_offset++] = 'g';
			break;
		case ':':
			buf[buf_offset++] = 'M' + count_for_semi;
			buf[buf_offset++] = 'Q' + count_for_semi++;
			break;
		case '-':
			buf[buf_offset++] = 'p' + count_for_semi;
			buf[buf_offset++] = '0' + count_for_semi++;
			break;
		default:
			buf[buf_offset++] = '|';
			buf[buf_offset++] = '-';
			buf[buf_offset++] = '^';
			break;
		}
	}
}

void fck_des(char *argv)
{
	int n;
	char argv_buf[30];

	/* padd the '7' to the tail of the buffer when user input less than 30 bytes */
	memset(argv_buf, '7', sizeof(argv_buf));
	memset(buf, 0x00, sizeof(buf));

	strncpy(argv_buf, argv, strlen(argv));

	/* a tiny symmetric encryption algorithm */
	generator(argv_buf);
	n = strlen(buf);

	unsigned char md5_buf[n + 1];
	strcpy(md5_buf, buf);

	/* this is md5 stuff,really */
	MDString(md5_buf + 2);
}

/*
 * Test Code for Real Time Clock Driver
 *
 * Compile with:
 *      gcc -s -Wall -Wstrict-prototypes settime.c -o settime
 *
 * This binary is a part of RTC test suite.
 *
 * History:
 * Copyright (C) 1996, Paul Gortmaker. This version is based on Paul's
 * XX-XX-XXXX   Texas Instruments       Initial version of the testcode
 * 12-09-2008   Ricardo Perez Olivares  Adding basic comments, variable
 *                                      names according to coding
 *                                      standars.
 *
 * Copyright (C) 2004-2009 Texas Instruments, Inc
 *
 * This package is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <linux/rtc.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "time_set.h"

int main(int argc, char *argv[])
{

	int fd, retval;
	struct rtc_time rtc_tm;
	int choice;

	fprintf(stdout, "\n\t\t\tTWL4030 RTC Driver Test\n\n");
	fflush(stdout);

	/* Creating a file descriptor for RTC */
	fd = open(argv[1], O_RDONLY);
	if (fd == -1) {
		perror("Requested device cannot be opened!");
		_exit(errno);
	}

	/* Displaying Menu */
	show_choice();

	/* Getting choice */
	scanf("%d", &choice);
	if (choice) {
		rtc_tm.tm_sec = 45;	/* second */
		rtc_tm.tm_min = 59;	/* minute */
		rtc_tm.tm_hour = 23;	/* hour */
		rtc_tm.tm_mday = 31;	/* day of the month */
		rtc_tm.tm_mon = 11;	/* month Jan=0, Feb=1 ... */
		rtc_tm.tm_year = 104;	/* = year - epoch */
	} else if (choice == 0) {
		printf("\n");
		get_time_date(&rtc_tm);
	}

	retval = ioctl(fd, RTC_SET_TIME, &rtc_tm);
	if (retval == -1) {
		printf("\nFailed setting RTC Date/Time!");
		perror("ioctl");
		show_menu();
		_exit(errno);
	} else {
		printf("\nSuccessful setting RTC Date/Time!");
	}

	/* Reading Current RTC Date/Time */
	retval = ioctl(fd, RTC_RD_TIME, &rtc_tm);
	if (retval == -1) {
		perror("ioctl");
		_exit(errno);
	}

	fprintf(stdout,
		"\n\nNew RTC Date/Time: %d-%d-%d %02d:%02d:%02d\n\n",
		rtc_tm.tm_mday, rtc_tm.tm_mon + 1, rtc_tm.tm_year + 1900,
		rtc_tm.tm_hour, rtc_tm.tm_min, rtc_tm.tm_sec);
	fflush(stdout);
	close(fd);
	return 0;

}

int get_time_date(struct rtc_time *rtc_tm)
{
	char date_str[7] = { 0 };
	char time_str[7] = { 0 };
	int num;
	unsigned char data[6] = { 0 };
	int i;

	show_menu();
	printf("Enter Date:");
	scanf("%s", date_str);
	printf("Enter Time:");
	scanf("%s", time_str);
	num = atoi(date_str);
	for (i = 0; i < 3; i++) {
		data[i] = num % 10;
		num /= 10;
		data[i] += (num % 10) * 10;
		num /= 10;
		/*printf ("data[%d] : %d\n", i, data[i]);*/
	}
	num = atoi(time_str);
	for (; i < 6; i++) {
		data[i] = num % 10;
		num /= 10;
		data[i] += (num % 10) * 10;
		num /= 10;
		/* printf ("data[%d] : %d\n", i, data[i]);*/
	}
	rtc_tm->tm_mday = data[1];
	rtc_tm->tm_mon = data[2] - 1;
	rtc_tm->tm_year = data[0] + 100;
	rtc_tm->tm_hour = data[5];
	rtc_tm->tm_min = data[4];
	rtc_tm->tm_sec = data[3];

	return 0;
}

void show_menu(void)
{
	int i = 0;
	printf("\n");
	for (i = 0; i++ < 40;)
		printf("*");
	printf("\n");
	printf("*\tSet Date Demo\t");
	printf("\n");
	for (i = 0; i++ < 40;)
		printf("*");
	printf("\n");
	printf("*\tUsage : Date format MMDDYY\n");
	printf("*\tUsage : Time format HHMMSS\n");
	for (i = 0; i++ < 40;)
		printf("*");
	printf("\n");
}

void show_choice(void)
{
	printf("\nSet date and time options:\n");
	printf("Enter 0 -> to accept date and time from user\n");
	printf("Enter 1 -> to use default date and time\n");
	printf("Choice:");
}

/*
 * lab1.c
 *
 *  Created on: January 21, 2016
 *      Author: Bradley Kohler
 */

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

void alarm_handler(int signo){
	if (signo == SIGALRM){
	printf("Alarm\n");
	alarm(2);
	}

	else if (signo == SIGINT){
	printf("CTRL+C pressed!\n");
	}

	else if (signo == SIGTSTP){
	printf("CTRL+Z pressed!\n");
	exit(1);
	}
}

int main(void){
	if (signal(SIGALRM , alarm_handler) == SIG_ERR){
	printf("FAIL\n");
	exit(1);
	}

	else if (signal(SIGINT, alarm_handler) == SIG_ERR){
	printf("FAIL\n");
	exit(1);
	}

	else if (signal(SIGTSTP, alarm_handler) == SIG_ERR){
	printf("FAIL\n");
	exit(1);
	}

	alarm(2);
	while(1){ sleep(10); }
}

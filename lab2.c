/*
 * lab2.c
 *
 *  Created on: Febuary 4, 2016
 *      Author: Bradley Kohler
 */

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
int main(void){
	int fd1[2], fd2[2], nbytes;
	int accum = 0;
	int temp = 0;
	int readbuffer;
	pid_t childpid;


	pipe(fd1);
	pipe(fd2);

	if((childpid = fork()) == -1){
		perror("fork");
		exit(0);
	}

	if(childpid == 0){
		close(fd1[0]);
		close(fd2[1]);
		while(1){
		printf("Enter a number: ");
		scanf("%d", &temp);
		write(fd1[1], &temp, 8);
		if (temp == -1){
		nbytes = read(fd2[0], &readbuffer, sizeof(readbuffer));
		printf("The sum is: %d", readbuffer+1);
		break;
		}
		}
	}
	else{
		close(fd1[1]);
		close(fd2[0]);
		while(1){
		nbytes = read(fd1[0], &readbuffer, sizeof(readbuffer));
		accum = accum + readbuffer;
		if (readbuffer == -1){
		write(fd2[1], &accum, 8);
		break;
		}
		}

	}
	return 0;
}

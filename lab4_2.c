/*
 * lab4_2.c
 *
 *  Created on: March 24, 2016
 *      Author: Bradley Kohler
 */

//includes
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/sem.h>


//global functions
int main(void);
void ChildProcess(void);
void ParentProcess(void);


//global variables
char * p;
off_t filesize;


//main function
/*
*/
int main(void){
int fd = open ("res.txt", O_RDWR); //assign file descriptor
struct stat finfo; // create stat structure file info

//assign file info file stats of the file descriptor
if (fstat(fd, &finfo) == -1){
perror("fstat");
return 1;
}

filesize = finfo.st_size; //assign filesize
p = mmap(NULL, filesize , PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); //create memory map

//if memory map failed perror mmap
if(p == MAP_FAILED) {
perror("mmap");
return 1;
}

long page_size = sysconf(_SC_PAGESIZE); //find page size

pid_t pid; //create process id
pid = fork(); //fork

//if child process id
if(pid == 0)
	ChildProcess();

//else parent process id
else
	ParentProcess();

//keep main in busy wait
while(1){
	sleep(100);
}

//return 1
return 1;
}

//child process
/*
*/
void ChildProcess(void)
{

//local variables
int i;

//infinite loop
while(1){

	sleep(10); //execute every 10 seconds

	printf("Page Size:\n"); //print page size
	printf("%d\n\n",getpagesize());

	printf("Current state of resources:\n"); //print the current state or resources
	for (i=0;i<filesize;i++){
		printf("%c",p[i]);
	}
	printf("\n");

	char char_array[((filesize+getpagesize())-1)/getpagesize()]; //find mincore value
	mincore(p, filesize, char_array);
	printf("Mincore value: %d\n\n",char_array[sizeof(char_array)-1]); //print mincore value

}

//return
return;

}


//parent process
/*
*/
void ParentProcess(void)
{

//local variables
int i,flag_type,type, quan;

printf("Supply resources at any time.\n"); //print message to user

//infinite loop
while(1){
while(scanf("%d %d",&type,&quan)<0){ //scan for resources
sleep(3); //sleep for 3 seconds
}

//if user inputs resources print resources entered
printf("You entered: %d %d\n",type, quan);

//
for(i=0;i<filesize;i++){
	if(i%4 == 0 && type == ((int) p[i])-48){ //if the number in the file is a type and it is the type the user entered
		flag_type = 1; //flag for found type
		printf("Quantity Availible of that Type: %c\n", p[i+2]); //print quantity
		p[i+2] += quan; //add quantity
		printf("New Quantity: %c\n",p[i+2]); //print new quantity
		//sync the file that is mapped in memory
		if (msync(p, filesize, MS_SYNC) == -1){
		perror("msync");
		}
		//break the loop
		break;
	}
}

//if no quantity is found
if(!flag_type){
printf("No Quantity of that type.\n"); //print no quantity found to screen
}

//sleep for 3 seconds
sleep(3);
}
}

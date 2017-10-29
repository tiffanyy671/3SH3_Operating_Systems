//includes
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

//main function
/*
*/
int main(){

char * p;
int fd = open ("res.txt", O_RDWR);
struct stat finfo;

if (fstat(fd, &finfo) == -1){
	perror("fstat");
	return 1;
}

off_t filesize = finfo.st_size;
p = mmap(NULL, filesize , PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); //map to memory

if(p == MAP_FAILED) {}
	perror("mmap");
	return 1;
}

long page_size = sysconf(_SC_PAGESIZE);
printf("Process Mapped \nPage_Size: %d\nBase_Address: %d\n",page_size,p);

int type, quan;
int i;
int flag_type, flag_quan;

while(1){
//ask how many resources are needed and of type
flag_type = 0;
flag_quan = 0;
printf("Type and Quantity: ");
scanf("%d %d", &type, &quan);
printf("You entered type: %d\nWith quantity: %d\n",type,quan);
for(i=0;i<filesize;i++){
	//printf("char[%d]: %c\n",i,p[i]);
	if(i%4 == 0 && type == ((int) p[i])-48){
		flag_type = 1;
		printf("Quantity Availible of that Type: %c\n", p[i+2]);
		if(quan <= ((int) p[i+2])-48){
			flag_quan = 1;
			p[i+2] -= quan;
			printf("New Quantity: %c\n",p[i+2]);
			if (msync(p, filesize, MS_SYNC) == -1){
			perror("msync");
			}
		}
		break;
	}
}

if(!flag_type){
printf("No Quantity of that type.\n");
}

else if(!flag_quan){
printf("That Quantity does not exist.\n");
}

}

return 1;
}

/*
 * lab3.c
 *
 *  Created on: March 3, 2016
 *      Author: Bradley Kohler
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <pthread.h>

typedef struct{
	int array[4];
	size_t size;
} array;

typedef struct{
	array darray[4];
	size_t dsize;
} darray;

typedef struct{
	darray *matrix;
	sem_t *semaphore;
	int phase;
} thread;

sem_t sem1;

void initArray(array* a);
void initDArray(darray* da);
void rowSort(darray* da);
void columnSort(darray* da);
void *shearSort(void *threadargs);
void printDArray(darray* da);

int main(){

if(sem_init(&sem1 , 1 , 1) < 0){
perror("semaphore initialization");
exit(0);
}

int i;
darray mydarray;
initDArray(&mydarray);
mydarray.darray[0].array[1] = 5;
mydarray.darray[0].array[2] = 3;
mydarray.darray[1].array[0] = 20;
mydarray.darray[3].array[1] = 11;
printDArray(&mydarray);
printf("----------\n");
thread threadobjects[5];
pthread_t threads[5];
int rc[5];
void *status[5];
thread *data_pointer;
for(i=0;i<mydarray.dsize+1;i++){
	printf("pthread_create %d\n",i);
	threadobjects[i].phase = i+1;
	threadobjects[i].semaphore = &sem1;
	threadobjects[i].matrix = &mydarray;
	data_pointer = &threadobjects[i];
	rc[i] = pthread_create(&threads[i],NULL,shearSort,(void *) data_pointer);
	rc[i] = pthread_join(threads[i],&status[i]);
}
printDArray(&mydarray);
pthread_exit(NULL);
}

void initArray(array* a){
a->size = 4;
int i;
for(i=0;i<a->size;i++){
	a->array[i]=0;
}
}

void initDArray(darray* da){
da->dsize = 4;
int i;
for(i=0;i<da->dsize;i++){
	initArray(&da->darray[i]);
}
}

void rowSort(darray* da){
int i, j, k, temp;
for(i=0;i<da->dsize;i++){
	if(i%2==0){
		for(j=0;j<da->darray[i].size-1;j++){
			for(k=0;k<da->darray[i].size-1;k++){
				if(da->darray[i].array[k]>da->darray[i].array[k+1]){
				temp = da->darray[i].array[k];
				da->darray[i].array[k] = da->darray[i].array[k+1];
				da->darray[i].array[k+1] = temp;
				}
			}
		}
	}
	else{
		for(j=da->darray[i].size-1;j>0;j--){
			for(k=da->darray[i].size-1;k>0;k--){
				if(da->darray[i].array[k]>da->darray[i].array[k-1]){
				temp = da->darray[i].array[k];
				da->darray[i].array[k] = da->darray[i].array[k-1];
				da->darray[i].array[k-1] = temp;
				}
			}
		}

	}
}
}

void columnSort(darray* da){
int i, j, k, temp;
for(i=0;i<da->dsize;i++){
	for(j=0;j<da->dsize-1;j++){
		for(k=0;k<da->dsize-1;k++){
			if(da->darray[k].array[i]>da->darray[k+1].array[i]){
			temp = da->darray[k].array[i];
			da->darray[k].array[i] = da->darray[k+1].array[i];
			da->darray[k+1].array[i] = temp;
			}
		}
	}
}
}

void printDArray(darray* da){
int i, j;
for(i=0;i<da->dsize;i++){
	for(j=0;j<da->dsize;j++){
		printf(" %d ",da->darray[i].array[j]);
		if(j == da->dsize-1){
		printf("\n");
		}
		else{
		printf(",");
		}
	}
}
}

void *shearSort(void * threadargs){
thread *threadi;
printf("Thread created\n");
threadi = threadargs;
printf("Data from void pointer stored in new thread\n");
sem_wait(threadi->semaphore);
printf("Released from queue\n");
if(threadi->phase%2==0){
	columnSort(threadi->matrix);
}
else{
	rowSort(threadi->matrix);
}
printf("Sorting phase complete\n");
sem_post(threadi->semaphore);
pthread_exit(NULL);
}

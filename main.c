#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>
#include <unistd.h>

#define ASIZE 2
#define BSIZE 1
#define PEOPLE 5

sem_t HallA;
sem_t HallB;

char timestamp[26]; 
//some pretty time format from stack overflow
void Timestamp(){
	time_t timer;
	struct tm* tm_info;
	timer = time(NULL);
	tm_info = localtime(&timer);
	strftime(timestamp, 26, "%H:%M:%S",tm_info);
	return;
}

void* Aguy(void* arg){

	sem_wait(&HallA);

	Timestamp();
	printf("[%s] enter A, times: %d %d\n",timestamp, *(int*)arg, *(int *)(arg+sizeof(int)));
	sleep(*(int*)arg);
	Timestamp();
	printf("[%s] exit A\n",timestamp);

	sem_post(&HallA);
	
	free(arg);
	pthread_exit(0);
}

void* Bguy(void* arg){
	
	sem_wait(&HallA);
	
	Timestamp();
	printf("[%s] enter A, times: %d %d\n", timestamp, *(int*)arg, *(int*)(arg+sizeof(int)));
	sleep(*(int*)arg);

	sem_wait(&HallB);
	sem_post(&HallA);
	
	Timestamp();
	printf("[%s] enter B\n", timestamp);
	sleep(*(int*)(arg+sizeof(int)));
	
	sem_wait(&HallA);
	sem_post(&HallB);
	
	sleep(1);
	Timestamp();
	printf("[%s] exit museum after B\n", timestamp);
	
	sem_post(&HallA);
	
	free(arg);
	pthread_exit(0);
}

int main(int argc, char* argv[])
{
	srand(time(NULL));

	sem_init(&HallA,0,ASIZE);
	sem_init(&HallB,0,BSIZE);
	
	pthread_t people[PEOPLE];

	for(int i=0;i<PEOPLE;i++){
		int* x = malloc(sizeof(int)*2);
		x[0] = rand()%3+1; 
		if(i%2==0){
			x[1] = 0;
			pthread_create(&people[i], NULL, Aguy, x);
		}else{
			x[1] = rand()%2+1;
			pthread_create(&people[i], NULL, Bguy, x);
		}
	}

	for(int i=0;i<PEOPLE;i++){
		pthread_join(people[i], NULL);
	}

	sem_destroy(&HallA);
	sem_destroy(&HallB);

	return 0;
}

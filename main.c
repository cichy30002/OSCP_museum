#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>

#define ATIMEBOUNDUP 5
#define ATIMEBOUNDDOWN 1 
#define BTIMEBOUNDUP 3
#define BTIMEBOUNDDOWN 1

sem_t HallA;
sem_t HallB;
int randint(int a, int b){
	return rand()%(a-b+1)+b;
}
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
	if(argc != 4){
		printf("%d",argc);
		return 1;
	}
	char *p;
	errno=0;
	long conv = strtol(argv[1], &p, 10);
	if (errno != 0 || *p != '\0' || conv > INT_MAX || conv < INT_MIN) return 2;
	int population = conv;
	conv = strtol(argv[2], &p, 10);
	if (errno != 0 || *p != '\0' || conv > INT_MAX || conv < INT_MIN) return 2;
	int sizeA = conv;
	conv = strtol(argv[3], &p, 10);
	if (errno != 0 || *p != '\0' || conv > INT_MAX || conv < INT_MIN) return 2;
	int sizeB = conv;

	srand(time(NULL));
	
	sem_init(&HallA,0,sizeA);
	sem_init(&HallB,0,sizeB);
	
	pthread_t people[population];

	for(int i=0;i<population;i++){
		int* x = malloc(sizeof(int)*2);
		x[0] = randint(ATIMEBOUNDUP,ATIMEBOUNDDOWN); 
		if(rand()%2==0){
			x[1] = 0;
			pthread_create(&people[i], NULL, Aguy, x);
		}else{
			x[1] = randint(BTIMEBOUNDUP,ATIMEBOUNDDOWN);
			pthread_create(&people[i], NULL, Bguy, x);
		}
	}

	for(int i=0;i<population;i++){
		pthread_join(people[i], NULL);
	}

	sem_destroy(&HallA);
	sem_destroy(&HallB);

	return 0;
}

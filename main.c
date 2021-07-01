#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>
#include <unistd.h>
void* Aguy(void* arg){
	printf("im A!");
	pthread_exit(0);
}
void* Bguy(void* arg){
	printf("im B!");
	pthread_exit(0);
}
int main(int argc, char* argv[])
{
	srand(time(NULL));
	pthread_t people[3];
	pthread_create(&people[0], NULL, Aguy, NULL);	
	pthread_join(people[0],NULL);


	return 0;
}

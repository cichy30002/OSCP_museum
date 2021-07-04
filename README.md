# OSCP_museum
Project for operating systems and concurrent programming
## Exercise:
Image a small museum with just 2 halls: A and B. While visiting the museum you
enter the hall A, and you can reach hall B by passing through hall A. The situation is
depicted in the following figure:

![image](https://user-images.githubusercontent.com/43621858/124396655-1b758f00-dd0b-11eb-9dca-ee3141ec3abb.png)

People visiting the museum enter hall A, watch the exhibition, and decide whether
they want to continue by proceeding to hall B. Some people leave the museum immediately, others enter hall B, watch the second exhibition, and than enter once again
hall A, and finally leave. For safety reasons there must be no more than NA people in
hall A at the same time, and no more than NB people in hall B (NB < NA). Write a
program(s) implementing synchronization algorithm that will allow:
1. visiting the museum at the same time by as many people as possible,
2. leaving the museum by people visiting hall B in the shortest possible time.

## Solution
Firstly we need to represent our museum in the program. So every guest will be a thread. I made 2 types of threads to make things simpler: we have a guest that wants to only watch Hall A and a guest who wants to visit both (Aguy and Bguy). The halls are represented as semaphores - we need to be sure that there are only Na people in Hall A and Nb in Hall B. 

Then implementing simple solution is very easy - when there is space in the hall the guest wants to come they enter (sem_wait) and then they leave the hall (sem_post). Watching the exhibition is implemented as sleep().
```C
void* Bguy(void* arg){
	sem_wait(&HallA);

	sleep(*(int*)arg);

	sem_wait(&HallB);
	sem_post(&HallA);

	sleep(*(int*)(arg+sizeof(int)));

	sem_wait(&HallA);
	sem_post(&HallB);

	sleep(1);

	sem_post(&HallA);

	free(arg);
	pthread_exit(0);
}
```
This solution is not a perfect one - there is a chance to get stuck when guests enter B and then new ones who also want to see B enter A so those from b can't leave.
To solve this problem we can go 2 ways - already described in exercise. 

Lets start with second because it's easier: allowing leaving the museum by people visiting hall B in the shortest possible time.
Nothing complicated - we need just to dont give back space in Hall A when guest enters Hall B so there always be option to leave as fast as possible
```C
void* Bguy(void* arg){
	sem_wait(&HallA);

	sleep(*(int*)arg);

	sem_wait(&HallB);
	//sem_post(&HallA);

	sleep(*(int*)(arg+sizeof(int)));

	//sem_wait(&HallA);
	sem_post(&HallB);

	sleep(1);
	
	sem_post(&HallA);
	
	free(arg);
	pthread_exit(0);
}
```
Now when we focus on first assumption there are really tough programming puzzle. My solution is using additional semaphore to inform all other guests wheter there are any people in Hall B waiting for free space in A. Other guests use this knowlege in 2 ways. Firstly if there exsit any queue in Hall B no other guest is going to enter the museum (from outside). Secondly there is an option to swap a guest from A that want to enter B wiith guest from B that want to enter A - they pass each other in the doorway. At this moment solution got quite complex so I wont paste there code as example.

## Runnig the program
I prepared a Makefile so at beggining we need to run
```bash
make
```
Then we will get 2 programs: museum and museum1. "museum" is the version that follow second assumption, "museum1" is one following first assumption.
we need to pass to them 3 arguments:
- number of guests that will visit the museum (X)
- capacity of Hall A (Na)
- capacity of Hall B (Nb)
```bash
./museum X Na Nb
./museum1 X Na Nb
```
Example
```bash
./museum 10 3 2
./muesum1 15 2 1
```

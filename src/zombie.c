#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

/*number of zombies in the game:
you should synchronize threads editing this variable*/
int zombieCounter = 0;
int zombie_killed = 0;
int zombie_entered = 0;
#define MAX_ZOMBIE_NUMBER 100
int yuzdeElliSans = 0;

/* Keeps track of number of zombies entered.*/
void zombieEntered(){
	zombie_entered ++;
    zombieCounter = zombieCounter + zombie_entered;
}
/* Keeps track of number of zombies killed.*/
void zombieKilled(){
	if(zombieCounter < zombie_entered){
		zombie_killed = zombieCounter - zombie_entered;
	}
	zombieCounter = zombieCounter - zombie_killed;
}
/* Returns true if number of zombies in the room are
greater than or equal to 100.*/
int tooManyZombiesInTheRoom(){ //return 1 dogru ise, return 0 yanlissa
	if(zombieCounter >= 100){
		printf("number of zombies in the room are greater than or equal to 100\n");
		return 1;
	}
	else{
		printf("number of zombie in the room are less than 100\n");
		return 0;
	}
}

/*Returns true if more than 100 zombies have been killed.*/
int killed100Zombies(){
	if(zombie_killed > 100){
		printf("more than 100 zombies have been killed\n");
		return 1;
	}
	else{
		return 0;
	}
}

/* Returns true if there is at least one zombies in the room.*/
int zombiesExist(){
	if(zombieCounter < 1){
		return 1;
	}
	else{
		return 0;
	}
}
/*Returns the number of zombies killed.*/
int getKilledCount(){
	void zombieKilled();
	return zombie_killed;
}

/* Returns the number of zombies in the room.*/
int getInTheRoomCount(){
	return zombieCounter;
}
/*doorman thread*/
void *doorMan(void *p){
	pthread_t door_man = (pthread_t)p;
	pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
	if(p != 0){
		usleep(2);
		for(int i = 0; i < 2; i++){//her kapici icin iki defa bu donguye girer, bir defa zombie'ye girmek icin izin verilir, bir defa da izin verilmez ,
			if(yuzdeElliSans % 2 == 0){
						pthread_mutex_lock(&m);
						zombieEntered();
						pthread_mutex_unlock(&m);
			}
			else{
						yuzdeElliSans ++;
			}
		}

	}
	else{
		printf("there is no door man\n");
	}
	if(getInTheRoomCount() > MAX_ZOMBIE_NUMBER || getKilledCount() > MAX_ZOMBIE_NUMBER){
		pthread_exit(NULL);
	}
	return NULL;
}

/*slayer thread*/
void *slayer(void *p){
	pthread_t tid = (pthread_t)p;
	if(tid != 0){
		pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;
		usleep(2);
		if(zombiesExist() ==  1){
			pthread_mutex_lock(&m1);
			int k = getKilledCount();
			k++;
			pthread_mutex_unlock(&m1);
		}
		if(getInTheRoomCount() > MAX_ZOMBIE_NUMBER || getKilledCount() > MAX_ZOMBIE_NUMBER){
				pthread_exit(NULL);
		}
	}
	else{
		printf("there is no slayer\n");
		pthread_exit(NULL);
	}
	return NULL;
}
/*simulator main thread*/
int main(int argc, char *argv[]){
	pthread_t thread_id2;
	pthread_t thread_id1;
	int rc1, n, rc2;
	rc1 = pthread_create(&thread_id1, NULL, slayer, (void*)1); //for slayer
	if(rc1){
		printf("\n ERROR: return code from pthread_create is %d \n", rc1);
	    exit(1);
    }
	else{
		printf("Create a new thread for slayer\n");
	}

	if(argc < 2){
		 printf("Please add the number of threads to the command line\n");

	}
	n = atoi(argv[2]);
	printf("integer degeri basariyla atandi\n");

	printf("your entered: %d", n);
	for(int i = 0; i < n; i++){
		rc2 = pthread_create(&thread_id2, NULL, doorMan, (void*)&n);
		if(rc2){
		      printf("\n ERROR: return code from pthread_create is %d \n", rc2);
		      exit(1);
	   }
	   else{
		      printf("\nCreate %d thread for doorMan\n", i + 1);
	 }
	}

	pthread_join(thread_id1, NULL);

	for(int i = 0; i < n; i++){
		pthread_join(thread_id2, NULL);
	}

	if(tooManyZombiesInTheRoom() == 1){
		printf("you have been killed by the zombies!!\n");
	}
	if(killed100Zombies() == 1){
		printf("you have killed 100 zombies!!\n");
	}

    pthread_exit(NULL);
}

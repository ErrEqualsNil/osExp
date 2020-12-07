#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <cstdio>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
using namespace std;

const int NUM_CHAIRS = 6;
const int CUSTOMER_NUM = 20;

sem_t mutex;
sem_t waiting_customers;
sem_t barber_ready;
int empty_chairs = NUM_CHAIRS;
int customer_sitting_id[NUM_CHAIRS];

void print_arr(){
	printf("Chairs Occupied Status: ");
	for(int i=0;i<NUM_CHAIRS;i++){
		if(customer_sitting_id[i]==0)
			printf("0 ");
		else
			printf("1 ");	
	}
	printf("\n");
}

void *Barber(void *args){
	while(1){
		sem_wait(&waiting_customers);
		sleep(5);
		sem_post(&barber_ready);    
	}
}

void *Customer(void* args){
	//sleep(rand()%10);
	sem_wait(&mutex);
	cout<<"Customer Comming!"<<endl;
	print_arr();
	if(empty_chairs > 0){
		int chair_id=0;
		for(chair_id=0;chair_id<NUM_CHAIRS;chair_id++) {
			if(customer_sitting_id[chair_id] == 0) {
				customer_sitting_id[chair_id] = 1;
				break;
			}	
				
		}
		empty_chairs--;
		printf("Customer sit down at %d chair\n", chair_id);
		print_arr();
		sem_post(&waiting_customers);
		sem_post(&mutex);

		sem_wait(&barber_ready);
		sem_wait(&mutex);
		customer_sitting_id[chair_id] = 0;
		empty_chairs++;
		printf("Customer at %d chair go to get haircut\n", chair_id);
		print_arr();
		sem_post(&mutex);
	}
	else{
		cout<<"Customer Leaving For No Empty Chair."<<endl;
		sem_post(&mutex);
	}
}

int main(int argc, char* argv[]){
	memset(customer_sitting_id, 0, sizeof(customer_sitting_id));
	pthread_t barber;
	pthread_t customer[CUSTOMER_NUM];
	sem_init(&mutex, 0, 1);
	sem_init(&waiting_customers, 0, 0);
	sem_init(&barber_ready, 0, 1);
	pthread_create(&barber, NULL, Barber, NULL);
	for(int i=0;i<CUSTOMER_NUM;i++){
		sleep(rand()%2+1);
		pthread_create(&customer[i], NULL, Customer, NULL);
	}
	pthread_exit(NULL);
	return 0;
}

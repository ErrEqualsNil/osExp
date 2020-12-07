#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <cstdio>
#include <unistd.h>
#include <stdlib.h>
using namespace std;

int reading_readers = 0;
int writing_writers = 0;
sem_t reading, writing;
sem_t mutex;

void* Reader(void* args){
    sem_wait(&reading);
    sem_wait(&mutex);
    reading_readers++;
    if(reading_readers == 1){
        sem_wait(&writing);
    }
    sem_post(&reading);
    sem_post(&mutex);
    cout<<"Reader Reading..."<<endl;
    sleep(1);
    sem_wait(&mutex);
    reading_readers--;
    if(reading_readers == 0){
        sem_post(&writing);
    }
    cout<<"Finish Reading"<<" "<<reading_readers<<endl;
    sem_post(&mutex);
}

void* Writer(void* args){
    sem_wait(&writing);
    sem_wait(&mutex);
    writing_writers++;
    if(writing_writers == 1){
        sem_wait(&reading);
    }
    sem_post(&mutex);
    cout<<"Writer Writing..."<<endl;
    sleep(3);
    sem_wait(&mutex);
    writing_writers--;
    if(writing_writers == 0){
        sem_post(&reading);
    }
    cout<<"Finish Writing"<<" "<<writing_writers<<endl;
    sem_post(&mutex);
    sem_post(&writing);
}

int main(int argc, char* argv[]){
    pthread_t readers[10];
    pthread_t writers[10];
    sem_init(&mutex, 0, 1);
    sem_init(&reading, 0, 1);
    sem_init(&writing, 0, 1);
    int read_cot, write_cot;
    read_cot = write_cot = 0;
    while(read_cot < 10 || write_cot < 10){
	sleep(1);
    	int choice = rand()%2;
	if (choice == 0){
		if(read_cot < 10)
			pthread_create(&readers[read_cot++], NULL, Reader, NULL);
		else
			pthread_create(&writers[write_cot++], NULL, Writer, NULL);
	}
	else if(choice == 1){
		if(write_cot < 10)
			pthread_create(&writers[write_cot++], NULL, Writer, NULL);
		else
			pthread_create(&readers[read_cot++], NULL, Reader, NULL);
	}
    }
    pthread_exit(NULL);
    return 0;
}

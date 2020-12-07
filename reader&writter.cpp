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
    int time_interval = rand()%10 * 100;
    usleep(time_interval);
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
    int time_interval = rand()%10 * 100;
    usleep(time_interval);
    sem_wait(&writing);
    sem_wait(&mutex);
    writing_writers++;
    if(writing_writers == 1){
        sem_wait(&reading);
    }
    sem_post(&mutex);
    cout<<"Writer Writing..."<<endl;
    sleep(1);
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
    for(int i=0;i<10;i++){
        pthread_create(&readers[i], NULL, Reader, NULL);
	pthread_create(&writers[i], NULL, Writer, NULL);
    }
    pthread_exit(NULL);
    return 0;
}

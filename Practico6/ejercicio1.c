#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#define N 5 /* number of slots in the buffer */
#define TRUE 1
//typedef int semaphore; /* semaphores are a special kind of int */
sem_t mutex;// = 1; /* controls access to critical region */
sem_t empty;// = N; /* counts empty buffer slots */
sem_t full;// = 0; /* counts full buffer slots */
int buffer = 0;

void *producer(void)
{
	//int item;
	while (TRUE) { /* TRUE is the constant 1 */
		//item = produce item( ); /* generate something to put in buffer */
        printf("Productor: produciendo...\n");
		sem_wait(&empty); /* decrement empty count */
		sem_wait(&mutex); /* enter critical region */
		//inser t item(item); /* put new item in buffer */
        printf("Productor: insertando...\n");
        buffer++;
        printf("buffer: %d\n\n",buffer);
        sleep(1);
		sem_post(&mutex); /* leave critical region */
		sem_post(&full); /* increment count of full slots */
	}
    pthread_exit(0);
}

void *consumer(void)
{
	//int item;
	while (TRUE) { /* infinite loop */
		sem_wait(&full); /* decrement full count */
		sem_wait(&mutex); /* enter critical region */
		//item = remove item( ); /* take item from buffer */
        printf("Consumidor: retirando...\n");
        buffer--;
        printf("buffer: %d\n\n",buffer);
        sleep(1);
		sem_post(&mutex); /* leave critical region */
		sem_post(&empty); /* increment count of empty slots */
        printf("Consumidor: consumiendo...\n");
		//consume item(item); /* do something with the item */
	}
    pthread_exit(0);
}


int main()
{
    //inicializar semaforos
    sem_init(&mutex,0,1);
    sem_init(&empty,0,N);
    sem_init(&full,0,0);

    //hilo productor
    int rc;
    pthread_t productor;
    rc = pthread_create(&productor,NULL,producer,NULL);

    if ( rc ) {
        printf("ERROR; return code from producer is %d\n", rc);
        exit(-1);
    }

    //hilo consumidor
    pthread_t consumidor;
    rc = pthread_create(&consumidor,NULL,consumer,NULL);

    if ( rc ) {
        printf("ERROR; return code from consumer is %d\n", rc);
        exit(-1);
    }

    /* End main thread */
   pthread_exit(NULL);
}
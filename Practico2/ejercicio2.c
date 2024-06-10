/*
 *Use 5 hilos para computar los 5 primeros términos de la serie de Fibonacci

0 1 1 2 3 5 8 13 ...
a_n = a_n-1 + a_n-2

Use una expresión no recursiva para la función de la serie.
Declare un arreglo común donde cada hilo debe almacenar el resultado.
Imprima el arreglo al finalizar
 * */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS     5

int fibonacci[NUM_THREADS]={0};

void *runner(void *param)
{
	int *tid = param;
	
	if( *tid == 0 || *tid == 1)
	{
		fibonacci[*tid] = *tid;
	}
	else
	{
		while(fibonacci[*tid-1] == 0 && fibonacci[*tid-2] == 0);
		fibonacci[*tid] = fibonacci[*tid-1] + fibonacci[*tid-2];
	}

	sleep(5);

   pthread_exit(0);
}


int main (int argc, char *argv[])
{
	pthread_t threads[NUM_THREADS];
	pthread_attr_t attr;
	int rc;
	int t;

	for ( t = 0; t < NUM_THREADS; t++ ) {
		printf("In main: creating thread %d\n", t);
		pthread_attr_init( &attr );

		rc = pthread_create( &threads[t], &attr, runner, (void *) &t );

      		if ( rc ) {
		printf("ERROR; return code from pthread_create() is %d\n", rc);
         	exit(-1);
	       	}
		
   	}
	//imprimir fibonacci
	printf("\nPrimeros numeros de la serie fibonacci\n");
	for ( t = 0; t < NUM_THREADS; t++ ){
		printf("Num %d: %d\n", t+1,fibonacci[t]);
	}

	pthread_exit(NULL);

}

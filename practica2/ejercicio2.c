#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

void *print_message_function( void *ptr );

int main(int argc, char *argv[])
{
pthread_t *thread;
char **matrizMensajes;
int *iret;
int numeroHilos, i;
char numero[5];

if (argc != 2)
{
printf ("Introduce un número, para la creación de hilo.");
exit (1);
}
	
sscanf(argv[1], "%d", &numeroHilos);

//Asignaciñon de memoria 
thread = (pthread_t *) malloc(sizeof(pthread_t) *numeroHilos);
iret = (int *) malloc(sizeof(int) *numeroHilos);
matrizMensajes = (char **)malloc (sizeof (char *) *numeroHilos);
for (i=0; i<numeroHilos; i++)
{
matrizMensajes[i] = (char *) malloc (sizeof(char) * 11);


strcpy(matrizMensajes[i], "Thread");
sprintf(numero, "%d", i+1);
strcat(matrizMensajes[i], numero);
}
//Se crean  los hilos
for (i=0; i<numeroHilos; i++)
{
iret[i] = pthread_create( &thread[i], NULL, print_message_function,(void*) matrizMensajes[i]);
}

//Se recorren los hilos del array, para que espere el hilo hasta que el otro hilo termine
for (i=0; i<numeroHilos; i++)
{
pthread_join( thread[i], NULL);
printf("Thread %d returns: %d\n", i+1, iret[i]);
}

//Se libera la memoria asignada por malloc
free (thread);
free (iret);
for (i=0; i<numeroHilos; i++)
free (matrizMensajes[i]);
free (matrizMensajes);
exit(0);
}

void *print_message_function( void *ptr )
{
char *matrizMensajes;
matrizMensajes = (char *) ptr;
printf("%s \n", matrizMensajes);
pthread_exit(0);
}

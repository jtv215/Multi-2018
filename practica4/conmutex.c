#include <stdio.h>  
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
//Función para calcular el tiempo
double get_wall_time(){
    struct timeval time;
    if (gettimeofday(&time,NULL)){
        //  Handle error
        return 0;
    }
    return (double)time.tv_sec + (double)time.tv_usec * .000001;
}
//Atributos generales
pthread_mutex_t mutex;
int *arrayA;
int *arrayB;
int sumaInterseccion=0;
int sumaUnion=0;
//Creación de estructuras datos
typedef struct{
	int comienza;
	int termina;
	int tamanioB;
} datos;
//Función para calcular la distacia Jaccard
void *jaccard(void *estructura){
	datos *p = (datos*)estructura;
	int interseccion = 0;
	int inicio = p->comienza;
	int finArrayA = p->termina;
	int finArrayB = p->tamanioB;
	
	for(int i = inicio; i < finArrayA; i++){
		for(int j = 0; j < finArrayB; j++){
			if(arrayA[i]==arrayB[j]){
				interseccion++;
				break;
			}
		}
	}
	//Uso de la función mutex lock y unlock
	pthread_mutex_lock(&mutex);
	sumaInterseccion += interseccion;
	sumaUnion += (finArrayA-inicio-interseccion);
	pthread_mutex_unlock(&mutex);
	pthread_exit(NULL);
}

int main(int argc, char *argv[]){  
	 double start=0.0;
	 double end=0.0;
     start = get_wall_time();
     sumaInterseccion=0;
 	 sumaUnion=0;
    // Las condiciones por parámetros al ejecutar el programa		
	if (argc != 4){
		printf ("Debes introducir un número\n");
		exit (1);
	}
	
	int tamanioA;
	int tamanioB;
	int numeroHilos;
	
	
	sscanf (argv[1], "%d", &tamanioA);
	sscanf (argv[2], "%d", &tamanioB);
	sscanf (argv[3], "%d", &numeroHilos);	
	
	
	if(tamanioA < 0 || tamanioB < 0 || numeroHilos < 1 ){
		printf("El número tiene que ser positivo\n");	
		exit (1);
	}
	if(tamanioA < numeroHilos || tamanioB < numeroHilos){
		printf("El número hilos tiene que ser menor que el tamaño del array\n");	
		exit (1);
	}	
	//reserva de memoria
	arrayA = (int *) malloc(tamanioA*sizeof(int));
	arrayB = (int *) malloc(tamanioB*sizeof(int));
	//rellenar los dos vectores	
	for(int i = 0; i < tamanioA; i++){
		arrayA[i] = i;		
	}
	for(int i = 0; i < tamanioB; i++){
		arrayB[i] = 2*i;
	}
	//iniacilizar los atributos de la función hilos	 y reservar memoria
	pthread_t *threads; 
	threads = (pthread_t *) malloc(numeroHilos*sizeof(pthread_t)); 
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	datos *datosArray;
	datosArray = (datos *)malloc(numeroHilos*sizeof(datos));
	pthread_mutex_init(&mutex, 0);
	
	int rc;  
	int inicio = 0;
	int fin = 0;
	int cociente = tamanioA / numeroHilos;
	int resto = tamanioA % numeroHilos;
	int t=0;
	//Se distribuye el array por el numero de hilos
	for( t = 0; t < numeroHilos; t++){  		
		inicio = t * cociente + (t < resto? t : resto);
		fin = inicio + cociente + (t < resto);
		
		datosArray[t].comienza = inicio;
		datosArray[t].termina = fin;
		datosArray[t].tamanioB = tamanioB;
		
		rc = pthread_create(&threads[t], &attr, jaccard, (void *)&datosArray[t]);		
	}
	
	int interseccionHilos = 0;
	int unionHilos = 0;
	
	
	pthread_attr_destroy(&attr);	
	//Se le asigna a cada hilo la función join:Espera el hilo hasta que el otro hilo termine.
	for(t = 0; t < numeroHilos; t++) {
    	rc = pthread_join(threads[t], NULL);
    	
	}
	
	sumaUnion += tamanioB;
	//printf("Elementos de la Interseción: %d\n",sumaInterseccion);
	//printf("Elementos de la Union: %d\n",sumaUnion);	
	printf("La distancia de Jaccard es: %.2f\n", (double)sumaInterseccion/sumaUnion);
		
	pthread_mutex_destroy(&mutex);
	//libera la memoria asignada por malloc		
	free(datosArray);
	free(arrayA);
	free(arrayB);
	
	end = get_wall_time();  
    printf("Tiempo: %.6fs\n", end-start);	    	
	pthread_exit(NULL);
}

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
int *arrayA;
int *arrayB;
//Creación de estructuras datos
typedef struct{
	int comienza;
	int termina;
	int solucionParcial;
	int tamanioA;
	int tamanioB;
}datos;
//Función para calcular la distacia Jaccard
void *jaccard (void *estructura){
	datos *p = (datos*)estructura;
	int interseccion = 0;
	int inicio = p->comienza;
	int finArrayA = p->termina;
	int finArrayB = p->tamanioB;
	
	for(int i=inicio; i < finArrayA; i++){
		for(int j = 0; j < finArrayB; j++){
			if(arrayA[i]==arrayB[j]){
				interseccion++;
				break;
			}
		}
	}	
	p->solucionParcial = interseccion;	
	pthread_exit(NULL);
}

int main (int argc, char *argv[]){  
	 double start=0.0;
	 double end=0.0;
     start = get_wall_time();
	// Las condiciones por parámetros al ejecutar el programa			
	if (argc != 4){
		printf ("\nDebes introducir un número");
		exit (1);
	}
	
	int tamanioA,tamanioB,numeroHilos;	
	
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
	int i;	
	for(i = 0; i < tamanioA; i++){
		arrayA[i] = i;
	}	
	for(i = 0; i < tamanioB; i++){
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
	
	int rc;  
	int inicio = 0;
	int fin = 0;
	int cociente = tamanioA / numeroHilos;
	int resto = tamanioA % numeroHilos;
	int t = 0;
	
	//Se distribuye el array por el numero de hilos
	for(t = 0; t < numeroHilos; t++){  		
		inicio = t * cociente + (t < resto? t : resto);
		fin = inicio + cociente + (t < resto);
		
		datosArray[t].comienza = inicio;
		datosArray[t].termina = fin;
		datosArray[t].solucionParcial = 0;
		datosArray[t].tamanioA = tamanioA;
		datosArray[t].tamanioB = tamanioB;		
		rc = pthread_create(&threads[t], &attr, jaccard, (void *)&datosArray[t]);  	
	}	
		
	pthread_attr_destroy(&attr);	
	int sumaInteseccion = 0;
	
	//Se le asigna a cada hilo la función join:Espera el hilo hasta que el otro hilo termine.
	for(t = 0; t < numeroHilos; t++) {
    	rc = pthread_join(threads[t], NULL);
		sumaInteseccion += datosArray[t].solucionParcial;
    	
	}
	
	int formulaUnion= tamanioA+tamanioB-sumaInteseccion;
	//printf("Elementos de la Interseción: %d\n",sumaInteseccion);
	//printf("Elementos de la Union: %d\n",formulaUnion);
	printf("La distancia de Jaccard es: %.2f\n", (double)sumaInteseccion/formulaUnion);
	
	//libera la memoria asignada por malloc		
	free(datosArray);	
	free(arrayA);
	free(arrayB);	
	end = get_wall_time();  
    printf("Tiempo: %.6fs\n", end-start);	
	pthread_exit(NULL);
	
}

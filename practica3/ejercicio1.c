#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define vectorSize 1000000

//Se crea una esctructura.
typedef struct datos{
	double suma_parcial;
	int ini;
	int fin;
} aux;

//Atributos
double sumaGeneral=0;
pthread_mutex_t mutex;
double* vector=NULL;

void *funcion(void *threadid)
{
int inicio,final, subSumaf,i;
double suma=0;
aux *pdatos;

pdatos = ((aux *) (threadid));
subSumaf = pdatos->suma_parcial;
inicio = pdatos->ini;
final = pdatos->fin;
   		
for(i=inicio;i<final;i++){		
	if(i%2==0) suma += vector[i];
	else suma -= vector[i];
}

pdatos->suma_parcial=suma;
pthread_mutex_lock(&mutex);
sumaGeneral+=suma;
pthread_mutex_unlock(&mutex);
pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
int rc,NUM_THREADS,i,t;
pthread_attr_t attr;
double sumaTotal=0;

if(argc!=2){
	printf("Introduce el número de hilos\n");
	exit(1);
}
sscanf (argv[1],"%i",&NUM_THREADS);	//guarda el valor en NUM_THREADS

pthread_t threads[NUM_THREADS];	//se crea un array de hilos
int resto=vectorSize%NUM_THREADS;//se obtiene el resto

//reserva de memoria.
vector=(double*)malloc(sizeof(double)*vectorSize);
for(i=0;i<vectorSize;i++){
	vector[i]=i*1;
}

//se inicializa el atributos y cambiamos el valor del atributo.
pthread_attr_init(&attr);
pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
aux param[NUM_THREADS];
int cociente= (vectorSize/NUM_THREADS);

//recorremos el número de hilos y asignamos a cada hilo
//el número de elementos que tiene que recorrer
for(t=0;t<NUM_THREADS;t++) {	
param[t].suma_parcial = 0;//se inicializa a cero
param[t].ini= t*cociente+(t<resto?t:resto);//Seleciona la posicion de inicio a comenzar	
param[t].fin=param[t].ini+cociente+(t<resto?1:0);//Seleciona la posicion de fin 
//se crea el hilo y se llama a la función , y como parametro la 
//estructura datos.
rc = pthread_create(&threads[t], NULL, funcion, (void *) &param[t]);	
 }

//Se recorren los hilo,y espera el hilo hasta que el otro hilo termine.
for(i=0; i<NUM_THREADS; i++) {
    rc = pthread_join(threads[i],NULL);  
}
printf("La suma total: %.1f\n",sumaGeneral);	
	
//Se libera la memoria asignada por malloc
free(vector);

//se libera los pthread_mutex_destroy()
pthread_mutex_destroy(&mutex);
pthread_exit(NULL);

}

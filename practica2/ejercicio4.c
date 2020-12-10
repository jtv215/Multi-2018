#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
//Se crea una esctructura.
struct datos
{
    long id;
    long posini;
    long nelementos;
};
//Atributos
double  sumaGeneral=0.0;
double *array;
pthread_mutex_t exc_mut;

//La función sumar de tipo void
void *sumar(void *d)
{
long inicio,fin,id;
struct datos *dat;//un array de tipo datos
double sumaSubArray=0;

dat = (struct datos *) d;//lo que me pasan por parametro un casting
id = dat->id; 
inicio = dat->posini;
fin =  dat->posini + dat->nelementos; 

//Suma la parte del sub-Array que le corresponde.  
for (int i=inicio; i < fin ; i++){
   sumaSubArray = sumaSubArray + array[i];
}
//bloquea y desbloquea el hilo hasta la espera del siguiente hilo
pthread_mutex_lock(&exc_mut);
sumaGeneral = sumaGeneral + sumaSubArray;
pthread_mutex_unlock(&exc_mut);
printf("El valor que tiene el hilo %ld número es: %2.lf\n", id, sumaSubArray);

pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
long numerohilos,tamanioArray,i, nelementos;
pthread_t *threads;//array de hilos
struct datos *arraydatos;
int rc;
pthread_attr_t attr;

if (argc != 2)
{
printf ("Hay que pasar por parámetro un número divisible a 4.\n");
exit (1);
}
tamanioArray =atoi(argv[1]);
numerohilos = 4;

if (tamanioArray % numerohilos != 0)
{
 printf ("Hay que pasar por parámetro un número divisible a 4.\n");
 exit(1);
}


//Asignación de memoria.
array = (double*)malloc(tamanioArray * sizeof(double));
threads = (pthread_t *) malloc (sizeof(pthread_t) * numerohilos);
arraydatos = (struct datos *) malloc (sizeof(struct datos) * numerohilos);

//Rellenamos el array a 2.
for ( i=0; i<tamanioArray; i++){
     array[i] = 2;
}
//se inicializa el mutex y los atributos y cambiamos el valor del atributo.
pthread_mutex_init(&exc_mut, NULL);
pthread_attr_init(&attr);
pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

//recorremos el número de hilos y asignamos a cada hilo
//el número de elementos que tiene que recoorer
nelementos = tamanioArray / numerohilos;
for ( i=0; i<numerohilos; i++)
{
  arraydatos[i].id = i;
  arraydatos[i].nelementos = nelementos; 
  arraydatos[i].posini = i * nelementos; 
  //printf ("El hilo creado es el número : %ld\n", i);
  
//se crea el hilo y se llama a la función sumar, y como parametro la 
//estructura datos.
rc = pthread_create (&threads[i], &attr, sumar, (void*)&arraydatos[i]);

}
//Se recorren los hilo,y para el hilo hasta que el otro hilo termine
for ( i=0; i<numerohilos; i++)
{
	pthread_join (threads[i], NULL);
}

printf ("La suma total es: %.2lf\n", sumaGeneral);
//Se libera la memoria asignada por malloc
free(array);
free(threads);
free(arraydatos);
//se libera los pthread_mutex_destroy() y el atributo.
pthread_attr_destroy(&attr);
pthread_mutex_destroy(&exc_mut);
pthread_exit(NULL);
    
}

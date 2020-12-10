#include <stdio.h>
#include <stdlib.h>

double sumar(double *vector, int longitud)
{
  double suma = 0;
  int i;
  for (i=0; i<longitud; i++){
  suma += vector[i];
   }
   return suma;
}

int main(int argc, char *argv[]){
if (argc != 2){ 
 printf ("Hay que pasar por parámetro un número.\n"); 
 exit (1); 
}

//Iniciamos el array y reservamos la memoria. 
int longitud;
sscanf(argv[1], "%d", &longitud);
double *array = (double*)malloc(longitud * sizeof(double));

//Rellanamos el array a 5.
for(int i=0; i<longitud; i++){ 
   array[i]=5;
}
//LLamamos a la funcioón sumar.
double resultado = sumar(array,longitud);
printf ("La suma total de un vector es: %.2f\n",resultado);    
free(array);
return 0;
}

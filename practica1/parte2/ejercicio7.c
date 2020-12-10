#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
//La condición
if (argc != 2){
	printf ("Poner un número mayor que cero");
	exit (1);
}
//Se pide tamaño del vector	
int tVector;
sscanf (argv[1], "%d", &tVector);	
	

//Se reserva la memoria	
int *p_vector = (int *)malloc(tVector*sizeof(int));
int *p_matriz = (int *)malloc(tVector*tVector*sizeof(int));
int *p_resultado = (int *)malloc(tVector*sizeof(int));



//pongo el valor del vector en i++
for(int i = 0; i < tVector; i++)
	p_vector[i] = i;

//Pongo el valor de la matriz bidimensional a 5
for (int i=0; i<tVector; i++){
	p_vector[i]=5;	
}
	
//Calculamos producto
for (int i=0; i<tVector; i++){
	p_resultado[i] = 0;
	for (int j=0; j<tVector; j++){
		p_resultado[i] += p_vector[j] * p_matriz[j*tVector + i];
	}
}

free(p_vector);
free(p_matriz);
free(p_resultado);
return 0;

}

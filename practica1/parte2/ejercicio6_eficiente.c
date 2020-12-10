#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

//La condición
if (argc != 2){
	printf ("Poner un número mayor que cero");
	exit (1);
}
//Se pide tamaño del vector
int tVector;
sscanf(argv[1], "%d", &tVector);	
    
//Se reserva la memoria
int *p_vector = (int*)malloc(tVector * sizeof(int));
int **p_matriz = (int**)malloc(tVector * sizeof(int*));
for(int i=0; i<tVector; i++){
	p_matriz[i] = (int*)malloc(tVector * sizeof(int));
}
int *p_resultado = (int*)malloc(tVector * sizeof(int));

//Pongo el valor del vector en i++
for(int i=0; i<tVector; i++){
	p_vector[i]=i;
}
//Pongo el valor de la matriz a 5     
for(int i=0; i<tVector; i++){
	for(int j=0; j<tVector; j++){
	    p_matriz[i][j]=5;
	}
}
//Calculamos el vector resultado
for(int i= 0; i < tVector; i++)
	p_resultado[i] = 0;

for (int i=0; i < tVector; i++) {
	for (int j = 0; j < tVector; j++) {
			p_resultado[i] += p_vector[j] * p_matriz[i][j];
	}
}
//Liberamos memoria
free(p_vector);
for(int i=0; i<tVector; i++){
	free(p_matriz[i]);
}
free(p_matriz);
free(p_resultado);
return 0;
}

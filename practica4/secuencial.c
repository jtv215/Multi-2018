#include <stdio.h>  
#include <stdlib.h>
#include <sys/time.h>

//Función tiempo
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
//Función para calcular la distacia Jaccard
double jaccard (int tamanioA, int tamanioB){
	int inter = 0;
	int i = 0;
	int j = 0;
	for(i = 0; i < tamanioA; i++){
		for(j = 0; j < tamanioB; j++){
			if(arrayA[i]==arrayB[j]){
				inter++;
				break;
			}
		}
	}
	int formulaUnion = tamanioA + tamanioB - inter;
	//printf("Elementos de la Interseción: %d\n",inter);
	//printf("Elementos de la Union: %d\n",formulaUnion);
		
	return (double)inter/formulaUnion;
}


int main (int argc, char *argv[]){  	
	 double start=0.0;
	 double end=0.0;
     start = get_wall_time();
	// Las condiciones por parámetros al ejecutar el programa
	if (argc != 3){
		printf ("\nDebes introducir un número");
		exit (1);
	}
	
	int tamanioA,tamanioB;
	
	sscanf (argv[1], "%d", &tamanioA);
	sscanf (argv[2], "%d", &tamanioB);	
	
	if(tamanioA < 0 || tamanioB < 0 ){
		printf("El número tiene que ser positivo\n");	
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
		
	printf("La distancia de Jaccard es: %.2f\n", (double)jaccard(tamanioA, tamanioB));
	//liberar la memoria asignada por malloc	
	free(arrayA);
	free(arrayB);
		
	end = get_wall_time();  
    printf("Tiempo: %.6fs\n", end-start);	
}

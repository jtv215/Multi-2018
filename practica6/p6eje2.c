#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

MPI_Status status;
int suma=0;
int tamanioVector=0;
int* vector=NULL;


int main(int argc, char *argv[]){
	int rank,size;	
	int ini,fin;
	//recoger por parámetro el tamaño del vector
	sscanf (argv[1],"%i",&tamanioVector);
	
    MPI_Init( &argc, &argv );    
    MPI_Comm_size( MPI_COMM_WORLD, &size);
    MPI_Comm_rank( MPI_COMM_WORLD, &rank);
    
	int aux1=size-1;
	int resto=tamanioVector%aux1;	
	vector=(int*)malloc(tamanioVector*sizeof(int));
	
	if(rank==0){ 		
		for(int i=0;i<tamanioVector;i++){
			vector[i]=i*1;		
		}		
		for(int i=1;i<size;i++){			
        	MPI_Send(vector,tamanioVector,MPI_INT,i,10,MPI_COMM_WORLD);                        
        }	
		int sumaTotal=0;
		for(int i=1;i<size;i++){
			MPI_Recv(&suma, 1000, MPI_INT, i, 10, MPI_COMM_WORLD,&status);
			sumaTotal+=suma;
		}
		printf("Suma total: %d\n",sumaTotal);
		
	}
		
	if(rank!=0){
		int aux=rank-1;		
		MPI_Recv(vector, tamanioVector, MPI_INT, 0, 10, MPI_COMM_WORLD,&status);
		ini=aux*(tamanioVector/aux1)+(aux<resto?aux:resto);
		fin=ini+(tamanioVector/aux1)+(aux<resto);		
		printf("rank %d: empieza--> %d hasta --> %d\n",rank,ini,fin);		
		for(int i=ini;i<fin;i++){
			if(i%2==0) suma += vector[i];
			else	suma -= vector[i];
		}
		MPI_Send (&suma, 100, MPI_INT, 0, 10, MPI_COMM_WORLD);	
	}
		
	MPI_Finalize();	
	
  	
}



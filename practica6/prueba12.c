#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

//inicializa el vector a 0,1,2,3,4....
void inicializarVector(int numItems, double** vec){
	*vec = malloc(sizeof(double) * numItems);
	for(int i = 0; i < numItems; i++)
		(*vec)[i] = i * 1.0;
}
//Distribuye las cargas
void distribuirCargas(int numItems, int numProcs, int** iniciaVector, int** cuentas){
	*iniciaVector = malloc(sizeof(int) * numProcs);
	*cuentas = malloc(sizeof(int) * numProcs);
	int paquete = numItems / (numProcs-1);
	int excedente = numItems % (numProcs-1);
	for(int i = 1, j = 0; i < numProcs; i++, j++){
		(*iniciaVector)[i] = (j*paquete + (j<excedente?j:excedente));
		(*cuentas)[i] = (paquete + (j<excedente));
	}
	(*iniciaVector)[0] = 0; 
	(*cuentas)[0] = 0;
}

int main(int argc, char* argv[]){
	
	//Se inicializa y se conoce el ranking y el numero total de procesos.
	MPI_Init(&argc, &argv);
	int myID = 0, numProcs = 0;
	double sumaTotal;
	MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myID);
	
	if(argc!=2){
		if(myID==0) 
			printf("Falta introducir el tamaño del vector\n");
	}else{
		int numItems = atoi(argv[1]);
		double* vector = 0;		
		int* iniciaVector = 0;
		int* cuentas = 0;
		distribuirCargas(numItems, numProcs, &iniciaVector, &cuentas);
		if(myID == 0){//Solo entra el proceso 0, 
			inicializarVector(numItems, &vector);//inicializa el vector	
			//Distribuye las tareas al proceso  la parte que le corresponde.   		
			MPI_Scatterv(vector, cuentas, iniciaVector, MPI_DOUBLE, MPI_IN_PLACE, cuentas[myID], MPI_DOUBLE, 0, MPI_COMM_WORLD);			
		printf("nodo %d -->  iniciaVector en la posición : %d, cuentas %d\n",myID, iniciaVector[myID],cuentas[myID]);
		}else{
			vector = malloc(sizeof(double)*cuentas[myID]);//Yo, como proceso estandar, solo pido espacio para lo que necesito
			//Distribuye las tareas al proceso  la parte que le corresponde
			MPI_Scatterv(vector, cuentas, iniciaVector, MPI_DOUBLE, vector, cuentas[myID], MPI_DOUBLE, 0, MPI_COMM_WORLD);	
		printf("nodo %d -->  iniciaVector en la posición : %d, cuentas %d\n",myID, iniciaVector[myID],cuentas[myID]);
		}
		//Cada proceso deberia tener su parte:		
		double suma=0.0;        
		for (int i = 0; i < cuentas[myID] ; i++) {
			if((int)vector[i]%2==0) suma += vector[i];
			else suma -= vector[i];
		}	
		
		MPI_Reduce(&suma, &sumaTotal, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);		
		
		if (myID == 0)
			printf("\nLa suma Total es: %f \n", sumaTotal);
		
		free(vector);
		free(iniciaVector);
		free(cuentas);
	}

	MPI_Finalize();
}

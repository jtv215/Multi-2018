#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "mpi.h"

int size, rank, option, numero,producto;
int i,j;
char texto[100], aux;
MPI_Status status;

void calculaEntero();
void mayusculaMinisucla();
void InvertirTexto();


int main (int argc, char *argv[])
{
	MPI_Init (&argc, &argv);
	MPI_Comm_size (MPI_COMM_WORLD, &size); 
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);

	switch(rank)
	{
		case 0:
			do
			{
				do{
					printf ("\nElige una de las siguientes opciones: \n");
					printf ("0 - Salir.\n");
					printf ("1 - Calcula el entero correspondiente a cada letra\n");
					printf ("2 - Poner en mayuscula los caracteres impar y miniscula par\n");
					printf ("3 - Invertir orden de texto\n");
					printf ("4 - Ejecutar todos las opciones\n");
					printf ("Introduce la opción deseada entre 0 y 4: \n"); 				
					scanf (" %d", &option);
					fflush (stdin);
					
				}while(option >4 );

				switch(option)
				{

						case 0:
							
							printf ("El programa ha finalizado.\n");
							for (i=1; i<size; i++){
								MPI_Send (&option, sizeof(int), MPI_INT, i, 10, MPI_COMM_WORLD);
							}
							break;
							
						case 1:	
											
							MPI_Send (&option, sizeof(int), MPI_INT, 1, 10, MPI_COMM_WORLD);
							printf ("Escriba la palabra: ");
							fflush (stdout);
							scanf (" %s", texto);
							MPI_Send (texto, 100, MPI_CHAR, 1, 10, MPI_COMM_WORLD);
							MPI_Recv (&producto, sizeof(int), MPI_INT, 1, 10, MPI_COMM_WORLD, &status);
							printf ("El entero correspondiente es : %d\n", producto);							
							break;
							
						case 2:
							
							MPI_Send (&option, sizeof(int), MPI_INT, 2, 10, MPI_COMM_WORLD);
							printf("Escriba la palabra: ");
                 			fflush (stdout);
                 			scanf(" %[^\n]", texto);
                  			
                  			MPI_Send (texto, 100, MPI_CHAR, 2, 10, MPI_COMM_WORLD);
                  			MPI_Recv (texto, 100, MPI_CHAR, 2, 10, MPI_COMM_WORLD, &status);
                 			printf ("Mayúscula-minúscula es: %s \n",texto);                  			
                  			break;
                  			
                  		case 3:
                  		
							MPI_Send (&option, sizeof(int), MPI_INT, 3, 10, MPI_COMM_WORLD);
							printf("Escriba la palabra: ");
                 			fflush (stdout);                  		
                  			
                  			scanf(" %[^\n]", texto);
                  			MPI_Send (texto, 100, MPI_CHAR, 3, 10, MPI_COMM_WORLD);
                  			MPI_Recv (texto, 100, MPI_CHAR, 3, 10, MPI_COMM_WORLD, &status);
                  			
                  			printf ("Palabra inversa es: %s \n", texto);
                 			break;
	
						case 4:
							printf("Escriba la palabra: ");
                  		    fflush (stdout);
                  		    scanf(" %[^\n]", texto);
                  			
                  			for (i=1; i<size; i++){
                     				MPI_Send (&option, sizeof(int), MPI_INT, i, 10, MPI_COMM_WORLD);
                     				MPI_Send (texto, 100, MPI_CHAR, i, 10, MPI_COMM_WORLD);
                  			}
                  			
                  			MPI_Recv (&producto, sizeof(int), MPI_INT, 1, 10, MPI_COMM_WORLD, &status);
                  		    printf ("El entero correspondiente es: %d \n",producto);                  			
                  			MPI_Recv (texto, 100, MPI_CHAR, 2, 10, MPI_COMM_WORLD, &status);
                  			printf ("Mayúscula-minúscula es: %s \n",texto);                    			
                  			MPI_Recv (texto, 100, MPI_CHAR, 3, 10, MPI_COMM_WORLD, &status);
                  			printf ("Palabra inversa es: %s \n", texto);
          					break;	

				}
			}while (option != 0);
			break;
							
		case 1:
			calculaEntero();
			break;

		case 2:
			mayusculaMinisucla();
			break;

		case 3:
			InvertirTexto();
			break;
	}
		
MPI_Finalize();
}



	
void calculaEntero(){
	
	do{		
            MPI_Recv (&numero, sizeof(int), MPI_INT, 0, 10, MPI_COMM_WORLD, &status);
            if (numero != 0)
            {
               MPI_Recv (texto, 100, MPI_CHAR, 0, 10, MPI_COMM_WORLD, &status);
               producto=1;
               for (i=0; i<strlen(texto); i++)
               {
				producto= texto[i]* producto;
               }             
        
			}			
               
               MPI_Send (&producto, sizeof(int), MPI_INT, 0, 10, MPI_COMM_WORLD);
            
    }while (numero != 0);
}

void mayusculaMinisucla(){
	do{
            MPI_Recv (&numero, sizeof(int), MPI_INT, 0, 10, MPI_COMM_WORLD, &status);
            if (numero !=0 )
            {
               MPI_Recv (&texto, 100, MPI_CHAR, 0, 10, MPI_COMM_WORLD, &status);
               for (i=0; i<strlen(texto); i++)
               {				   
				  if(texto[i]==' '){
					  
					   i++;
				   }
				   
                  texto[i] = toupper(texto[i]);
                  i++;
                  texto[i]=tolower(texto[i]);

               }
               MPI_Send (texto, 100, MPI_CHAR, 0, 10, MPI_COMM_WORLD);
            }
    }while (numero != 0);
}

void InvertirTexto(){
	do{
            MPI_Recv (&numero, sizeof(int), MPI_INT, 0, 10, MPI_COMM_WORLD, &status);
            if (numero != 0)
            {
               MPI_Recv (texto, 100, MPI_CHAR, 0, 10, MPI_COMM_WORLD, &status);
               i = 0;
               j = strlen(texto) - 1;
               while (i < j)
               {
                  aux = texto[i];
                  texto[i] = texto[j];
                  texto[j] = aux;
                  i++;
                  j--;
               }
               MPI_Send (texto, 100, MPI_CHAR, 0, 10, MPI_COMM_WORLD);
            }
    }while (numero != 0);

}





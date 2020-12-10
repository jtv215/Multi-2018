#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <assert.h>
#include <time.h>

// Crea una array de números aleatorios. Cada número tiene un valor de 0 a 1
float *create_rand_nums(int num_elements) {
  float *rand_nums = (float *)malloc(sizeof(float) * num_elements);
  assert(rand_nums != NULL);//confirmar que no sea null
  int i;
  
  for (i = 0; i < num_elements; i++) {
    rand_nums[i] = (rand() / (float)RAND_MAX);
  }
  return rand_nums;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: avg num_elements_per_proc\n");
    exit(1);
  }

  int num_elements_per_proc = atoi(argv[1]);

	
  MPI_Init(NULL, NULL);
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

 
//Crea una matriz aleatoria de elementos en todos los procesos.
  srand(time(NULL)*world_rank);// generador de números aleatorios de procesos de manera única
  float *rand_nums = NULL;//array de tipo float
  rand_nums = create_rand_nums(num_elements_per_proc);//llama al método de arriba el número de elementos es el tamaó del array

  //Suma los números localmente
  float local_sum = 0;
  int i;
  for (i = 0; i < num_elements_per_proc; i++) {
    local_sum += rand_nums[i];
  }
 //ALLREDUCE: envia la suma total a todos los proceso .
 //Reduzca todas las sumas locales en la suma global y
//calculamos la media
  float global_sum;
  MPI_Allreduce(&local_sum, &global_sum, 1, MPI_FLOAT, MPI_SUM,
                MPI_COMM_WORLD);
  float mean = global_sum / (num_elements_per_proc * world_size);//realiza la media

 //Calcula la suma local de las diferencias cuadradas de la media
  float local_sq_diff = 0;
  for (i = 0; i < num_elements_per_proc; i++) {
    local_sq_diff += (rand_nums[i] - mean) * (rand_nums[i] - mean);
  }
//Con MPi redice recoge el resultado de todos los proces y se lo lleva al proceso maestro.
//Reducir la suma global de las diferencias al cuadrado al proceso raíz
//e imprimir la respuesta
  float global_sq_diff;
  MPI_Reduce(&local_sq_diff, &global_sq_diff, 1, MPI_FLOAT, MPI_SUM, 0,
             MPI_COMM_WORLD);

//La desviación estándar es la raíz cuadrada de la media del cuadrado
//diferencias.
  if (world_rank == 0) {
    float stddev = sqrt(global_sq_diff /
                        (num_elements_per_proc * world_size));
    printf("Mean - %f, Standard deviation = %f\n", mean, stddev);
  }

  //Clean up
  free(rand_nums);
 //Bloquea al proceso hasta que todos los procesos pertenecientes al comunicador especificado lo ejecuten.
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();
}

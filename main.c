#include <stdio.h>
#include <mpi.h>

#define ARRAY_SIZE 10

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int array[ARRAY_SIZE];
    int local_sum = 0;

    // Inicializa el arreglo con valores para sumar
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        array[i] = i + 1;
    }

    // Divide el arreglo entre los procesos
    int local_elements = ARRAY_SIZE / size;
    int start_index = rank * local_elements;
    int end_index = (rank + 1) * local_elements;

    // Cada proceso calcula la suma de sus elementos
    for (int i = start_index; i < end_index; ++i) {
        local_sum += array[i];
    }

    // Combina los resultados de cada proceso mediante MPI_Reduce
    int global_sum;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Imprime el resultado desde el proceso 0
    if (rank == 0) {
        printf("La suma total es: %d\n", global_sum);
    }

    MPI_Finalize();
    return 0;
}

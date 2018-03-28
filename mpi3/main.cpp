#include <iostream>
#include "mpi.h"
#include <random>

using namespace std;

#define dim 10

int main(int argc, char **argv) {
    int process_id;
    int world_size;

    MPI_Init(&argc, &argv);
    int arr[dim];

    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_id);

    if (world_size < 2) {
        printf("Требуется не менее 2 процессора\n");
        MPI_Finalize();
        exit(-1);
    }
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, dim);

    if (process_id == 0) {
        printf("Работает процесс %d... \n", process_id);
        for (int i = 0; i < dim; ++i) {
            arr[i] = dis(gen);
        }


        MPI_Send(&arr, dim, MPI_INT, 1, 0, MPI_COMM_WORLD);

    } else if (process_id > 0) {
        MPI_Status status{};
        MPI_Recv(&arr, dim, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
         printf("Работает процесс %d: \n", process_id);
        for (int i = 0; i < dim; ++i) {
            printf("arr[%d]=%d\n", i, arr[i]);
        }
    }
    // printf("\nЧисло процессов в коммуникаторе MPI_COMM_WORLD: %d\n", world_size);
    MPI_Finalize();
    return 0;
}

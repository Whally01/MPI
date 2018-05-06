#include <iostream>

#include "mpi.h"

using namespace std;

int main(int argc, char **argv) {
    int world_size;
    //Инициализируем mpi
    MPI_Init(&argc, &argv);

    // Число процессов
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Номер текущего процесса
    int process_id;
    MPI_Comm_rank(MPI_COMM_WORLD, &process_id);

    printf("Работает процесс: %d\n", process_id);
    printf("Число процессов в коммуникаторе MPI_COMM_WORLD: %d\n", world_size);

    //Завершаем работу с mpi
    MPI_Finalize();

    return 0;
}
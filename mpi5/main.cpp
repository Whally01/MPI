#include <iostream>
#include <mpi.h>

using namespace std;
#define dim 12
#define ROOT 0

void broadcast(void *data, int count, MPI_Datatype datatype, int root,
               MPI_Comm communicator) {
    int world_rank;
    MPI_Comm_rank(communicator, &world_rank);
    int world_size;
    MPI_Comm_size(communicator, &world_size);

    if (world_rank == root) {
        // Если процесс широковещатель
        int i;
        for (i = 0; i < world_size; i++) {
            if (i != world_rank) {
                MPI_Send(data, count, datatype, i, 0, communicator);
            }
        }
    } else {
        // Если процесс получатель
        MPI_Recv(data, count, datatype, root, 0, communicator,
                 MPI_STATUS_IGNORE);
    }
}

int main(int argc, char **argv) {
    int process_id;
    int world_size;

    MPI_Init(&argc, &argv);
    MPI_Status s{};
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_id);

    if (world_size < 2) {
        printf("Требуется не менее 2 процессора\n");
        MPI_Finalize();
        exit(-1);
    }


    if (process_id == ROOT) {
        int arr[dim] = {3, 4, 5, 2, 8, 9, 11, 0, 1, 7, 6, 10};
        printf("Работает процесс: %d\n", process_id);
        broadcast(&arr, 12 / process_id + 1, MPI_INT, ROOT, MPI_COMM_WORLD);

        /*MPI_Send(&arr, 12 / process_id + 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(&arr, 2 * (12 / process_id + 1) - (12 / process_id + 1), MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Send(&arr, 3 * (12 / process_id + 1) - 2 * (12 / process_id + 1), MPI_INT, 2, 0, MPI_COMM_WORLD);
        MPI_Send(&arr, 12 / process_id + 1, MPI_INT, 3, 0, MPI_COMM_WORLD);*/

    } else if (process_id > 2) {
        int arr[12 / (process_id + 1)];
        printf("Работает процесс: %d\n", process_id);

        broadcast(&arr, dim, MPI_INT, ROOT, MPI_COMM_WORLD);

        for (int i = 0; i < 12 / (process_id + 1); i++) {
            printf("i=%d\n", i);
        }
    }

    printf("Кол-во процессов: %d\n", world_size);
}
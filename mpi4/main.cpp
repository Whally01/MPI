#include "iostream"
#include "mpi.h"
#include "random"

using namespace std;

#define dim 5

int main(int argc, char **argv) {
    int process_id;
    int world_size;
    int count; // кол-во элементов в буфере

    MPI_Status status{};
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_id);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    if (world_size < 2) {
        printf("Требуется не менее 2 процессора\n");
        MPI_Finalize();
        exit(-1);
    }

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 5);

    if (process_id == 1) {
        int arr[dim];
        for (int i = 0; i < dim; i++) {
            arr[i] = dis(gen);
        }
        for (int i = 0; i < world_size; i++) {
            if (i != process_id) {
                MPI_Send(arr, dim, MPI_INT, i, 0, MPI_COMM_WORLD);
            }
        }
    } else if (process_id != 1) {
        MPI_Probe(1, 0, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &count);
        int *buffer = (int *) malloc(count * sizeof *buffer);
        MPI_Recv(buffer, count, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
        for (int i = 0; i < count; i++) {
            printf("arr[%d]=%d ", i, buffer[i]);
        };
        printf("\n");
        free(buffer);
    }

    MPI_Finalize();
    return 0;
}
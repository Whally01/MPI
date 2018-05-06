#include "stdio.h"
#include "mpi.h"
#include "algorithm"
#include "iostream"

#define MASTER_RANK 0

#define M_SIZE 3

using namespace std;

MPI_Status status;

float a[M_SIZE][M_SIZE];
float b[M_SIZE][M_SIZE];
float c[M_SIZE][M_SIZE];


template<int rows, int cols>
void fillMatrix(float (&matrix)[rows][cols]) {
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            matrix[i][j] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

        }
    }
}

template<int rows, int cols>
void printMatrix(float (&matrix)[rows][cols]) {
    printf("\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%f ", matrix[i][j]);
        }
        printf("\n");
    }
}

template<int rows, int cols>
void multiplay_by_elements(float (&matrix)[rows][cols], float (&matrix1)[rows][cols], float(&matrix2)[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix2[i][j] = matrix1[i][j] * matrix1[i][j];
        };
    };
}

template<int rows, int cols>
void multiplay(float (&matrix)[rows][cols], float (&matrix1)[rows][cols], float(&matrix2)[rows][cols]) {
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            for (int inner = 0; inner < rows; inner++) {
                matrix2[row][col] = matrix[row][inner] * matrix1[inner][col];
            };
        };
    };
}

template<int rows, int cols>
void transpose(float (&matrix)[rows][cols]) {
    float trans[rows][cols];
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            trans[j][i] = matrix[i][j];
        }
    }
    printMatrix(trans);
}

int main(int argc, char *argv[]) {
    int numprocs, process_rank, process_id;
    int offset, rows_num;
    int workers_num;
    int remainder;
    int whole_part;
    int message_tag;
    int i, j, k;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);

    if (process_rank == MASTER_RANK) {
        fillMatrix(a);
        printMatrix(a);

        fillMatrix(b);
        printMatrix(b);

        cout << "Транспонированная матирца а: ";
        transpose(a);

        workers_num = numprocs - 1;
        whole_part = M_SIZE / workers_num;
        remainder = M_SIZE % workers_num;
        offset = 0;

        message_tag = 1;
        for (process_id = 1; process_id <= workers_num; process_id++) {
            rows_num = process_id <= remainder ? whole_part + 1 : whole_part;
            MPI_Send(&offset, 1, MPI_INT, process_id, message_tag, MPI_COMM_WORLD);
            MPI_Send(&rows_num, 1, MPI_INT, process_id, message_tag, MPI_COMM_WORLD);
            MPI_Send(&a, M_SIZE * M_SIZE, MPI_INT, process_id, message_tag, MPI_COMM_WORLD);
            MPI_Send(&b, M_SIZE * M_SIZE, MPI_INT, process_id, message_tag, MPI_COMM_WORLD);

            offset += rows_num;
        }

        message_tag = 2;
        for (process_id = 1; process_id <= workers_num; process_id++) {
            MPI_Recv(&offset, 1, MPI_INT, process_id, message_tag, MPI_COMM_WORLD, &status);
            MPI_Recv(&rows_num, 1, MPI_INT, process_id, message_tag, MPI_COMM_WORLD, &status);
            MPI_Recv(&c[offset][0], rows_num * M_SIZE, MPI_INT, process_id, message_tag, MPI_COMM_WORLD, &status);
        }
        printf("Результат:\n");
        printMatrix(c);
    }

    if (process_rank != MASTER_RANK) {
        message_tag = 1;
        MPI_Recv(&offset, 1, MPI_INT, MASTER_RANK, message_tag, MPI_COMM_WORLD, &status);
        MPI_Recv(&rows_num, 1, MPI_INT, MASTER_RANK, message_tag, MPI_COMM_WORLD, &status);
        MPI_Recv(&a, M_SIZE * M_SIZE, MPI_INT, MASTER_RANK, message_tag, MPI_COMM_WORLD, &status);
        MPI_Recv(&b, M_SIZE * M_SIZE, MPI_INT, MASTER_RANK, message_tag, MPI_COMM_WORLD, &status);

        for (i = offset; i < offset + rows_num; i++) {
            for (j = 0; j < M_SIZE; j++) {
                c[i][j] = a[i][j] * b[i][j];
            }
        }

        message_tag = 2;
        MPI_Send(&offset, 1, MPI_INT, MASTER_RANK, message_tag, MPI_COMM_WORLD);
        MPI_Send(&rows_num, 1, MPI_INT, MASTER_RANK, message_tag, MPI_COMM_WORLD);
        MPI_Send(&c[offset][0], rows_num * M_SIZE, MPI_INT, MASTER_RANK, message_tag, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
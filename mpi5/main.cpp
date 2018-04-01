        #include <iostream>
        #include <mpi.h>
        #include <zconf.h>

        #define dim 12

        /*p*(dim/(p+1) - (p+1)*(dim/p+2)*/
        int main(int argc, char **argv) {
            int process_id;
            int world_size;
            MPI_Init(&argc, &argv);
            MPI_Comm_rank(MPI_COMM_WORLD, &process_id);
            MPI_Comm_size(MPI_COMM_WORLD, &world_size);
            MPI_Status s;

            if (process_id == 0) {
                int arr[dim] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
                for (int i = 0; i < world_size; i++) {
                    int someArr[dim / (i + 1)];
                    int k = 0;
                    for (int j = i * (dim / ((world_size) + 1)); j < (i + 1) * (dim / ((world_size) + 1)); j++) {
                        someArr[k] = arr[j];
                        k++;
                    }
                    MPI_Send(someArr, dim / (i + 1), MPI_INT, i, 0, MPI_COMM_WORLD);
                }
                int count;
                MPI_Probe(0, 0, MPI_COMM_WORLD, &s);
                MPI_Get_count(&s, MPI_INT, &count);
                int *buf = (int *) malloc(sizeof(int) * count);
                MPI_Recv(buf, count, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                for (int i = 0; i < count; i++) {
                    printf("%d ", buf[i]);
                };
            } else {
                int count;
                MPI_Probe(0, 0, MPI_COMM_WORLD, &s);
                MPI_Get_count(&s, MPI_INT, &count);
                int *buffer = (int *) malloc(count * sizeof *buffer);
                MPI_Recv(buffer, count, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                for (int i = 0; i < count; ++i) {
                    printf("%d ", buffer[i]);
                }
                printf("\n");
                printf("Процесс %d\n___________________\n", process_id);
                free(buffer);
                sleep(10);
            }

            MPI_Finalize();
            return 0;
        }
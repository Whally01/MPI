#include "iostream"
#include "mpi.h"
#include "vector"
#include "cmath"
#include "cstdlib"
#include <random>


using namespace std;

void print(int *x, int k) {
    for (int i = 0; i < k; i++) {
        printf("%d ", x[i]);
    }
    printf("\n");
}

int *abs(int *vector, int size) {
    int *res = new int[10];
    for (int i = 0; i < size; i++) {
        res[i] = abs(vector[i]);
    }
    return res;
}

int main(int argc, char **argv) {

    const int K = 10;
    int vec[K];
    int rank = argc, size, resultVar = 0;
    int *vbuf, *result;

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 10);

    for (int &j : vec) {
        j = dis(gen);
    }

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    vbuf = new int[10 / size];
    result = new int[10 / size];

    if (10 % size != 0) {
        MPI_Finalize();
        return 0;
    }

    MPI_Scatter(vec, 10 / size, MPI_INT, vbuf, 10 / size, MPI_INT, 0, MPI_COMM_WORLD);
    vbuf = abs(vbuf, 10 / size);
    MPI_Reduce(vbuf, result, 10 / size, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        for (int i = 0; i < 10 / size; i++) {
            resultVar += result[i];
        }
        printf("%d - норма вектора ", resultVar);
        print(vec, 10);
    }
    MPI_Finalize();
    return 0;

}
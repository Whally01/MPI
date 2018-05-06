#include "iostream"
#include "mpi.h"
#include "random"
#include "vector"

#define SIZE 10
#define A 4
#define B 4

void printVector(std::vector<int> x) {
    for (int i = 0; i < x.size(); i++) {
        printf("%d ", x[i]);
    };
    printf("]\n");
}

int add(std::vector<int> x, std::vector<int> y, int index) {
    return (A * x[index]) + (B * y[index]);
}

int multiplay(std::vector<int> x, std::vector<int> y, int index) {
    return x[index] * y[index];
}

std::vector<int> exchange(std::vector<int> x, std::vector<int> y, int index) {
    int buffer = x[index];
    x[index] = y[index];
    y[index] = buffer;
    return x;
}

std::vector<int> addVector(std::vector<int> x, std::vector<int> y) {
    std::vector<int> result(x.size());
    for (int i = 0; i < x.size(); i++) {
        result[i] = add(x, y, i);
    }
    return result;
}

std::vector<int> multiplayVector(std::vector<int> x, std::vector<int> y) {
    std::vector<int> result(x.size());
    for (int i = 0; i < x.size(); i++) {
        result[i] = multiplay(x, y, i);
    }
    return result;
}

int main(int argc, char **argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 10);
    int partSize = SIZE / (size - 1);
    if (rank == 0) {
        std::vector<int> x(SIZE), y(SIZE);

        for (int i = 0; i < SIZE; i++) {
            x[i] = dis(gen);
            y[i] = dis(gen);
        }
        printf("x: [");
        printVector(x);
        printf("y: [");
        printVector(y);
        printf("-----------------------------\n");
        for (int i = 1; i < size; i++) {
            std::vector<int> xPart(&x[partSize * (i - 1)], &x[partSize * i]),
                    yPart(&y[partSize * (i - 1)], &y[partSize * i]),
                    result(partSize),
                    auxResult(partSize);
            MPI_Send(&xPart[0], xPart.size(), MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&yPart[0], yPart.size(), MPI_INT, i, 1, MPI_COMM_WORLD);
            MPI_Recv(&result[0], partSize, MPI_INT, MPI_ANY_SOURCE, 10, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("a) [");
            printVector(result);
            MPI_Recv(&result[0], partSize, MPI_INT, MPI_ANY_SOURCE, 20, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("b) [");
            printVector(result);
            MPI_Recv(&result[0], partSize, MPI_INT, MPI_ANY_SOURCE, 30, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("c) [");
            printVector(result);
            MPI_Recv(&result[0], partSize, MPI_INT, MPI_ANY_SOURCE, 41, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&auxResult[0], partSize, MPI_INT, MPI_ANY_SOURCE, 42, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("d) x: [ ");
            printVector(result);
            printf("d) y: [");
            printVector(auxResult);
        }
    } else {
        std::vector<int> xPart(partSize), yPart(partSize), result(partSize);
        MPI_Recv(&xPart[0], partSize, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&yPart[0], partSize, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        result = addVector(xPart, yPart);
        MPI_Send(&result[0], result.size(), MPI_INT, 0, 10, MPI_COMM_WORLD);
        addVector(xPart, yPart);
        MPI_Send(&result[0], result.size(), MPI_INT, 0, 20, MPI_COMM_WORLD);
        result = multiplayVector(xPart, yPart);
        MPI_Send(&result[0], result.size(), MPI_INT, 0, 30, MPI_COMM_WORLD);
        result = exchange(xPart, yPart, 0);
        MPI_Send(&result[0], result.size(), MPI_INT, 0, 41, MPI_COMM_WORLD);
        result = exchange(yPart, xPart, 0);
        MPI_Send(&result[0], result.size(), MPI_INT, 0, 42, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}
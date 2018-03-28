#include <iostream>

#include "mpi.h"

using namespace std;

int main(int argc, char **argv) {
    //Инициализируем mpi
    MPI_Init(&argc, &argv);

    cout << "Hello, World!" << endl;

    MPI_Finalize();
    return 0;
}
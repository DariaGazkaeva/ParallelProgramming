#include <cstdio>
#include <iostream>
#include "mpi.h"

int main(int argc, char** argv) {
    int rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        int a[10];

        for (int i = 0; i < 10; i++) {
            a[i] = rand() % 100;
        }

        MPI_Send(a, 10, MPI_INT, 1, 0, MPI_COMM_WORLD);

    } else if (rank == 1) {
        int a[10];
        MPI_Status status;

        MPI_Recv(a, 10, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

        for (int i = 0; i < 10; i++) {
            printf("%d ", a[i]);
        }

        printf("\n");
    }

    MPI_Finalize();
    return 0;
}

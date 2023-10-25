#include <mpi.h>
#include <iostream>

int main(int argc, char **argv)
{
    int rank, num;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank != 0) {
        num = rank * 10;
        MPI_Send(&num, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    } else {
        int amount;
        MPI_Comm_size(MPI_COMM_WORLD, &amount);
        for (int i = 1; i < amount; i++) {
            MPI_Recv(&num, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
            printf("%d ", num);
        }
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}

#include <mpi.h>
#include <iostream>

int main(int argc, char **argv)
{
    srand(4);

    int rank;
    int count;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 1) {
        int a[10];
        for (int i = 0; i < 10; i++) {
            a[i] = rand() % 100;
        }
        int amount;
        MPI_Comm_size(MPI_COMM_WORLD, &amount);
        for (int i = 0; i < amount; i++) {
            if (i != 1) {
                MPI_Send(a, 10, MPI_INT, i, 0, MPI_COMM_WORLD);
            }
        }
    } else {
        MPI_Probe(1, 0, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &count);

        int a[count];
        MPI_Recv(a, count, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);

        for (int i = 0; i < count; i++) {
            printf("%d ", a[i]);
        }
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}

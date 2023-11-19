#include <mpi.h>
#include <random>
#include <math.h>

int main(int argc, char** argv)
{
    srand(21);

    int rank, count, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Status status;

    const int SIZE = 100;
    const int BLOCK = SIZE / (size - 1);
    const int ZERO_PROCESS = 0;
    const int A = 2;
    const int B = 3;

    if (rank == ZERO_PROCESS) {
        int x[SIZE];
        int y[SIZE];
        int z[SIZE];

        for (int i = 0; i < SIZE; i++) {
            x[i] = rand() % 100;
            y[i] = rand() % 100;
        }

        printf("Vector X: ");
        for (int i = 0; i < SIZE; i++) {
            printf("%d ", x[i]);
        }
        printf("\n\n");
        printf("Vector Y: ");
        for (int i = 0; i < SIZE; i++) {
            printf("%d ", y[i]);
        }
        printf("\n\n");

        int dest = 1;
        for (int i = 0; i < size; i += BLOCK, dest++) {
            MPI_Send(&x[i], BLOCK, MPI_INT, dest, 0, MPI_COMM_WORLD);
            MPI_Send(&y[i], BLOCK, MPI_INT, dest, 1, MPI_COMM_WORLD);
        }

        int source = 1;
        for (int i = 0; i < size; i += BLOCK, source++) {
            MPI_Recv(&z[i], BLOCK, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
        }

        printf("Vector Z: ");
        for (int i = 0; i < SIZE; i++) {
            printf("%d ", z[i]);
        }
    } else {
        MPI_Probe(ZERO_PROCESS, 0, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &count);
        int x[BLOCK];
        int y[BLOCK];
        int z[BLOCK];

        MPI_Recv(&x[0], count, MPI_INT, ZERO_PROCESS, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&y[0], count, MPI_INT, ZERO_PROCESS, 1, MPI_COMM_WORLD, &status);

        for (int i = 0; i < count; i++) {
            z[i] = A * x[i] + B * y[i];
        }
        MPI_Send(&z, count, MPI_INT, ZERO_PROCESS, 2, MPI_COMM_WORLD);
    }
    MPI_Finalize();
}

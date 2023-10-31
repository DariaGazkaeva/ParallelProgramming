#include <mpi.h>
#include <random>
#include <math.h>

int main(int argc, char** argv)
{
    srand(22);

    int rank, count;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Status status;

    const int ZERO_PROCESS = 0;

    if (rank == ZERO_PROCESS) {
        int A[10][10];
        int B[10];
        int C[10];

        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                A[i][j] = rand() % 10;
            }
        }
        for (int i = 0; i < 10; i++) {
            B[i] = rand() % 10;
        }

        printf("Matrix A: \n");
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                printf("%d ", A[i][j]);
            }
            printf("\n");
        }
        printf("\n");
        printf("Vector B: \n");
        for (int i = 0; i < 10; i++) {
            printf("%d ", B[i]);
        }
        printf("\n\n");

        int dest = 1;
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j += 10, dest++) {
                MPI_Send(&A[i][j], 10, MPI_INT, dest, 0, MPI_COMM_WORLD);
                MPI_Send(&B[0], 10, MPI_INT, dest, 0, MPI_COMM_WORLD);
            }
        }

        int source = 1;
        for (int i = 0; i < 10; i++, source++) {
            MPI_Recv(&C[i], 1, MPI_INT, source, 0, MPI_COMM_WORLD, &status);
        }

        printf("Vector C: \n");
        for (int i = 0; i < 10; i++) {
            printf("%d ", C[i]);
        }
    } else {
        MPI_Probe(ZERO_PROCESS, 0, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &count);
        int x[10];
        int y[10];
        int z = 0;

        MPI_Recv(&x[0], count, MPI_INT, ZERO_PROCESS, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&y[0], count, MPI_INT, ZERO_PROCESS, 0, MPI_COMM_WORLD, &status);

        for (int i = 0; i < count; i++) {
            z += x[i] * y[i];
        }
        MPI_Send(&z, 1, MPI_INT, ZERO_PROCESS, 0, MPI_COMM_WORLD);
    }
    MPI_Finalize();
}

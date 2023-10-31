#include <mpi.h>
#include <random>
#include <math.h>

int main(int argc, char** argv)
{
    srand(23);

    int rank, count;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Status status;

    const int ZERO_PROCESS = 0;
    const int DIM = 10;
    const int BLOCK = 10;

    if (rank == ZERO_PROCESS) {
        int A[DIM][DIM];
        int B[DIM][DIM];
        int C[DIM][DIM];

        for (int i = 0; i < DIM; i++) {
            for (int j = 0; j < DIM; j++) {
                A[i][j] = rand() % 10;
                B[i][j] = rand() % 10;
            }
        }

        printf("Matrix A: \n");
        for (int i = 0; i < DIM; i++) {
            for (int j = 0; j < DIM; j++) {
                printf("%d ", A[i][j]);
            }
            printf("\n");
        }
        printf("\n");
        printf("Matrix B: \n");
        for (int i = 0; i < DIM; i++) {
            for (int j = 0; j < DIM; j++) {
                printf("%d ", B[i][j]);
            }
            printf("\n");
        }
        printf("\n");

        int dest = 1;
        for (int i = 0; i < DIM; i++) {
            for (int j = 0; j < DIM; j += BLOCK, dest++) {
                MPI_Send(&A[i][j], BLOCK, MPI_INT, dest, 0, MPI_COMM_WORLD);
                MPI_Send(&B[i][j], BLOCK, MPI_INT, dest, 0, MPI_COMM_WORLD);
            }
        }

        int source = 1;
        for (int i = 0; i < DIM; i++) {
            for (int j = 0; j < DIM; j += BLOCK, source++) {
                MPI_Recv(&C[i][j], BLOCK, MPI_INT, source, 0, MPI_COMM_WORLD, &status);
            }
        }

        printf("Vector C: \n");
        for (int i = 0; i < DIM; i++) {
            for (int j = 0; j < DIM; j++) {
                printf("%d ", C[i][j]);
            }
            printf("\n");
        }
    } else {
        MPI_Probe(ZERO_PROCESS, 0, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &count);
        int x[BLOCK];
        int y[BLOCK];
        int z[BLOCK];

        MPI_Recv(&x[0], count, MPI_INT, ZERO_PROCESS, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&y[0], count, MPI_INT, ZERO_PROCESS, 0, MPI_COMM_WORLD, &status);

        for (int i = 0; i < count; i++) {
            z[i] = x[i] * y[i];
        }
        MPI_Send(&z, count, MPI_INT, ZERO_PROCESS, 0, MPI_COMM_WORLD);
    }
    MPI_Finalize();
}
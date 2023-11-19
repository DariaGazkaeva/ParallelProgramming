#include <mpi.h>
#include <random>
#include <math.h>

int main(int argc, char** argv)
{
    srand(22);

    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Status status;

    const int ROWS = 10;
    const int COLUMNS = 10;
    const int BLOCK_C = ROWS / (size - 1);
    const int BLOCK_A = BLOCK_C * COLUMNS;
    const int ZERO_PROCESS = 0;

    if (rank == ZERO_PROCESS) {
        int A[ROWS][COLUMNS];
        int B[COLUMNS];
        int C[ROWS];

        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLUMNS; j++) {
                A[i][j] = rand() % 10;
            }
        }
        for (int i = 0; i < COLUMNS; i++) {
            B[i] = rand() % 10;
        }

        printf("Matrix A: \n");
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLUMNS; j++) {
                printf("%d ", A[i][j]);
            }
            printf("\n");
        }
        printf("\n");
        printf("Vector B: \n");
        for (int i = 0; i < COLUMNS; i++) {
            printf("%d ", B[i]);
        }
        printf("\n\n");

        for (int dest = 1; dest < size; dest++) {
            int row = (dest - 1) * BLOCK_C;
            MPI_Send(&A[row][0], BLOCK_A, MPI_INT, dest, 0, MPI_COMM_WORLD);
            MPI_Send(&B[0], COLUMNS, MPI_INT, dest, 1, MPI_COMM_WORLD);
        }

        for (int source = 1; source < size; source++) {
            int index = (source - 1) * BLOCK_C;
            MPI_Recv(&C[index], BLOCK_C, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
        }

        printf("Vector C: \n");
        for (int i = 0; i < COLUMNS; i++) {
            printf("%d ", C[i]);
        }
    } else {
        int x[BLOCK_C][COLUMNS]; // part of A
        int y[COLUMNS]; // full B
        int z[BLOCK_C]; // part of C

        MPI_Recv(&x[0], BLOCK_A, MPI_INT, ZERO_PROCESS, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&y[0], COLUMNS, MPI_INT, ZERO_PROCESS, 1, MPI_COMM_WORLD, &status);

        for (int i = 0; i < BLOCK_C; i++) {
            z[i] = 0;
            for (int j = 0; j < COLUMNS; j++) {
                z[i] += x[i][j] * y[j];
            }
        }
        MPI_Send(&z, BLOCK_C, MPI_INT, ZERO_PROCESS, 2, MPI_COMM_WORLD);
    }
    MPI_Finalize();
}

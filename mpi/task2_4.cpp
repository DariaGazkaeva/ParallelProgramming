#include <mpi.h>
#include <random>
#include <math.h>

int main(int argc, char** argv)
{
    srand(24);

    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Status status;

    const int ZERO_PROCESS = 0;
    const int ROWS_A = 10; // = ROWS_C
    const int COLUMNS_A = 10; // = ROWS_B
    const int COLUMNS_B = 10; // = COLUMNS_C

    const int BLOCK_ROW = ROWS_A / (size - 1); // часть строк С или А
    const int BLOCK_A = BLOCK_ROW * COLUMNS_A;
    const int BLOCK_C = BLOCK_ROW * COLUMNS_B;
    const int SIZE_B = COLUMNS_A * COLUMNS_B;

    if (rank == ZERO_PROCESS) {
        int A[ROWS_A][COLUMNS_A];
        int B[COLUMNS_A][COLUMNS_B];
        int C[ROWS_A][COLUMNS_B];

        printf("Matrix A: \n");
        for (int i = 0; i < ROWS_A; i++) {
            for (int j = 0; j < COLUMNS_A; j++) {
                A[i][j] = rand() % 10;
                printf("%d ", A[i][j]);
            }
            printf("\n");
        }
        printf("\n");
        printf("Matrix B: \n");
        for (int i = 0; i < COLUMNS_A; i++) {
            for (int j = 0; j < COLUMNS_B; j++) {
                B[i][j] = rand() % 10;
                printf("%d ", B[i][j]);
            }
            printf("\n");
        }
        printf("\n");

        for (int dest = 1; dest < size; dest++) {
            int index = (dest - 1) * BLOCK_ROW;
            MPI_Send(&A[index], BLOCK_A, MPI_INT, dest, 0, MPI_COMM_WORLD);
            MPI_Send(&B[0][0], SIZE_B, MPI_INT, dest, 1, MPI_COMM_WORLD);
        }

        for (int source = 1; source < size; source++) {
            int index = (source - 1) * BLOCK_ROW;
            MPI_Recv(&C[index], BLOCK_C, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
        }

        printf("Matrix C: \n");
        for (int i = 0; i < ROWS_A; i++) {
            for (int j = 0; j < COLUMNS_B; j++) {
                printf("%d ", C[i][j]);
            }
            printf("\n");
        }
    } else {
        int x[BLOCK_ROW][COLUMNS_A];
        int y[COLUMNS_A][COLUMNS_B];
        int z[BLOCK_ROW][COLUMNS_B];

        MPI_Recv(&x[0][0], BLOCK_A, MPI_INT, ZERO_PROCESS, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&y[0][0], SIZE_B, MPI_INT, ZERO_PROCESS, 1, MPI_COMM_WORLD, &status);

        for (int i = 0; i < BLOCK_ROW; i++) {
            for (int j = 0; j < COLUMNS_B; j++) {
                z[i][j] = 0;
                for (int k = 0; k < COLUMNS_A; k++) {
                    z[i][j] += x[i][k] * y[k][j];
                }
            }
        }
        MPI_Send(&z[0][0], BLOCK_C, MPI_INT, ZERO_PROCESS, 2, MPI_COMM_WORLD);
    }
    MPI_Finalize();
}

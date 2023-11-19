#include <mpi.h>
#include <random>
#include <math.h>

int main(int argc, char** argv)
{
    srand(23);

    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Status status;

    const int ZERO_PROCESS = 0;
    const int ROWS = 10;
    const int COLUMNS = 10;
    const int BLOCK_ROW = ROWS / (size - 1);
    const int BLOCK = BLOCK_ROW * COLUMNS;

    if (rank == ZERO_PROCESS) {
        int A[ROWS][COLUMNS];
        int B[ROWS][COLUMNS];
        int C[ROWS][COLUMNS];

        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLUMNS; j++) {
                A[i][j] = rand() % 10;
                B[i][j] = rand() % 10;
            }
        }

        printf("Matrix A: \n");
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLUMNS; j++) {
                printf("%d ", A[i][j]);
            }
            printf("\n");
        }
        printf("\n");
        printf("Matrix B: \n");
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLUMNS; j++) {
                printf("%d ", B[i][j]);
            }
            printf("\n");
        }
        printf("\n");

        for (int dest = 1; dest < size; dest++) {
            int row = (dest - 1) * BLOCK_ROW;
            MPI_Send(&A[row][0], BLOCK, MPI_INT, dest, 0, MPI_COMM_WORLD);
            MPI_Send(&B[row][0], BLOCK, MPI_INT, dest, 1, MPI_COMM_WORLD);
        }

        for (int source = 1; source < size; source++) {
            int row = (source - 1) * BLOCK_ROW;
            MPI_Recv(&C[row][0], BLOCK, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
        }

        printf("Vector C: \n");
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLUMNS; j++) {
                printf("%d ", C[i][j]);
            }
            printf("\n");
        }
    } else {
        int x[BLOCK_ROW][COLUMNS];
        int y[BLOCK_ROW][COLUMNS];
        int z[BLOCK_ROW][COLUMNS];

        MPI_Recv(&x[0], BLOCK, MPI_INT, ZERO_PROCESS, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&y[0], BLOCK, MPI_INT, ZERO_PROCESS, 1, MPI_COMM_WORLD, &status);

        for (int i = 0; i < BLOCK_ROW; i++) {
            for (int j = 0; j < COLUMNS; j++) {
                z[i][j] = x[i][j] * y[i][j];
            }
        }
        MPI_Send(&z[0][0], BLOCK, MPI_INT, ZERO_PROCESS, 2, MPI_COMM_WORLD);
    }
    MPI_Finalize();
}

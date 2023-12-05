#include <mpi.h>
#include <random>

int main(int argc, char** argv)
{
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Status status;

    const int ZERO_PROCESS = 0;
    const int ROWS_A = 8;
    const int ROWS_B_C = 4;
    const int COLUMNS = 8;

    if (rank == ZERO_PROCESS) {
        int A[ROWS_A][COLUMNS];
        printf("Matrix A: \n");
        for (int i = 0; i < ROWS_A; i++) {
            for (int j = 0; j < COLUMNS; j++) {
                A[i][j] = rand() % 10;
                printf("%d ", A[i][j]);
            }
            printf("\n");
        }
        printf("\n");

        MPI_Datatype datatype;
        MPI_Type_vector(ROWS_B_C, COLUMNS, COLUMNS * 2, MPI_INT, &datatype);
        MPI_Type_commit(&datatype);

        MPI_Send(&A[1][0], 1, datatype, 1, 0, MPI_COMM_WORLD);
        MPI_Send(&A[0][0], 1, datatype, 1, 1, MPI_COMM_WORLD);
    } else {
        int B[ROWS_B_C][COLUMNS];
        int C[ROWS_B_C][COLUMNS];
        MPI_Recv(&C[0][0], ROWS_B_C * COLUMNS, MPI_INT, ZERO_PROCESS, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&B[0][0], ROWS_B_C * COLUMNS, MPI_INT, ZERO_PROCESS, 1, MPI_COMM_WORLD, &status);

        printf("Matrix B: \n");
        for (int i = 0; i < ROWS_B_C; i++) {
            for (int j = 0; j < COLUMNS; j++) {
                printf("%d ", B[i][j]);
            }
            printf("\n");
        }
        printf("\n");
        printf("Matrix C: \n");
        for (int i = 0; i < ROWS_B_C; i++) {
            for (int j = 0; j < COLUMNS; j++) {
                printf("%d ", C[i][j]);
            }
            printf("\n");
        }
    }
    MPI_Finalize();
}

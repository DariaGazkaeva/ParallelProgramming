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
    const int ROWS = 10;
    const int COLUMNS = 10;
    const int BLOCK_COLUMNS = COLUMNS / (size - 1);
    const int BLOCK = BLOCK_COLUMNS * ROWS;

    if (rank == ZERO_PROCESS) {
        int A[ROWS][COLUMNS];
        int A_transposed[COLUMNS][ROWS];
        int count = 0;
        printf("Matrix A: \n");
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLUMNS; j++, count++) {
                A[i][j] = count;
                printf("%02d ", A[i][j]);
            }
            printf("\n");
        }
        printf("\n");

        MPI_Datatype datatype;
        MPI_Type_vector(ROWS, BLOCK_COLUMNS, COLUMNS, MPI_INT, &datatype);
        MPI_Type_commit(&datatype);

        for (int dest = 1; dest < size; dest++) {
            int col = (dest - 1) * BLOCK_COLUMNS;
            MPI_Send(&A[0][col], 1, datatype, dest, 0, MPI_COMM_WORLD);
        }

        for (int source = 1; source < size; source++) {
            int index = (source - 1) * BLOCK_COLUMNS;
            MPI_Recv(&A_transposed[index][0], BLOCK, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
        }

        printf("Transposed matrix: \n");
        for (int i = 0; i < COLUMNS; i++) {
            for (int j = 0; j < ROWS; j++) {
                printf("%02d ", A_transposed[i][j]);
            }
            printf("\n");
        }
    }
    else {
        int A[ROWS][BLOCK_COLUMNS];
        int A_transposed[BLOCK_COLUMNS][ROWS];

        MPI_Recv(&A[0][0], BLOCK, MPI_INT, ZERO_PROCESS, 0, MPI_COMM_WORLD, &status);

        for (int i = 0; i < BLOCK_COLUMNS; i++) {
            for (int j = 0; j < ROWS; j++) {
                A_transposed[i][j] = A[j][i];
            }
        }

        MPI_Send(&A_transposed[0][0], BLOCK, MPI_INT, ZERO_PROCESS, 1, MPI_COMM_WORLD);
    }
    MPI_Finalize();
}

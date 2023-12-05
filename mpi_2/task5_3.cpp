#include <mpi.h>
#include <random>

int main(int argc, char** argv)
{
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Status status;
    MPI_Request request;

    const int ZERO_PROCESS = 0;
    const int ROWS_A = 8;
    const int ROWS_D = 2;
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

        MPI_Datatype types[2] = { MPI_INT, MPI_INT };
        int block[2] = { ROWS_A, COLUMNS };
        MPI_Aint address[2];
        MPI_Aint displacement[2];
        MPI_Datatype datatype;

        MPI_Get_address(&A[0][0], &address[0]);
        MPI_Get_address(&A[4][0], &address[1]);
        displacement[0] = 0;
        displacement[1] = address[1] - address[0];

        MPI_Type_create_struct(2, block, displacement, types, &datatype);
        MPI_Type_commit(&datatype);

        for (int dest = 0; dest < size; dest++) {
            MPI_Isend(&A[dest][0], 1, datatype, dest, 0, MPI_COMM_WORLD, &request);
        }
    }

    int D[ROWS_D][COLUMNS];
    MPI_Irecv(&D[0][0], ROWS_D * COLUMNS, MPI_INT, ZERO_PROCESS, 0, MPI_COMM_WORLD, &request);
    MPI_Wait(&request, &status);

    printf("Rank = %d, matrix D: \n", rank);
    for (int i = 0; i < ROWS_D; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            printf("%d ", D[i][j]);
        }
        printf("\n");
    }
    MPI_Finalize();
}

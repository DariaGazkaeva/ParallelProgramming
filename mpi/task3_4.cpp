#include <mpi.h>
#include <random>

int main(int argc, char** argv) {

    srand(34);

    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int n = 100;
    const int m = 100;
    const int block = (n / size) * m;

    float A[n][m];
    float A_part[n / size][m];

    if (rank == 0) {
        printf("Matrix: \n");
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                A[i][j] = rand() % 10;
                printf("%0.1f ", A[i][j]);
            }
            printf("\n");
        }
        printf("\n");
    }

    MPI_Scatter(&A[0][0],
                block,
                MPI_FLOAT,
                &A_part[0][0],
                block,
                MPI_FLOAT,
                0,
                MPI_COMM_WORLD);

    float sum = 0;
    float max = 0;
    for (int i = 0; i < n / size; i++) {
        for (int j = 0; j < m; j++) {
            sum += A_part[i][j];
        }
        if (sum > max) {
            max = sum;
        }
        sum = 0;
    }

    float result;
    MPI_Reduce(&max, &result, 1, MPI_FLOAT, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Norm: %0.1f", result);
    }
    MPI_Finalize();
    return 0;
}

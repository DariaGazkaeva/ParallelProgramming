#include <mpi.h>
#include <random>

int main(int argc, char** argv) {

    srand(33);

    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int n = 10;
    const int m = 10;
    const int block = (n / size) * m;
    float vector[m];
    float matrix[n][m];
    float matrix_part[n / size][m];

    if (rank == 0) {
        printf("Vector: \n");
        for (int i = 0; i < m; i++) {
            vector[i] = rand() % 10;
            printf("%0.1f ", vector[i]);
        }
        printf("\n\n");
        printf("Matrix: \n");
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                matrix[i][j] = rand() % 10;
                printf("%0.1f ", matrix[i][j]);
            }
            printf("\n");
        }
        printf("\n");
    }

    MPI_Bcast(vector, m, MPI_FLOAT, 0, MPI_COMM_WORLD);
    MPI_Scatter(&(matrix[0][0]),
                block,
                MPI_FLOAT,
                &(matrix_part[0][0]),
                block,
                MPI_FLOAT, 0,
                MPI_COMM_WORLD);

    float res[n / size];
    for (int i = 0; i < n / size; i++) {
        res[i] = 0;
        for (int j = 0; j < m; j++) {
            res[i] += matrix_part[i][j] * vector[j];
        }
    }

    float* result;

    if (rank == 0) {
        result = new float[n];
    }

    MPI_Gather(res, n / size, MPI_FLOAT, result, n / size, MPI_FLOAT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Result: \n");
        for (int i = 0; i < n; i++) {
            printf("%0.1f ", result[i]);
        }
    }
    MPI_Finalize();
    return 0;
}
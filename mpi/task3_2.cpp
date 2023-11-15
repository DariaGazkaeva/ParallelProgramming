#include <mpi.h>
#include <random>

int main(int argc, char **argv) {

    srand(32);

    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int const n = 100;
    int const block = n / size;
    float x[n];
    float x_part[block];
    float y[n];
    float y_part[block];

    if (rank == 0) {
        printf("Vector X: ");
        for (int i = 0; i < n; i++) {
            x[i] = rand() % 10;
            printf("%0.1f ", x[i]);
        }
        printf("\n\n");
        printf("Vector Y: ");
        for (int i = 0; i < n; i++) {
            y[i] = rand() % 10;
            printf("%0.1f ", y[i]);
        }
        printf("\n\n");
    }

    MPI_Scatter(&x[0], block, MPI_FLOAT, &x_part[0], block, MPI_FLOAT, 0, MPI_COMM_WORLD);
    MPI_Scatter(&y[0], block, MPI_FLOAT, &y_part[0], block, MPI_FLOAT, 0, MPI_COMM_WORLD);

    float part_sum = 0;
    for (int i = 0; i < block; i++) {
        part_sum += x_part[i] * y_part[i];
    }

    float result;

    MPI_Reduce(&part_sum, &result, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Scalar product: %0.1f", result);
    }
    MPI_Finalize();
    return 0;
}

#include <mpi.h>
#include <random>

int main(int argc, char** argv) {

    srand(31);

    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int const n = 100;
    int const block = n / size;
    float vector[n];
    float part[block];

    if (rank == 0) {
        printf("Vector: ");
        for (int i = 0; i < n; i++) {
            float x = rand() % 10;
            vector[i] = x;
            printf("%0.1f ", x);
        }
        printf("\n");
        printf("Norm: ");
    }

    MPI_Scatter(&vector[0],
                block,
                MPI_FLOAT,
                &part[0],
                block,
                MPI_FLOAT,
                0,
                MPI_COMM_WORLD);

    float sum = 0;
    for (int i = 0; i < block; i++) {
        sum += std::abs(part[i]);
    }

    float res;
    MPI_Reduce(&sum, &res, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("%0.1f", res);
    }
    MPI_Finalize();
    return 0;
}

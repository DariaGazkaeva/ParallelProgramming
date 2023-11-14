#include <mpi.h>
#include <random>

int main(int argc, char** argv) {

    srand(35);

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
            float x = rand() % 100;
            vector[i] = x;
            printf("%0.1f ", x);
        }
        printf("\n");
    }

    MPI_Scatter(&vector[0],
                block,
                MPI_FLOAT,
                &part[0],
                block,
                MPI_FLOAT,
                0,
                MPI_COMM_WORLD);

    float max, local_max;
    local_max = part[0];

    for (int i = 0; i < block; i++) {
        if (part[i] > local_max) {
            local_max = part[i];
        }
    }

    float local_max_all[size * 2];
    float res[2] = {(float) rank, local_max};

    MPI_Reduce(&local_max, &max, 1, MPI_FLOAT_INT, MPI_MAXLOC, 0, MPI_COMM_WORLD);
    MPI_Gather(&res[0], 2, MPI_FLOAT, &local_max_all, 2, MPI_FLOAT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Max: %0.1f", max);
        printf("\n");
        printf("All local max: \n");

        for (int i = 1; i < size * 2; i += 2) {
            printf("%0.1f ", local_max_all[i]);
        }
        printf("\n");
        printf("Corresponding ranks: \n");
        for (int i = 0; i < size * 2; i += 2) {
            printf("%d ", (int)local_max_all[i]);
        }
        printf("\n");
    }
    MPI_Finalize();
    return 0;
}


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
    float* vector;
    float part[block];

    if (rank == 0) {
        vector = new float[n];
        printf("Vector: ");
        for (int i = 0; i < n; i++) {
            float x = rand() % 100;
            vector[i] = x;
            printf("%0.1f ", x);
        }
        printf("\n");
    }

    MPI_Scatter(vector,
                block,
                MPI_FLOAT,
                &part[0],
                block,
                MPI_FLOAT,
                0,
                MPI_COMM_WORLD);

    // находим максимум в процессе
    float local_max = part[0];
    for (int i = 0; i < block; i++) {
        if (part[i] > local_max) {
            local_max = part[i];
        }
    }

    float res[2] = {local_max, (float) rank}; // максимум в процессе + номер этого процесса
    float* local_max_all; // все максимумы из процессов + соответствующие им номера процессов
    float* max_and_rank; // номер процесса с глобальным максимумом + глобальный максимум

    if (rank == 0) {
        max_and_rank = new float[2];
        local_max_all = new float[size * 2];
    }

    MPI_Reduce(&res[0], max_and_rank, 2, MPI_FLOAT_INT, MPI_MAXLOC, 0, MPI_COMM_WORLD);
    MPI_Gather(&res[0], 2, MPI_FLOAT, local_max_all, 2, MPI_FLOAT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Max: %0.1f (rank is %d)", max_and_rank[0], (int)max_and_rank[1]);
        printf("\n");
        printf("All local max_and_rank: \n");

        for (int i = 0; i < size * 2; i += 2) {
            printf("%0.1f ", local_max_all[i]);
        }
        printf("\n");
        printf("Corresponding ranks: \n");
        for (int i = 1; i < size * 2; i += 2) {
            printf("%d ", (int)local_max_all[i]);
        }
        printf("\n");
    }
    MPI_Finalize();
    return 0;
}


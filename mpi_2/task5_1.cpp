#include <mpi.h>
#include <random>

int main(int argc, char** argv)
{
    int rank, size;
    int ranks[5] = {8, 3, 9, 1, 6};

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Status status;

    MPI_Group new_group, old_group;
    MPI_Comm_group(MPI_COMM_WORLD, &old_group);
    MPI_Group_incl(old_group, 5, ranks, &new_group);

    MPI_Comm new_comm;
    MPI_Comm_create(MPI_COMM_WORLD, new_group, &new_comm);

    int new_rank = -1;
    if (new_comm != MPI_COMM_NULL) {
        MPI_Comm_rank(new_comm, &new_rank);

        float array[10];
        if (new_rank == 0) {
            printf("Rank = %d, Array: \n", new_rank);
            for (int i = 0; i < 10; i++) {
                array[i] = rand() % 10;
                printf("%f ", array[i]);
            }
            printf("\n");
        }
        MPI_Bcast(array, 10, MPI_FLOAT, 0, new_comm);
        if (new_rank != 0) {
            printf("Rank = %d, Array: \n", new_rank);
            for (int i = 0; i < 10; i++) {
                printf("%f ", array[i]);
            }
            printf("\n");
        }

        if (new_rank == 4) {
            MPI_Send(&array[0], 10, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
        }
    }

    if (rank == 0) {
        float array2[10];
        MPI_Recv(array2, 10, MPI_FLOAT, ranks[4], 0, MPI_COMM_WORLD, &status);
        printf("From old group. Rank = %d, Array: \n", rank);
        for (int i = 0; i < 10; i++) {
            printf("%f ", array2[i]);
        }
        printf("\n");
    }
    MPI_Group_free(&old_group);
    MPI_Group_free(&new_group);
    if (new_comm != MPI_COMM_NULL) {
        MPI_Comm_free(&new_comm);
    }
    MPI_Finalize();
}

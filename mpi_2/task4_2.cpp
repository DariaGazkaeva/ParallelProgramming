#include <mpi.h>
#include <string>
#include <random>

int main(int argc, char** argv)
{
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Status status;

    const int PROCESS_ZERO = 0;
    const int n = 12;
    const int block = (n / size) + 1;
    MPI_Request request;

    if (rank == PROCESS_ZERO) {
        int array[n];
        for (int i = 0; i < n; i++) {
            array[i] = rand() % 10;
            printf("%d ", array[i]);
        }
        printf("\n");
        int dest = 0;
        int sent = 0;
        for (; dest < size, sent < n - block + 1; dest++, sent += block) {
            MPI_Isend(&array[sent], block, MPI_INT, dest, 0, MPI_COMM_WORLD, &request);
        }
        MPI_Isend(&array[sent], n - sent, MPI_INT, dest, 0, MPI_COMM_WORLD, &request);
    }
    if (rank * block < n) {
        int part[block];
        MPI_Irecv(&part[0], block, MPI_INT, PROCESS_ZERO, 0, MPI_COMM_WORLD, &request);
        MPI_Wait(&request, &status);
        int count;
        MPI_Get_count(&status, MPI_INT, &count);

        std::string string;
        for (int i = 0; i < count; i++) {
            string += std::to_string(part[i]);
            string += " ";
        }
        printf("Count = %d, rank = %d, elements: %s \n", count, rank, string.c_str());
    }
    MPI_Finalize();
}
